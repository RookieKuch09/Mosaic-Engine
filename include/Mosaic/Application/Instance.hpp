#pragma once

#include <Mosaic/Macros/Exposure.hpp>

#include <Mosaic/Application/Application.hpp>
#include <Mosaic/Application/Resources.hpp>

#include <Mosaic/ECS/ECSManager.hpp>
#include <Mosaic/ECS/EventManager.hpp>

#include <Mosaic/Debug/Console.hpp>

#include <Mosaic/Window/Window.hpp>

namespace Mosaic
{
    template <typename T> requires std::is_base_of_v<Application, T>
    class MOSAIC_PUBLIC_EXPOSURE Instance
    {
    public:
        Instance(const Instance& other) = delete;
        Instance(Instance&& other) noexcept = delete;

        Instance& operator=(const Instance& other) = delete;
        Instance& operator=(Instance&& other) noexcept = delete;

        Instance();
        ~Instance() = default;

        void Setup();

        std::int32_t Run();

    private:
        void OnApplicationExit(InstanceResources&, const ApplicationExitEvent& event);

        T mInstance;

        InstanceResources mInstanceResources;

        ECSManager mECSManager;
        EventManager mEventManager;

        Console mConsole;
        Window mWindow;

        bool mRunning;

        std::int32_t mExitCode;
    };
}

#include <Mosaic/Inline/Application/Instance.inl>