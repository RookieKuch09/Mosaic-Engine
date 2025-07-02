#pragma once

#include <algorithm>
#include <cstdint>
#include <tuple>
#include <vector>

namespace Mosaic
{
    template <typename... Args>
    class MaterialDescriptor
    {
    };

    template <typename>
    struct IsMaterialDescriptorType : std::false_type
    {
    };

    template <typename... Args>
    struct IsMaterialDescriptorType<MaterialDescriptor<Args...>> : std::true_type
    {
    };

    enum class PipelineDepthTestState
    {
        Enabled,
        Disabled,
    };

    enum class PipelineDepthTestOperation
    {
        Never,
        Less,
        LessOrEqual,
        Equal,
        NotEqual,
        GreaterOrEqual,
        Greater,
        Always,
    };

    template <PipelineDepthTestState _State, PipelineDepthTestOperation _Operation>
    class PipelineDepthTestSettings
    {
    public:
        constexpr static PipelineDepthTestState State = _State;
        constexpr static PipelineDepthTestOperation Operation = _Operation;
    };

    template <typename>
    struct IsPipelineDepthTestSettingsType : std::false_type
    {
    };

    template <PipelineDepthTestState _State, PipelineDepthTestOperation _Operation>
    struct IsPipelineDepthTestSettingsType<PipelineDepthTestSettings<_State, _Operation>> : std::true_type
    {
    };

    enum class PipelineDrawMode
    {
        Fill,
        Wireframe,
        Point,
    };

    enum class PipelinePrimitive
    {
        Points,
        Lines,
        Triangles,
    };

    enum class PipelineBlendState
    {
        Enabled,
        Disabled,
    };

    enum class PipelineBlendOperation
    {
        None,
        Add,
        Subtract,
        ReverseSubtract,
        Minimum,
        Maximum,
    };

    enum class PipelineBlendFactor
    {
        None,
        Zero,
        One,
        SourceColour,
        SourceColourComplement,
        DestinationColour,
        DestinationColourComplement,
        SourceAlpha,
        SourceAlphaComplement,
        DestinationAlpha,
        DestinationAlphaComplement,
        ConstantColour,
        ConstantColourComplement,
        ConstantAlpha,
        ConstantAlphaComplement,
    };

    template <PipelineBlendState _BlendState,
              PipelineBlendOperation _ColourOperation,
              PipelineBlendFactor _SourceColourFactor,
              PipelineBlendFactor _DestinationColourFactor,
              PipelineBlendOperation _AlphaOperation,
              PipelineBlendFactor _SourceAlphaFactor,
              PipelineBlendFactor _DestinationAlphaFactor>
    class PipelineBlendSettings
    {
    public:
        constexpr static PipelineBlendState BlendState = _BlendState;
        constexpr static PipelineBlendOperation ColourOperation = _ColourOperation;
        constexpr static PipelineBlendFactor SourceColourFactor = _SourceColourFactor;
        constexpr static PipelineBlendFactor DestinationColourFactor = _DestinationColourFactor;
        constexpr static PipelineBlendOperation AlphaOperation = _AlphaOperation;
        constexpr static PipelineBlendFactor SourceAlphaFactor = _SourceAlphaFactor;
        constexpr static PipelineBlendFactor DestinationAlphaFactor = _DestinationAlphaFactor;
    };

    template <typename>
    struct IsPipelineBlendSettingsType : std::false_type
    {
    };

    template <PipelineBlendState _BlendState,
              PipelineBlendOperation _ColourOperation,
              PipelineBlendFactor _SourceColourFactor,
              PipelineBlendFactor _DestinationColourFactor,
              PipelineBlendOperation _AlphaOperation,
              PipelineBlendFactor _SourceAlphaFactor,
              PipelineBlendFactor _DestinationAlphaFactor>
    struct IsPipelineBlendSettingsType<PipelineBlendSettings<_BlendState, _ColourOperation, _SourceColourFactor, _DestinationColourFactor, _AlphaOperation, _SourceAlphaFactor, _DestinationAlphaFactor>> : std::true_type
    {
    };

    template <PipelinePrimitive _Primitive, PipelineDrawMode _DrawMode, typename _DepthTestSettings, typename _BlendSettings>
    requires IsPipelineDepthTestSettingsType<_DepthTestSettings>::value && IsPipelineBlendSettingsType<_BlendSettings>::value
    class PipelineFlags
    {
    public:
        static constexpr PipelinePrimitive Primitive = _Primitive;
        static constexpr PipelineDrawMode DrawMode = _DrawMode;

