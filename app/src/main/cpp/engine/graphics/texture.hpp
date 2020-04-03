#pragma once

#include <GLES3/gl3.h>
#include <string_view>
#include <memory>

namespace engine::image {
    struct ImageData;
}

namespace engine::graphics {
    struct Texture {
    protected:
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
        ~Texture();

        inline operator GLuint() const noexcept {
            return m_texture;
        }

        Texture();
        GLuint m_texture;
    };

    struct Image : private Texture {
        using Texture::operator GLuint;
        friend std::shared_ptr<Image> newImage(std::string_view filename);
    private:
        Image(const std::shared_ptr<image::ImageData>& data);
    };

    extern std::shared_ptr<Image> newImage(std::string_view filename);
}