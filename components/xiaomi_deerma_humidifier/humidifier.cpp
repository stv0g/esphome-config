// SPDX-FileCopyrightText: 2023 Steffen Vogel <post@steffenvogel.de>
// SPDX-License-Identifier: GPL-3.0

#include "humidifier.h"

using namespace esphome::xiaomi;

DeermaHumidifier::DeermaHumidifier() :
    buffer(512)
{
    power = new Switch(this, 2, 1);
    led = new Switch(this, 6, 1);
    sound = new Switch(this, 5, 1);
    humidity_setpoint = new Setpoint(this, 2, 6);
    mode = new ModeSelect(this, 2, 5);
    temperature = new sensor::Sensor();
    humidity = new sensor::Sensor();
    tank_removed = new binary_sensor::BinarySensor();
    tank_empty = new binary_sensor::BinarySensor();
}

void DeermaHumidifier::setup()
{
    buffer.clear();

    down_command("MIIO_net_change", "cloud");
}

void DeermaHumidifier::loop()
{
    std::string line;

    while (available()) {
        if (readline(line) <= 0)
            continue;

        Command cmd(line);

        if (cmd.name == "get_down") {
            if (queue.empty()) {
                writeline("down none");
            } else {
                writeline("down " + queue.front().serialize());
                queue.pop();
            }
        } else if (cmd.name == "properties_changed") {
            for (const auto &prop : Property::from_params(cmd.parameters))
                on_changed_property(prop);

            writeline("ok");
        } else if (cmd.name == "model") {
            if (cmd.parameters.size() > 0) {
                ESP_LOGI("xiaomi-deerma", "Model: %s", cmd.parameters[0].c_str());

                writeline("ok");
            } else {
                writeline(model);
            }
        } else if (cmd.name == "mcu_version") {
            writeline("ok");
        } else if (cmd.name == "ble_config") {
            // Not supported yet
            writeline("ok");
        } else if (cmd.name == "result") {
            writeline("ok");
        } else if (cmd.name == "error") {
            writeline("ok");
        } else if (cmd.name == "event_occured") {
            writeline("ok");
        } else if (cmd.name == "reboot") {
            writeline("ok");
        } else if (cmd.name == "restore") {
            writeline("ok");
        } else if (cmd.name == "net") {
            writeline("cloud");
        } else if (cmd.name == "time") {
            if (cmd.parameters.size() > 0) {
                if (cmd.parameters[0] == "posix") {
                    writeline("1434446397");
                } else {
                    writeline("error");
                }
            } else {
                writeline("2023-01-01 01:01:01");
            }
        } else if (cmd.name == "mac") {
            writeline("34ce00892ab7");
        } else if (cmd.name == "version") {
            writeline("1.0.0");
        }
    }
}

void DeermaHumidifier::on_changed_property(const Property &prop)
{
    ESP_LOGD("xiaomi-deerma", "Property changed: %d %d %s", prop.siid, prop.piid, prop.value.c_str());

    if        (prop.siid == 2 && prop.piid == 1) {
        auto state = prop.value == "true";
        power->publish_state(state);
    } else if (prop.siid == 6 && prop.piid == 1) {
        auto state = prop.value == "true";
        led->publish_state(state);
    } else if (prop.siid == 5 && prop.piid == 1) {
        auto state = prop.value == "true";
        sound->publish_state(state);
    } else if (prop.siid == 2 && prop.piid == 5) {
        auto state = (Mode) std::stoi(prop.value);
        mode->publish_state(mode_to_string(state));
    } else if (prop.siid == 3 && prop.piid == 1) {
        auto state = std::stof(prop.value);
        humidity->publish_state(state);
    } else if (prop.siid == 3 && prop.piid == 7) {
        auto state = std::stof(prop.value);
        temperature->publish_state(state);
    } else if (prop.siid == 2 && prop.piid == 6) {
        auto state = std::stof(prop.value);
        humidity_setpoint->publish_state(state);
    } else if (prop.siid == 7 && prop.piid == 2) {
        auto state = prop.value == "true";
        tank_removed->publish_state(state);
    } else if (prop.siid == 7 && prop.piid == 1) {
        auto state = prop.value == "true";
        tank_empty->publish_state(state);
    } else {
        ESP_LOGW("xiaomi-deerma", "Unknown property: %d %d %s", prop.siid, prop.piid, prop.value.c_str());
    }
}

int DeermaHumidifier::writeline(const std::string &line)
{
    ESP_LOGI("xiaomi-deerma", "Sent: %s", line.c_str());

    write_array((uint8_t *) line.c_str(), line.size());
    write_byte('\r');

    return line.size();
}

int DeermaHumidifier::readline(std::string &line)
{
    int readch = read();
    if (readch > 0) {
        switch (readch) {
            case '\n': // Ignore new-lines
                break;

            case '\r': { // Return on CR
                if (buffer.size() == 0)
                    return -1;

                line = std::string(buffer.begin(), buffer.end());
                auto sz = buffer.size();
                buffer.clear();

                ESP_LOGI("xiaomi-deerma", "Received: %s", line.c_str());

                return sz;
            }

            default:
                if (buffer.size() < buffer.capacity())
                    buffer.push_back(readch);
        }
    }

    // No end of line has been found, so return -1.
    return -1;
}

const char * DeermaHumidifier::mode_to_string(Mode m)
{
    switch (m) {
        case Mode::LOW_MODE:
            return "low";
        case Mode::MEDIUM_MODE:
            return "medium";
        case Mode::HIGH_MODE:
            return "high";
        case Mode::SETPOINT_MODE:
            return "setpoint";
        default:
            return "unknown";
    }
}