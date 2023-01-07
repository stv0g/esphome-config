// SPDX-FileCopyrightText: 2023 Steffen Vogel <post@steffenvogel.de>
// SPDX-License-Identifier: GPL-3.0

#pragma once

#include "esphome/components/light/light_output.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace fnordlicht {

class Output : public Component, public light::LightOutput, public uart::UARTDevice {

public:
    Output() :
        init_completed(false)
    { }

    void setup() override;

    light::LightTraits get_traits() override;

    void write_state(light::LightState *state) override;

protected:
    bool init_completed;

    void cmd_fade_rgb(float red, float green, float blue, uint8_t addr = BROADCAST, uint8_t step = 0xff, uint8_t delay = 0);
    void cmd_powerdown(uint8_t addr = BROADCAST);
    void cmd_stop(uint8_t addr = BROADCAST);
    void cmd_sync();
};

} // namespace fnordlicht
} // namespace esphome