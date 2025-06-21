#pragma once

#include <Mosaic/Window/Resources.hpp>

#include <Mosaic/Macros/Exposure.hpp>

namespace Mosaic
{
    class Application;
    class InstanceResources;

    class MOSAIC_PUBLIC_EXPOSURE Window
    {
    public:
        Window(const Window& other) = delete;
        Window(Window&& other) noexcept = default;

        Window& operator=(const Window& other) = delete;
        Window& operator=(Window&& other) noexcept = delete;

        WindowResources& GetResources();
        const WindowResources& GetResources() const;

    private:
        class Backend
        {
        private:
            struct Storage;

            Backend(WindowResources& windowResources, InstanceResources& instanceResources);
            ~Backend();

            void Create();
            void Destroy();

            void GetState();
            void SetState();

            void SetSize(WindowSize size);
            void SetPosition(WindowPosition position);
            void SetTitle(WindowTitle title);
            void SetVisibility(WindowVisibility visibility);

            void Update();

            friend class Window;

            Storage* mStorage;

            WindowResources& mWindowResources;
            InstanceResources& mInstanceResources;
        };

        Window(InstanceResources& resources);
        ~Window();

        void Setup();
        void Update();

        Backend mBackend;

        WindowResources mWindowResources;
        WindowResources mWindowResourcesCopy;

        InstanceResources& mInstanceResources;

        template <typename T> requires std::is_base_of_v<Application, T>
        friend class Instance;
    };

    struct WindowSizeChangeEvent
    {
        WindowSize Size;
    };

    struct WindowPositionChangeEvent
    {
        WindowPosition Position;
    };

    struct WindowVisibilityChangeEvent
    {
        WindowVisibility Visibility;
    };

    struct WindowTitleChangeEvent
    {
        WindowTitle Title;
    };
}