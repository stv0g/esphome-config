#pragma once

#include "esphome/components/number/number.h"

namespace esphome {
namespace xiaomi {

// Forward declarations
class DeermaHumidifier;

class Setpoint : public number::Number {

    DeermaHumidifier *humidifier;

    uint8_t siid;
    uint8_t piid;

public:
    Setpoint(DeermaHumidifier *hum, uint8_t sid, uint8_t pid) :
        humidifier(hum),
        siid(sid),
        piid(pid)
    { }

    virtual
    void control(float value) override;
};

} // namespace xiaomi
} // namespace esphome