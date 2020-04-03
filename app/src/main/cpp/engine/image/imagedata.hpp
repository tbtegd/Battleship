#pragma once

#include <string_view>
#include <memory>
#include <vector>

namespace engine::image {
    enum class Color : unsigned {};

    struct ImageData {
        friend std::shared_ptr<ImageData> newImageData(std::string_view filename);

        inline auto getDimensions() const noexcept {
            return std::pair(m_width, m_height);
        }

        inline auto getWidth() const noexcept {
            return m_width;
        }

        inline auto getHeight() const noexcept {
            return m_height;
        }

        inline auto pixels() const noexcept {
            return m_pixels.data();
        }
    private:
        ImageData(int width, int height, const std::vector<unsigned char>& pixels);
        ImageData(int width, int height, std::vector<unsigned char>&& pixels);

        int m_width, m_height;
        std::vector<unsigned char> m_pixels;
    };

    std::shared_ptr<ImageData> newImageData(std::string_view filename);
}