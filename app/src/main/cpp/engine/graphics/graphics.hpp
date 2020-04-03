#pragma once

#include "shader.hpp"

namespace engine::graphics {
    extern std::shared_ptr<Shader> newShader(ShaderSource vertex_src, ShaderSource fragment_src);
    extern void setActive(bool enable) noexcept;
    extern bool isActive() noexcept;
}