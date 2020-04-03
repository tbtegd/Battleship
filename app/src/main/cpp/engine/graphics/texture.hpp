#pragma once

#include <GLES3/gl3.h>
#include <string_view>

namespace engine::graphics {
    struct Texture {
        Texture(const Texture& other) = delete;
        Texture& operator=(const Texture& other) = delete;
        Texture(Texture&& other) noexcept : m_texture(other.m_texture) {
            other.m_texture = 0;
        }
        Texture& operator=(Texture&& shader) noexcept {
            m_texture = shader.m_texture;
            shader.m_texture = 0;
            return *this;
        }
        inline void release() {
            if (m_texture != 0) {
                glDeleteTextures(1, &m_texture);
                m_texture = 0;
            }
        }
        inline operator GLuint() const noexcept {
            return m_texture;
        }

    protected:
        Texture();
        GLuint m_texture;
    };

    struct Image : Texture {
        friend Image newImage(std::string_view path);
    private:
        Image(std::string_view path);
    };

    Image newImage(std::string_view path);
}