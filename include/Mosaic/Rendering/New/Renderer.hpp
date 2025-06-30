#pragma once

#include <cstdint>
#include <tuple>
#include <vector>

namespace Mosaic
{
    enum class MeshPrimitive
    {
        Triangle,
    };

    template <PipelinePrimitive _Primitive>
    class MeshFlags
    {
    public:
        static constexpr PipelinePrimitive Primitive = _Primitive;
    };

    template <auto _Semantic, typename _Type, std::size_t _Count>
    class MeshAttribute
    {
    public:
        using SemanticType = decltype(_Semantic);
        using ElementType = _Type;

        static constexpr std::size_t ElementCount = _Count;
        static constexpr SemanticType SemanticValue = _Semantic;
    };

    template <typename T>
    struct IsMeshAttributeType : std::false_type
    {
    };

    template <auto _Semantic, typename _Type, std::size_t _Count>
    struct IsMeshAttributeType<MeshAttribute<_Semantic, _Type, _Count>> : std::true_type
    {
    };

    template <typename T>
    struct GetSemantic
    {
        static constexpr auto Value = T::SemanticValue;
        using Type = typename T::SemanticType;
    };

    template <auto...>
    struct AreAttributeSemanticsUnique : std::true_type
    {
    };

    template <auto First, auto... Rest>
    struct AreAttributeSemanticsUnique<First, Rest...> : std::bool_constant<((First != Rest) && ...) && AreAttributeSemanticsUnique<Rest...>::value>
    {
    };

    template <typename... Attributes>
    struct AreAllAttributeSemanticsValid
    {
        static constexpr bool value = AreAttributeSemanticsUnique<GetSemantic<Attributes>::Value...>::value;
    };

    template <typename...>
    struct AreAttributeSemanticsSameType : std::true_type
    {
    };

    template <typename First, typename... Rest>
    struct AreAttributeSemanticsSameType<First, Rest...> : std::bool_constant<((std::same_as<First, Rest>) && ...) && AreAttributeSemanticsSameType<Rest...>::value>
    {
    };

    template <typename... Attributes>
    struct AreAllAttributeSemanticsSameType
    {
        static constexpr bool value = AreAttributeSemanticsSameType<typename GetSemantic<Attributes>::Type...>::value;
    };

    template <typename... _Attributes>
    concept MeshAttributesAreValid = AreAllAttributeSemanticsValid<_Attributes...>::value &&
                                     AreAllAttributeSemanticsSameType<_Attributes...>::value &&
                                     (IsMeshAttributeType<_Attributes>::value && ...);

    template <typename... _Attributes>
    requires MeshAttributesAreValid<_Attributes...>
    class MeshAttributePack
    {
    public:
        template <std::size_t N>
        using GetAttributeByIndex = typename std::tuple_element<N, std::tuple<_Attributes...>>::type;

        template <auto _Semantic>
        using GetAttributeBySemantic = int; // TODO: retrieve mesh attribute by semantic

        template <auto _Semantic>
        static constexpr bool HasAttribute = ((GetAttributeSemantic<_Attributes>() == _Semantic) || ...);

    private:
        template <typename Attribute>
        static constexpr auto GetAttributeSemantic()
        {
            return Attribute::SemanticValue;
        }
    };

    template <typename _AttributePack, typename _Flags>
    class MeshDescriptor
    {
    public:
        using AttributePack = _AttributePack;
        using Flags = _Flags;
    };

    template <typename T>
    struct IsMeshDescriptorType : std::false_type
    {
    };

    template <typename... Args>
    struct IsMeshDescriptorType<MeshDescriptor<Args...>> : std::true_type
    {
    };

    template <typename... Args>
    class MaterialDescriptor
    {
    };

    template <typename T>
    struct IsMaterialDescriptorType : std::false_type
    {
    };

    template <typename... Args>
    struct IsMaterialDescriptorType<MaterialDescriptor<Args...>> : std::true_type
    {
    };

    enum class PipelineDepthTestMode
    {
        Disabled,
        Less,
        Equal,
        Greater,
    };

    enum class PipelinePrimitiveMode
    {
        Standard,
        Wireframe,
    };

    enum class PipelinePrimitive
    {
        Triangle,
    };

    enum class PipelineTransparencyMode
    {
        Disabled,

    };

    template <PipelinePrimitive _Primitive, PipelineDepthTest _DepthTest, PipelineDrawMode _DrawMode>
    class PipelineFlags
    {
    public:
        static constexpr PipelinePrimitive Primitive = _Primitive;
        static constexpr PipelineDepthTest DepthTest = _DepthTest;
        static constexpr PipelineDrawMode DrawMode = _DrawMode;
    };

    template <typename T>
    struct IsPipelineFlagsType : std::false_type
    {
    };

    template <PipelinePrimitive _Primitive, PipelineDepthTest _DepthTest, PipelineDrawMode _DrawMode>
    struct IsPipelineFlagsType<PipelineFlags<_Primitive, _DepthTest, _DrawMode>> : std::true_type
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

    template <typename T>
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

    template <typename T>
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
    };

    template <typename Descriptor>
    class Material
    {
    };

    template <typename Descriptor>
    class Pipeline
    {
    };

    class MeshHandle
    {
        std::uint32_t mID;
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
                mRenderComponents.push_back(renderComponent);
            }
            else
            {
                // TODO: warn about component already being in this render pass
            }
        }

    private:
        bool ComponentNotFound(const ComponentType& renderComponent) const
        {
            auto it = std::find(mRenderComponents.begin(), mRenderComponents.end(), &renderComponent);

            return it == mRenderComponents.end();
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