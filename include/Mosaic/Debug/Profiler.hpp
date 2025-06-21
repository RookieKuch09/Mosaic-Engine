#pragma once

namespace Mosaic
{
    class Profiler;

    class ScopedProfiler
    {
    public:
        ScopedProfiler(const ScopedProfiler& other) = delete;
        ScopedProfiler(ScopedProfiler&& other) noexcept = default;

        ScopedProfiler& operator=(const ScopedProfiler& other) = delete;
        ScopedProfiler& operator=(ScopedProfiler&& other) noexcept = default;

        ScopedProfiler(Profiler& profiler);
        ~ScopedProfiler();

    private:
    };

    class Profiler
    {
    };
}