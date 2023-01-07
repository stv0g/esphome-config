#pragma once

#include <sstream>
#include <iterator>

namespace esphome {
namespace xiaomi {

class Command {

public:
    Command(const std::string &line)
    {
        std::istringstream ss(line);

        ss >> name;

        std::istream_iterator<std::string> begin(ss);
        std::istream_iterator<std::string> end;

        parameters = std::vector<std::string>(begin, end);
    }

    Command(const std::string &nme, const std::vector<std::string> params) :
        name(nme),
        parameters(params)
    { }

    std::string serialize() const
    {
        std::stringstream ss;

        ss << name;

        for (auto & param : parameters)
            ss << " " << param;

        return ss.str();
    }

    std::string name;
    std::vector<std::string> parameters;
};

} // namespace xiaomi
} // namespace esphome