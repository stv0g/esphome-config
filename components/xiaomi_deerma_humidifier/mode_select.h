#pragma once

#include "esphome/components/select/select.h"

namespace esphome {
namespace xiaomi {

// Forward declarations
class DeermaHumidifier;

class ModeSelect : public select::Select {

    DeermaHumidifier *humidifier;

    uint8_t siid;
    uint8_t piid;

public:
    ModeSelect(DeermaHumidifier *hum, uint8_t sid, uint8_t pid) :
        humidifier(hum),
        siid(sid),
        piid(pid)
    { }

    virtual
    void control(const std::string &value) override;
};

} // namespace xiaomi
} // namespace esphome