#pragma once

namespace Mosaic::Types::Constraints
{
#if !defined(MOSAIC_COMPILER_UNSUPPORTED)

    inline constexpr bool IsCompilerSupported = true;

#else

    inline constexpr bool IsCompilerSupported = false;

#endif
}