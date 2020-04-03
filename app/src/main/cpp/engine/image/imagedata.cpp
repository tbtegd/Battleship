#include "imagedata.hpp"
#include <filesystem>
#include <fstream>
#include <utility>

#include "lodepng/lodepng.hpp"

namespace engine::image {
    ImageData::ImageData(int width, int height, const std::vector<unsigned char>& pixels) : m_width(width), m_height(height), m_pixels(pixels) {}
    ImageData::ImageData(int width, int height, std::vector<unsigned char> &&pixels) : m_width(width), m_height(height), m_pixels(std::move(pixels)) {}

    std::shared_ptr<ImageData> newImageData(std::string_view filename) {
        if (!std::filesystem::exists(filename)) {
            return nullptr;
        }

        auto size = std::filesystem::file_size(filename);
        std::vector<char> file_data(size);

        std::ifstream file(filename, std::ios::binary);
        file.read(file_data.data(), size);
        file.close();

        unsigned width, height;
        std::vector<unsigned char> pixels{};

        auto in = reinterpret_cast<unsigned char*>(file_data.data());

        LodePNGColorType colortype = LCT_RGBA;
        const int bitdepth = 8;

        unsigned char* buffer = nullptr;
        unsigned error = lodepng_decode_memory(&buffer, &width, &height, in, size, colortype, bitdepth);
        if(buffer && !error) {
            lodepng::State state;
            state.info_raw.colortype = colortype;
            state.info_raw.bitdepth = bitdepth;
            pixels.resize(lodepng_get_raw_size(width, height, &state.info_raw));
            std::memcpy(pixels.data(), buffer, pixels.size());
        } else {
            return nullptr;
        }
        free(buffer);

        return std::make_shared<ImageData>(ImageData(int(width), int(height), std::move(pixels)));
    }
}
