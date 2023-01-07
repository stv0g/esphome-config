// SPDX-FileCopyrightText: 2023 Steffen Vogel <post@steffenvogel.de>
// SPDX-License-Identifier: GPL-3.0

#pragma once

#include "esphome/components/switch/switch.h"

namespace esphome {
namespace xiaomi {

// Forward declarations
class DeermaHumidifier;

class Switch : public switch_::Switch {

    DeermaHumidifier *humidifier;

    uint8_t siid;
    uint8_t piid;

public:
    Switch(DeermaHumidifier *hum, uint8_t sid, uint8_t pid) :
        humidifier(hum),
        siid(sid),
        piid(pid)
    { }

    virtual
    void write_state(bool state) override;
};

} // namespace xiaomi
} // namespace esphome