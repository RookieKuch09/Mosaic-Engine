#pragma once

#include <frontend/instance.hpp>

int main()
{
    Mosaic::Frontend::Instance* instance = Mosaic::Frontend::Instance::Create();

    instance->Setup();

    return instance->Run();
}