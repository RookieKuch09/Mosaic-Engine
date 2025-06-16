#pragma once

#if defined(MOSAIC_PLATFORM_UNIX)

#include <unistd.h>

#define MOSAIC_GET_PID() (static_cast<std::uint32_t>(getpid()))

#else

#error Unsupported platform for Mosaic

#endif