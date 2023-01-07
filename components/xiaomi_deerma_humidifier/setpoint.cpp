#include "setpoint.h"
#include "humidifier.h"

using namespace esphome::xiaomi;

void Setpoint::control(float value)
{
    humidifier->set_property(siid, piid, (int) value);
}