#pragma once

#include <mosaic/window/resources.hpp>

#include <mosaic/macros/exposure.hpp>

namespace Mosaic
{
    class Application;

    struct ApplicationResources;

    class MOSAIC_PUBLIC_EXPOSURE Window
    {
    public:
        WindowResources& GetResources();
        const WindowResources& GetResources() const;

    private:
        class Backend
        {
        private:
            struct Storage;

            Backend(WindowResources& windowResources, ApplicationResources& applicationResources);
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
            ApplicationResources& mApplicationResources;
        };

        Window(ApplicationResources& resources);
        ~Window();

        void Setup();
        void Update();

        Backend mBackend;

        WindowResources mWindowResources;
        WindowResources mWindowResourcesCopy;

        ApplicationResources& mApplicationResources;

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