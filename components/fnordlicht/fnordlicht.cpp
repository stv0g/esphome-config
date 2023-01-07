// SPDX-FileCopyrightText: 2023 Steffen Vogel <post@steffenvogel.de>
// SPDX-License-Identifier: GPL-3.0

#include "command.h"
#include "fnordlicht.h"

using namespace esphome;
using namespace esphome::fnordlicht;

void Output::setup()
{
    cmd_sync();
    flush();
    sleep(1);

    cmd_stop();
    flush();

    ESP_LOGI("fnordlicht", "Init completed");
}

light::LightTraits Output::get_traits()
{
    auto traits = light::LightTraits();

    traits.set_supported_color_modes({
        light::ColorMode::RGB,
        light::ColorMode::BRIGHTNESS
    });

    return traits;
}

void Output::write_state(light::LightState *state)
{
    float red, green, blue;

    state->current_values_as_rgb(&red, &green, &blue);

    cmd_fade_rgb(red, green, blue);
}

void Output::cmd_fade_rgb(float red, float green, float blue, uint8_t addr, uint8_t step, uint8_t delay)
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

void Output::cmd_powerdown(uint8_t addr)
{
    Command cmd(addr, CommandType::POWERDOWN);

    cmd.send(this);

    ESP_LOGI("fnordlicht", "Light powered down");
}

void Output::cmd_stop(uint8_t addr)
{
    Command cmd(addr, CommandType::STOP);

    cmd.payload[0] = 1;

    cmd.send(this);

    ESP_LOGI("fnordlicht", "Light powered down");
}

void Output::cmd_sync()
{
    uint8_t cmd[16] = { SYNC };
    cmd[15] = 0x0;

    write_array(cmd, sizeof(cmd));

    ESP_LOGI("fnordlicht", "Light synced");
}