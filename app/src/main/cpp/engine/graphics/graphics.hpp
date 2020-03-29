#pragma once

#include "shader.hpp"

namespace engine::graphics {
    extern Shader newShader(ShaderSource vertex_src, ShaderSource fragment_src);
}