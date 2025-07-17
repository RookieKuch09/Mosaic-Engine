#include <Mosaic/Application/Application.hpp>

namespace Mosaic::Application
{
    Application::Application(const std::vector<std::string>& arguments)
        : mArguments(arguments)
    {
    }

    const std::vector<std::string>& Application::GetLaunchArguments() const
    {
        return mArguments;
    }
}