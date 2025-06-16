#pragma once

#if defined(MOSAIC_PLATFORM_UNIX)

#define MOSAIC_PUBLIC_EXPOSURE __attribute__((visibility("default")))

#define MOSAIC_PRIVATE_EXPOSURE __attribute__((visibility("hidden")))

#else

#define MOSAIC_PUBLIC_EXPOSURE

#define MOSAIC_PRIVATE_EXPOSURE

#error Unsupported platform for Mosaic

#endif