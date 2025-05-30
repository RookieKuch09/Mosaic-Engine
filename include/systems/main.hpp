#pragma once

#include "instance.hpp"

int main()
{
    auto instance = Mosaic::Systems::Instance::Create();

    return instance->Run();
}