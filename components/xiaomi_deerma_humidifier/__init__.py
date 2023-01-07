# SPDX-FileCopyrightText: 2023 Steffen Vogel <post@steffenvogel.de>
# SPDX-License-Identifier: Apache-2.0

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, number, sensor, switch, select, binary_sensor
from esphome.const import (
    CONF_ID,
    CONF_NAME,
    CONF_TEMPERATURE,
    CONF_HUMIDITY,
    CONF_MODE,
    CONF_POWER,
    CONF_LED,
    CONF_INTERNAL
)

CONF_SOUND = "sound"
CONF_HUMIDITY_SETPOINT = "humidity_setpoint"
CONF_TANK_REMOVED = "tank_removed"
CONF_TANK_EMPTY = "tank_empty"

CODEOWNERS = ["@stv0g"]

DEPENDENCIES = ["uart"]

AUTO_LOAD = ["number", "select", "switch", "sensor", "binary_sensor"]

xiaomi_ns = cg.esphome_ns.namespace("xiaomi")

DeermaHumidifier = xiaomi_ns.class_(
    "DeermaHumidifier", cg.Component, uart.UARTDevice
)

Switch = xiaomi_ns.class_(
    "Switch", switch.Switch
)

Setpoint = xiaomi_ns.class_(
    "Setpoint", number.Number
)

ModeSelect = xiaomi_ns.class_(
    "ModeSelect", select.Select
)

# Schemas

TEMPERATURE_SCHEMA = sensor.sensor_schema(sensor.Sensor,
    unit_of_measurement="°C",
    accuracy_decimals=0,
    device_class="temperature",
    state_class="measurement",
)

HUMIDITY_SCHEMA = sensor.sensor_schema(sensor.Sensor,
    unit_of_measurement="%",
    accuracy_decimals=0,
    device_class="humidity",
    state_class="measurement"
)

POWER_SCHEMA = switch.switch_schema(Switch,
)

SOUND_SCHEMA = switch.switch_schema(Switch,
    entity_category=cv.ENTITY_CATEGORY_CONFIG
)

LED_SCHEMA = switch.switch_schema(Switch,
    entity_category=cv.ENTITY_CATEGORY_CONFIG
)

HUMIDITY_SETPOINT_SCHEMA = number.NUMBER_SCHEMA

MODE_SCHEMA = select.SELECT_SCHEMA

TANK_EMPTY_SCHEMA = binary_sensor.binary_sensor_schema(binary_sensor.BinarySensor,
    entity_category=cv.ENTITY_CATEGORY_DIAGNOSTIC
)

TANK_REMOVED_SCHEMA = binary_sensor.binary_sensor_schema(binary_sensor.BinarySensor,
    entity_category=cv.ENTITY_CATEGORY_DIAGNOSTIC
)

# Default values

TEMPERATURE_DEFAULT = TEMPERATURE_SCHEMA({
    CONF_ID: "temperature",
    CONF_NAME: "Temperature"
})

HUMIDITY_DEFAULT = HUMIDITY_SCHEMA({
    CONF_ID: "humiditiy",
    CONF_NAME: "Humiditiy"
})

POWER_DEFAULT = POWER_SCHEMA({
    CONF_ID: "state",
    CONF_NAME: "On / Off"
})

SOUND_DEFAULT = SOUND_SCHEMA({
    CONF_ID: "sound",
    CONF_NAME: "Sound"
})

LED_DEFAULT = LED_SCHEMA({
    CONF_ID: "led",
    CONF_NAME: "LED"
})

HUMIDITY_SETPOINT_DEFAULT = HUMIDITY_SETPOINT_SCHEMA({
    CONF_ID: "humidity_setpoint",
    CONF_NAME: "Humidity setpoint"
})

MODE_DEFAULT = MODE_SCHEMA({
    CONF_ID: "mode",
    CONF_NAME: "Mode"
})

TANK_EMPTY_DEFAULT = TANK_EMPTY_SCHEMA({
    CONF_ID: "tank_empty",
    CONF_NAME: "Tank empty"
})

TANK_REMOVED_DEFAULT = TANK_REMOVED_SCHEMA({
    CONF_ID: "tank_removed",
    CONF_NAME: "Tank removed"
})

TEMPERATURE_DEFAULT[CONF_INTERNAL] = False
HUMIDITY_DEFAULT[CONF_INTERNAL] = False
POWER_DEFAULT[CONF_INTERNAL] = False
SOUND_DEFAULT[CONF_INTERNAL] = False
LED_DEFAULT[CONF_INTERNAL] = False
HUMIDITY_SETPOINT_DEFAULT[CONF_INTERNAL] = False
MODE_DEFAULT[CONF_INTERNAL] = False
TANK_EMPTY_DEFAULT[CONF_INTERNAL] = False
TANK_REMOVED_DEFAULT[CONF_INTERNAL] = False

# Main schema

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(DeermaHumidifier),
            cv.Optional(CONF_TEMPERATURE, default=TEMPERATURE_DEFAULT): TEMPERATURE_SCHEMA,
            cv.Optional(CONF_HUMIDITY, default=HUMIDITY_DEFAULT): HUMIDITY_SCHEMA,
            cv.Optional(CONF_POWER, default=POWER_DEFAULT): POWER_SCHEMA,
            cv.Optional(CONF_SOUND, default=SOUND_DEFAULT): SOUND_SCHEMA,
            cv.Optional(CONF_LED, default=LED_DEFAULT): LED_SCHEMA,
            cv.Optional(CONF_HUMIDITY_SETPOINT, default=HUMIDITY_SETPOINT_DEFAULT): HUMIDITY_SETPOINT_SCHEMA,
            cv.Optional(CONF_MODE, default=MODE_DEFAULT): MODE_SCHEMA,
            cv.Optional(CONF_TANK_EMPTY, default=TANK_EMPTY_DEFAULT): TANK_EMPTY_SCHEMA,
            cv.Optional(CONF_TANK_REMOVED, default=TANK_REMOVED_DEFAULT): TANK_REMOVED_SCHEMA,
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    await uart.register_uart_device(var, config)

    await number.register_number(var.humidity_setpoint, config[CONF_HUMIDITY_SETPOINT],
        min_value=40,
        max_value=70,
        step=1)

    await select.register_select(var.mode, config[CONF_MODE],
        options=["low", "medium", "high", "setpoint"])

    await sensor.register_sensor(var.temperature, config[CONF_TEMPERATURE])
    await sensor.register_sensor(var.humidity, config[CONF_HUMIDITY])
    await switch.register_switch(var.power, config[CONF_POWER])
    await switch.register_switch(var.led, config[CONF_LED])
    await switch.register_switch(var.sound, config[CONF_SOUND])
    await binary_sensor.register_binary_sensor(var.tank_empty, config[CONF_TANK_EMPTY])
    await binary_sensor.register_binary_sensor(var.tank_removed, config[CONF_TANK_REMOVED])
