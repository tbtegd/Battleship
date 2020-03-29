#include "graphics.hpp"

namespace engine::graphics {
    Shader newShader(ShaderSource vertex_src, ShaderSource fragment_src) {
        return Shader(vertex_src, fragment_src);
    }
}