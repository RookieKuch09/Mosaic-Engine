#pragma once

namespace Mosaic
{
    enum class MeshTopography
    {
        Points,
        Lines,
        Triangles,
    };

    template <MeshTopography _Topography>
    class MeshFlags
    {
    public:
        static constexpr MeshTopography Topography = _Topography;
    };

    // TODO: ensure vertex count is supported by topography
    // TODO: cache useful info (vertex length, etc.)
    template <typename _AttributePack, typename _Flags>
    class MeshDescriptor
    {
    public:
        using AttributePack = _AttributePack;
        using Flags = _Flags;
    };
}