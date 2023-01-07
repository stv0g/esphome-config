#pragma once

#include <string>
#include <vector>

namespace esphome {
namespace xiaomi {

class Property {

public:
    Property(uint8_t sid, uint8_t pid, const std::string &val) :
        siid(sid),
        piid(pid),
        value(val)
    { }

    uint8_t siid;
    uint8_t piid;
    std::string value;

    static
    std::vector<Property> from_params(const std::vector<std::string> &params)
    {
        std::vector<Property> props;

        for (auto it = params.cbegin(); it+0 != params.cend() && it+1 != params.cend() && it+2 != params.cend(); ++it) {
            auto siid = std::stoi(*(it+0));
            auto piid = std::stoi(*(it+1));
            auto value = *(it+2);

            props.emplace_back(siid, piid, value);
        }

        return props;
    }
};

} // namespace xiaomi
} // namespace esphome