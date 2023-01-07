#pragma once

#include <queue>

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/uart/uart.h"

#include "command.h"
#include "property.h"
#include "switch.h"
#include "setpoint.h"
#include "mode_select.h"

namespace esphome {
namespace xiaomi {

class DeermaHumidifier : public Component, public uart::UARTDevice {

public:
    Switch *power;
    Switch *led;
    Switch *sound;
    Setpoint *humidity_setpoint;
    ModeSelect *mode;
    sensor::Sensor *temperature;
    sensor::Sensor *humidity;
    binary_sensor::BinarySensor *tank_removed;
    binary_sensor::BinarySensor *tank_empty;

    static constexpr const char *model = "xiaomi.dev.1";

protected:
    enum class Mode : uint8_t {
        LOW_MODE = 1,
        MEDIUM_MODE = 2,
        HIGH_MODE = 3,
        SETPOINT_MODE = 4,
    };

    std::vector<char> buffer;
    std::queue<Command> queue;

    static
    const char * mode_to_string(Mode m);

    int writeline(const std::string &line);
    int readline(std::string &line);
    void on_changed_property(const Property &prop);

    template <typename ...Params>
    void down_command(const std::string &name, Params && ...params)
    {
        queue.emplace(name, std::vector<std::string>{ std::forward<Params>(params)... });
    }

public:
    DeermaHumidifier();

    void setup() override;
    void loop() override;

    float get_setup_priority() const override
    {
        return esphome::setup_priority::LATE;
    }

    void set_property(uint8_t siid, uint8_t piid, std::string value)
    {
        ESP_LOGD("xiaomi-deerma", "Property set: %d %d %s", siid, piid, value.c_str());

        down_command("set_properties", std::to_string(siid), std::to_string(piid), value);
    }

    void set_property(uint8_t siid, uint8_t piid, bool value)
    {
        std::string str_value = value ? "true" : "false";

        set_property(siid, piid, str_value);
    }

    void set_property(uint8_t siid, uint8_t piid, float value)
    {
        set_property(siid, piid, std::to_string(value));
    }

    void set_property(uint8_t siid, uint8_t piid, int value)
    {
        set_property(siid, piid, std::to_string(value));
    }
};

} // namespace xiaomi
} // namespace esphome