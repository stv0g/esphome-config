// SPDX-FileCopyrightText: 2023 Steffen Vogel <post@steffenvogel.de>
// SPDX-License-Identifier: GPL-3.0

#pragma once

#include "esphome/components/uart/uart.h"

namespace esphome {
namespace fnordlicht {

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
    BOOT_ENTER_APP    = 0x87  // start application
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

    void send(uart::UARTDevice *dev)
    {
        dev->write_array(reinterpret_cast<uint8_t*>(this), sizeof(Command));
    }
};

} // namespace fnordlicht
} // namespace esphome