#pragma once

#include "contexts.hpp"

namespace Mosaic::Frontend
{
    class Instance
    {
    public:
        Instance(const std::string& configPath);
        virtual ~Instance() = default;

        static std::unique_ptr<Instance> Create();

        virtual void Setup() = 0;

        std::uint32_t Run();

    protected:
        template <typename T>
        T& CreateResource(const std::string& id);

        GlobalContext GlobalContext;
    };
}