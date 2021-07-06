// Fleet : engine/graphics/texture.cpp (c) 2021 Andrew Woo

/* Modified MIT License
 *
 * Copyright 2021 Andrew Woo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * Restrictions:
 >  The Software may not be sold unless significant, mechanics changing modifications are made by the seller, or unless the buyer
 >  understands an unmodified version of the Software is available elsewhere free of charge, and agrees to buy the Software given
 >  this knowledge.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

#include <ASWL/logger.hpp>

namespace Fleet::Core::Graphics {

    Texture::Texture(const glm::vec2& _dimensions) : dimensions(_dimensions) {

        InternalFormat = GL_RGBA8;
        DataFormat = GL_RGBA;

        glad_glCreateTextures(GL_TEXTURE_2D, 1, &TextureID);
        glad_glTextureStorage2D(TextureID, 1, InternalFormat, dimensions.x, dimensions.y);

        glad_glTextureParameteri(TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glad_glTextureParameteri(TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glad_glTextureParameteri(TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glad_glTextureParameteri(TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    Texture::Texture(const std::string& _path) : path(_path) {

        dimensions = glm::vec2();

        TextureID = 0;

        InternalFormat = 0;
        DataFormat = 0;

        int width = 0;
        int height = 0;
        int channels = 0;

        stbi_set_flip_vertically_on_load(true);
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

        if (data) {

            dimensions = glm::vec2(width, height);

            GLenum iFormat = 0;     // Temp Internal Format
            GLenum dFormat = 0;     // Temp Data Format

            if (channels == 4) {
                iFormat = GL_RGBA8;
                dFormat = GL_RGBA;
            }
            else if (channels == 3) {
                iFormat = GL_RGB8;
                dFormat = GL_RGB;
            }
            else
                ASWL::Logger::logger("T0001", "Error: Bad channel #", std::to_string(channels));

            InternalFormat = iFormat;
            DataFormat = dFormat;

            glad_glCreateTextures(GL_TEXTURE_2D, 1, &TextureID);
            glad_glTextureStorage2D(TextureID, 1, InternalFormat, dimensions.x, dimensions.y);

            glad_glTextureParameteri(TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glad_glTextureParameteri(TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glad_glTextureParameteri(TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glad_glTextureParameteri(TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glad_glTextureSubImage2D(TextureID, 0, 0, 0, dimensions.x, dimensions.y, DataFormat, GL_UNSIGNED_BYTE, data);
        }
        else
            ASWL::Logger::logger("T0002", "Error: Failed to load image -> !stbi_load() [", path, "].");

        stbi_image_free(data);
    }

    Texture::~Texture() {
        glad_glDeleteTextures(1, &TextureID);
    }

    int Texture::init(const std::string& _path) {

        path = _path;

        dimensions = glm::vec2();

        TextureID = 0;

        InternalFormat = 0;
        DataFormat = 0;

        int width = 0;
        int height = 0;
        int channels = 0;

        stbi_set_flip_vertically_on_load(true);
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

        if (data) {

            dimensions = glm::vec2(width, height);

            GLenum iFormat = 0;     // Temp Internal Format
            GLenum dFormat = 0;     // Temp Data Format

            if (channels == 4) {
                iFormat = GL_RGBA8;
                dFormat = GL_RGBA;
            }
            else if (channels == 3) {
                iFormat = GL_RGB8;
                dFormat = GL_RGB;
            }
            else
                ASWL::Logger::logger("T0001", "Error: Bad channel #", std::to_string(channels));

            InternalFormat = iFormat;
            DataFormat = dFormat;

            glad_glCreateTextures(GL_TEXTURE_2D, 1, &TextureID);
            glad_glTextureStorage2D(TextureID, 1, InternalFormat, dimensions.x, dimensions.y);

            glad_glTextureParameteri(TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glad_glTextureParameteri(TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glad_glTextureParameteri(TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glad_glTextureParameteri(TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glad_glTextureSubImage2D(TextureID, 0, 0, 0, dimensions.x, dimensions.y, DataFormat, GL_UNSIGNED_BYTE, data);
        }
        else
            ASWL::Logger::logger("T0002", "Error: Failed to load image -> !stbi_load() [", path, "].");

        stbi_image_free(data);

        return 0;
    }

    const glm::vec2& Texture::GetDimensions() const {
        return dimensions;
    }

    void Texture::SetData(void* _data, unsigned int _size) {

        unsigned int bpp = (DataFormat == GL_RGBA) ? 4 : 3;

        if (_size != dimensions.x * dimensions.y * bpp)
            ASWL::Logger::logger("T0003", "Error: Size does not match.");

        glad_glTextureSubImage2D(TextureID, 0, 0, 0, dimensions.x, dimensions.y, DataFormat, GL_UNSIGNED_BYTE, _data);
    }

    void Texture::Bind(unsigned int _slot) const {
        glad_glBindTextureUnit(_slot, TextureID);
    }

    const unsigned int Texture::GetTextureID() const {
        return TextureID;
    }

    bool Texture::operator==(const Texture& other) {
        return TextureID == other.TextureID;
    }
}
