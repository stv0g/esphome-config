// SPDX-FileCopyrightText: 2023 Steffen Vogel <post@steffenvogel.de>
// SPDX-License-Identifier: GPL-3.0

#include "switch.h"
#include "humidifier.h"

using namespace esphome::xiaomi;

void Switch::write_state(bool state)
{
    humidifier->set_property(siid, piid, state);
}