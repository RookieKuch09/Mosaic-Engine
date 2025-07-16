#pragma once

#include <Mosaic/Utilities/Terminal/Input.hpp>

#include <iostream>
#include <sstream>
#include <string>

namespace Mosaic::Utilities::Terminal
{
    template <typename T>
    std::optional<T> Get()
    {
        std::string input;

        std::getline(std::cin, input);

        std::istringstream buffer(input);

        T output;

        buffer >> output;

        if (buffer.fail() || !buffer.eof())
        {
            return std::nullopt;
        }

        return output;
    }
}