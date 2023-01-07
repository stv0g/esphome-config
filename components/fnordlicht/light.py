# SPDX-FileCopyrightText: 2023 Steffen Vogel <post@steffenvogel.de>
# SPDX-License-Identifier: Apache-2.0

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, light
from esphome.const import (
    CONF_OUTPUT_ID,
)

CODEOWNERS = ["@stv0g"]

DEPENDENCIES = ["uart"]

fnordlicht_ns = cg.esphome_ns.namespace("fnordlicht")

Output = fnordlicht_ns.class_(
    "Output", cg.Component, light.LightOutput, uart.UARTDevice
)

CONFIG_SCHEMA =  light.RGB_LIGHT_SCHEMA.extend(
    cv.Schema(
        {
            cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(Output),
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await light.register_light(var, config)
