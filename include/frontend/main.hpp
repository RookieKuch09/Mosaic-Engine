#pragma once

#include "instance.hpp"

int main()
{
    auto instance = Mosaic::Frontend::Instance::Create();

    return instance->Run();
}