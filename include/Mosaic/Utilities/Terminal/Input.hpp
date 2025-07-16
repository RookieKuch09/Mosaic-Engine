#pragma once

#include <optional>

namespace Mosaic::Utilities::Terminal
{
    template <typename T>
    inline std::optional<T> Get();
}

#include <Mosaic/Inline/Utilities/Terminal/Input.inl>