        using DepthTestSettings = _DepthTestSettings;
        using BlendSettings = _BlendSettings;
    };

    template <typename>
    struct IsPipelineFlagsType : std::false_type
    {
    };

    template <PipelinePrimitive _Primitive, PipelineDrawMode _DrawMode, typename _DepthTestSettings, typename _BlendSettings>
    requires IsPipelineDepthTestSettingsType<_DepthTestSettings>::value && IsPipelineBlendSettingsType<_BlendSettings>::value
    struct IsPipelineFlagsType<PipelineFlags<_Primitive, _DrawMode, _DepthTestSettings, _BlendSettings>> : std::true_type
    {
    };

    enum class ShaderType
    {
        Vertex,
        Pixel,
    };

    template <ShaderType... _ShaderTypes>
    class ShaderOrder
    {
    public:
        template <std::size_t N>
        static constexpr ShaderType GetShader = std::get<N>(std::tuple{{_ShaderTypes...}});

        template <ShaderType _ShaderType>
        static constexpr bool HasShader = ((_ShaderTypes == _ShaderType) || ...);
    };

    template <typename>
    struct IsShaderOrderType : std::false_type
    {
    };

    template <ShaderType... _ShaderTypes>
    struct IsShaderOrderType<ShaderOrder<_ShaderTypes...>> : std::true_type
    {
    };

    template <typename _MeshDescriptor, typename _MaterialDescriptor, typename _ShaderOrder, typename _Flags>
    concept IsValidPipelineDescriptor = IsMeshDescriptorType<_MeshDescriptor>::value &&
                                        IsMaterialDescriptorType<_MaterialDescriptor>::value &&
                                        IsShaderOrderType<_ShaderOrder>::value &&
                                        IsPipelineFlagsType<_Flags>::value;

    template <typename _MeshDescriptor, typename _MaterialDescriptor, typename _ShaderOrder, typename _Flags>
    requires IsValidPipelineDescriptor<_MeshDescriptor, _MaterialDescriptor, _ShaderOrder, _Flags>
    class PipelineDescriptor
    {
    public:
        using MeshDescriptor = _MeshDescriptor;
        using MaterialDescriptor = _MaterialDescriptor;
        using ShaderOrder = _ShaderOrder;
        using Flags = _Flags;
    };

    template <typename>
    struct IsPipelineDescriptorType : std::false_type
    {
    };

    template <typename _MeshDescriptor, typename _MaterialDescriptor, typename _ShaderOrder, typename _Flags>
    struct IsPipelineDescriptorType<PipelineDescriptor<_MeshDescriptor, _MaterialDescriptor, _ShaderOrder, _Flags>> : std::true_type
    {
    };

    template <typename Descriptor>
    class Mesh
    {
        // TODO: implement mesh
    };

    template <typename Descriptor>
    class Material
    {
        // TODO: implement material
    };

    template <typename Descriptor>
    class Pipeline
    {
        // TODO: implement pipeline
    };

    class MaterialHandle
    {
        std::uint32_t mID;
    };

    class PipelineHandle
    {
        std::uint32_t mID;
    };

    template <typename MeshDescriptor, typename MaterialDescriptor>
    class RenderComponent
    {
    public:
        MeshHandle Mesh;
        MaterialHandle Material;
    };

    template <typename PD>
    class RenderPass
    {
    private:
        using ComponentType = RenderComponent<typename PD::MeshDescriptor, typename PD::MaterialDescriptor>;

    public:
        RenderPass(const RenderPass&) = delete;
        RenderPass(RenderPass&&) noexcept = default;

        RenderPass& operator=(const RenderPass&) = delete;
        RenderPass& operator=(RenderPass&&) noexcept = default;

        void Enqueue(const ComponentType& renderComponent)
        {
            if (ComponentNotFound(renderComponent))
            {
                mRenderComponents.push_back(&renderComponent);
            }
            else
            {
                // TODO: warn about component already being in this render pass
            }
        }

    private:
        bool ComponentNotFound(const ComponentType& renderComponent) const
        {
            return std::find(mRenderComponents.begin(), mRenderComponents.end(), &renderComponent) == mRenderComponents.end();
        }

        std::vector<const ComponentType*> mRenderComponents;
    };

    class RenderPassHandle
    {
        std::uint32_t mID;
    };

    class Renderer
    {
    public:
        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&) noexcept = delete;

        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&) noexcept = delete;

    private:
    };
}