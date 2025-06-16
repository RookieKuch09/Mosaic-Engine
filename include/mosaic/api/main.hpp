#pragma once

#if !defined(MOSAIC_BUILD_ENVIRONMENT)

#if defined(MOSAIC_PLATFORM_UNIX)

#define MOSAIC_DEFINE_MAIN()        \
                                    \
    int main(int argc, char** argv) \
    {                               \
        return 0;                   \
    }

#else

#define MOSAIC_DEFINE_MAIN()

#error Unsupported platform for Mosaic

#endif

#endif