// Fleet : engine/graphics/texture.hpp (c) 2021 Andrew Woo

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

#pragma once

#ifndef FLEET_ENGINE_GRAPHICS_TEXTURE
#define FLEET_ENGINE_GRAPHICS_TEXTURE

// Include standard library
#include <string>

// Include dependencies
#include <glad/glad.h>
#include <GLM/glm/glm.hpp>

namespace Fleet::Core::Graphics {

    class Texture {

        /// Basic texture loader & mapper

    public:

        Texture() = default;
        Texture(const glm::vec2& _dimensions);
        Texture(const std::string& _path);
        ~Texture();

        virtual int init(const std::string& _path);

        const glm::vec2& GetDimensions() const;

        void SetData(void* _data, unsigned int _size);
        void Bind(unsigned int _slot = 1) const;

        const unsigned int GetTextureID() const;

        bool operator== (const Texture& other);

    protected:

        std::string path;
        glm::vec2 dimensions;

        unsigned int TextureID;

        GLenum InternalFormat;
        GLenum DataFormat;
    };
}

#endif // !FLEET_ENGINE_GRAPHICS_TEXTURE
