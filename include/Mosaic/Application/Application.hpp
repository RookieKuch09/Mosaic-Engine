#pragma once

#include <Mosaic/Macros/Symbols.hpp>

#include <string>
#include <vector>

namespace Mosaic::Application
{
    class MOSAIC_PUBLIC_SYMBOL Application
    {
    public:
        Application(const std::vector<std::string>& arguments);
        virtual ~Application() = default;

        Application(const Application&) = delete;
        Application(Application&&) noexcept = delete;

        Application& operator=(const Application&) = delete;
        Application& operator=(Application&&) noexcept = delete;

        virtual void OnStart() = 0;
        virtual void OnUpdate() = 0;
        virtual void OnClose() = 0;

        [[nodiscard]] virtual bool ShouldUpdate() = 0;

    private:
        const std::vector<std::string> mArguments;

    protected:
        [[nodiscard]] const std::vector<std::string>& GetLaunchArguments() const;
    };
}