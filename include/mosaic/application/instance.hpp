#pragma once

#include <mosaic/macros/exposure.hpp>

#include <mosaic/application/application.hpp>
#include <mosaic/application/resources.hpp>

#include <mosaic/ecs/manager.hpp>

#include <mosaic/events/manager.hpp>

#include <mosaic/debug/console.hpp>

#include <mosaic/window/window.hpp>

#include <cstdint>
#include <type_traits>

namespace Mosaic
{
    template <typename T> requires std::is_base_of_v<Application, T>
    class MOSAIC_PUBLIC_EXPOSURE Instance
    {
    public:
        Instance();

        void Setup();

        std::int32_t Run();

    private:
        void OnApplicationExit(ApplicationResources&, const ApplicationExitEvent& event);

        T mInstance;

        ApplicationResources mApplicationResources;

        ECSManager mECSManager;
        EventManager mEventManager;

        Console mConsole;
        Window mWindow;

        bool mRunning;

        std::int32_t mExitCode;
    };
}

#include <mosaic/inline/application/instance.inl>