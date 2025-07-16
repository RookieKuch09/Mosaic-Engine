#pragma once

#if defined(MOSAIC_PLATFORM_UNIX)

#define MOSAIC_PUBLIC_EXPOSURE __attribute__((visibility("default")))

#define MOSAIC_PRIVATE_EXPOSURE __attribute__((visibility("hidden")))

#elif defined(MOSAIC_PLATFORM_WINDOWS)

#define MOSAIC_PRIVATE_EXPOSURE

#if defined(MOSAIC_BUILD_ENVIRONMENT)

#define MOSAIC_PUBLIC_EXPOSURE __declspec(dllexport)

#else

#define MOSAIC_PUBLIC_EXPOSURE __declspec(dllimport)

#endif

#else

#define MOSAIC_PUBLIC_EXPOSURE

#define MOSAIC_PRIVATE_EXPOSURE

#error Unsupported platform for Mosaic

#endif
