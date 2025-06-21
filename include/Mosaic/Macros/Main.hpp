#pragma once

#if !defined(MOSAIC_BUILD_ENVIRONMENT)

#if defined(MOSAIC_PLATFORM_UNIX)

#define MOSAIC_DEFINE_MAIN(ApplicationType)         \
                                                    \
    auto main(int argc, char** argv) -> int         \
    {                                               \
        Mosaic::Instance<ApplicationType> instance; \
                                                    \
        instance.Setup();                           \
                                                    \
        return instance.Run();                      \
    }

#else

#define MOSAIC_DEFINE_MAIN(ApplicationType)

#error Unsupported platform for Mosaic

#endif

#endif