#pragma once

#include <mosaic/api/exposure.hpp>

#include <type_traits>

namespace Mosaic
{
    struct Resources;

    class MOSAIC_PUBLIC_EXPOSURE Application
    {
    public:
        virtual void Setup() = 0;

    protected:
        Application(Resources& resources);

        Resources& ApplicationResources;

        template <typename T> requires std::is_base_of<Application, T>::value
        friend class Instance;
    };
}