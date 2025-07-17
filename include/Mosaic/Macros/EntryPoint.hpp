#pragma once

#if !defined(MOSAIC_PLATFORM_UNSUPPORTED)

#if !defined(MOSAIC_BUILD_ENVIRONMENT)

#include <Mosaic/Application/Application.hpp>

#include <memory>

#define MOSAIC_ENTRY_POINT(ApplicationType)                                                                           \
    int main(int argc, char** argv)                                                                                   \
    {                                                                                                                 \
        std::vector<std::string> arguments(argv, argv + argc);                                                        \
                                                                                                                      \
        std::unique_ptr<Mosaic::Application::Application> application = std::make_unique<ApplicationType>(arguments); \
                                                                                                                      \
        application->OnStart();                                                                                       \
                                                                                                                      \
        while (application->ShouldUpdate())                                                                           \
        {                                                                                                             \
            application->OnUpdate();                                                                                  \
        }                                                                                                             \
                                                                                                                      \
        application->OnClose();                                                                                       \
                                                                                                                      \
        return 0;                                                                                                     \
    }

#endif

#else

#define MOSAIC_ENTRY_POINT(ApplicationType)

#error Unsupported platform for Mosaic

#endif