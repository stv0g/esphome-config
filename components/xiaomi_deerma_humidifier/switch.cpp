#include "switch.h"
#include "humidifier.h"

using namespace esphome::xiaomi;

void Switch::write_state(bool state)
{
    humidifier->set_property(siid, piid, state);
}