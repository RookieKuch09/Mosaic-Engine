#pragma once

#include "instance.hpp"

int main()
{
    Mosaic::Frontend::Instance* instance = Mosaic::Frontend::Instance::Create();

    return instance->Run();
}