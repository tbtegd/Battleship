#include "graphics.hpp"
#include "shader.hpp"

namespace {
    std::atomic_bool active;
}

namespace engine::graphics {
    std::shared_ptr<Shader> newShader(ShaderSource vertex_src, ShaderSource fragment_src) {
        return std::make_shared<Shader>(std::move(Shader(vertex_src, fragment_src)));
    }

    void setActive(bool enable) noexcept {
        active.store(enable);
    }

    bool isActive() noexcept {
        return active.load();
    }
}