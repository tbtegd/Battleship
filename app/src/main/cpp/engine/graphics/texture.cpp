#include "texture.hpp"
#include <filesystem>
#include <fstream>

engine::graphics::Texture::Texture() {
}

engine::graphics::Image::Image(std::string_view path) : Texture() {
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

engine::graphics::Image engine::graphics::newImage(std::string_view path) {
    return Image(path);
}