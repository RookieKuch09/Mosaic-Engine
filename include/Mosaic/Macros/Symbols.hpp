#pragma once

#if defined(MOSAIC_PLATFORM_UNIX)

#define MOSAIC_PUBLIC_SYMBOL __attribute__((visibility("default")))

#define MOSAIC_PRIVATE_SYMBOL __attribute__((visibility("hidden")))

#elif defined(MOSAIC_PLATFORM_WINDOWS)

#define MOSAIC_PRIVATE_SYMBOL

#if defined(MOSAIC_BUILD_ENVIRONMENT)

#define MOSAIC_PUBLIC_SYMBOL __declspec(dllexport)

#else

#define MOSAIC_PUBLIC_SYMBOL __declspec(dllimport)

#endif

#else

#define MOSAIC_PUBLIC_SYMBOL

#define MOSAIC_PRIVATE_SYMBOL

#error Unsupported platform for Mosaic

#endif
