#include "../../frontend/window.hpp"

struct GLFWwindow;

namespace Mosaic::Backend::Windows
{
    class GLFWWindow : public Frontend::WindowBackend
    {
    public:
        GLFWWindow(Frontend::GlobalContext& context, glm::uvec2& size, glm::uvec2& position, std::string& title);

        void Start() override;
        void Update() override;
        void Stop() override;

        void SetPosition() override;
        void SetSize() override;
        void SetTitle() override;

        void GetPosition() override;
        void GetSize() override;
        void GetTitle() override;

    private:
        GLFWwindow* mHandle;
    };
}