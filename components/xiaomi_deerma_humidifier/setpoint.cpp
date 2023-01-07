// SPDX-FileCopyrightText: 2023 Steffen Vogel <post@steffenvogel.de>
// SPDX-License-Identifier: GPL-3.0

#include "setpoint.h"
#include "humidifier.h"

using namespace esphome::xiaomi;

void Setpoint::control(float value)
{
    humidifier->set_property(siid, piid, (int) value);
}