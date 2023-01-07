// SPDX-FileCopyrightText: 2023 Steffen Vogel <post@steffenvogel.de>
// SPDX-License-Identifier: GPL-3.0

#include "mode_select.h"
#include "humidifier.h"

using namespace esphome::xiaomi;

void ModeSelect::control(const std::string &value)
{
    int m = -1;

    if      (value == "low")
        m = 1;
    else if (value == "medium")
        m = 2;
    else if (value == "high")
        m = 3;
    else if (value == "setpoint")
        m = 4;
    
    if (m >= 0)
        humidifier->set_property(siid, piid, m);
}