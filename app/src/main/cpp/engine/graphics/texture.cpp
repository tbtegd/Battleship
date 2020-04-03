#include "texture.hpp"

#include "image/imagedata.hpp"

#include <filesystem>
#include <fstream>

namespace engine::graphics {
    Texture::Texture() {
        glGenTextures(1, &m_texture);
    }

    Texture::~Texture() {
        if (m_texture != 0) {
            glDeleteTextures(1, &m_texture);
            m_texture = 0;
        }
    }

    Image::Image(const std::shared_ptr<image::ImageData>& data) : Texture(), m_width(data->getWidth()), m_height(data->getHeight()) {
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data->getWidth(), data->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, data->pixels());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    std::shared_ptr<Image> newImage(std::string_view filename) {
        return std::shared_ptr<Image>::make_shared(Image(engine::image::newImageData(filename)));
    }
}