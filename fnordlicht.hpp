#include "esphome.h"

class FnordlichtOutput : public Component, public LightOutput, public UARTDevice {
public:
    FnordlichtOutput(UARTComponent *parent) :
        UARTDevice(parent),
        init_completed(false)
    { }

    void setup() override
    {
        cmd_sync();
        flush();
        sleep(1);

        cmd_stop();
        flush();

        ESP_LOGI("fnordlich", "Init completed");
    }

    LightTraits get_traits() override
    {
        auto traits = LightTraits();
        
        traits.set_supported_color_modes({ColorMode::RGB, ColorMode::BRIGHTNESS});
        
        return traits;
    }

    void write_state(LightState *state) override
    {
        float red, green, blue;

        state->current_values_as_rgb(&red, &green, &blue);

        cmd_fade_rgb(red, green, blue);
    }

protected:
    bool init_completed;

    static constexpr uint8_t BROADCAST = 0xff;
    static constexpr uint8_t SYNC = 0x1B; // sync-sequence

    enum struct CommandType : uint8_t {
        FADE_RGB          = 0x01, // set color/fade to color (RGB)
        FADE_HSV          = 0x02, // set color/fade to color (HSV)
        SAVE_RGB          = 0x03, // save color to EEPROM (RGB)
        SAVE_HSV          = 0x04, // save color to EEPROM (HSV)
        SAVE_CURRENT      = 0x05, // save current color to EEPROM
        CONFIG_OFFSETS    = 0x06, // set global offset values
        START_PROGRAM     = 0x07, // start program
        STOP              = 0x08, // stop color changing
        MODIFY_CURRENT    = 0x09, // modify current color
        PULL_INT          = 0x0A, // pull down INT line
        CONFIG_STARTUP    = 0x0B, // configure startup
        POWERDOWN         = 0x0C, // power down the device
        BOOTLOADER        = 0x80, // start bootloader
        BOOT_CONFIG       = 0x81, // configure bootloader
        BOOT_INIT         = 0x82, // initialize bootloader data buffer
        BOOT_DATA         = 0x83, // store data in bootloader data buffer
        BOOT_CRC_CHECK    = 0x84, // compare check-sum
        BOOT_CRC_FLASH    = 0x85, // compare check-sum with flash
        BOOT_FLASH        = 0x86, // write provided data to flash
        BOOT_ENTER_APP    = 0x87 // start application
    };

    struct __attribute__ ((packed)) Command {
        uint8_t address;
        CommandType command;
        uint8_t payload[13];

        Command(uint8_t addr = BROADCAST, CommandType cmd = CommandType::POWERDOWN) :
            address(addr),
            command(cmd),
            payload{0}
        { }

        void send(UARTDevice *dev)
        {
            dev->write_array(reinterpret_cast<uint8_t*>(this), sizeof(Command));
        }
    };

    void cmd_fade_rgb(float red, float green, float blue, uint8_t addr = BROADCAST, uint8_t step = 0xff, uint8_t delay = 0)
    {
        Command cmd(addr, CommandType::FADE_RGB);

        cmd.payload[0] = step;
        cmd.payload[1] = delay;
        cmd.payload[2] = 0xff * red;
        cmd.payload[3] = 0xff * green;
        cmd.payload[4] = 0xff * blue;

        cmd.send(this);

        ESP_LOGI("fnordlicht", "Faded to (%x %x %x)", cmd.payload[2], cmd.payload[3], cmd.payload[4]);
    }

    void cmd_powerdown(uint8_t addr = BROADCAST)
    {
        Command cmd(addr, CommandType::POWERDOWN);

        cmd.send(this);

        ESP_LOGI("fnordlicht", "Light powered down");
    }

    void cmd_stop(uint8_t addr = BROADCAST)
    {
        Command cmd(addr, CommandType::STOP);

        cmd.payload[0] = 1;

        cmd.send(this);

        ESP_LOGI("fnordlicht", "Light powered down");
    }

    void cmd_sync()
    {
        uint8_t cmd[16] = { SYNC };
        cmd[15] = 0x0;

        write_array(cmd, sizeof(cmd));

        ESP_LOGI("fnordlicht", "Light synced");
    }
};