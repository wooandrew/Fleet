// Fleet : engine/graphics/font.hpp (c) 2021 Andrew Woo

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

#ifndef FLEET_ENGINE_GRAPHICS_FONT
#define FLEET_ENGINE_GRAPHICS_FONT

// Include standard library
#include <string>
#include <vector>
#include <memory>
#include <map>

// Include dependencies
#include <GLM/glm/glm.hpp>
#include <FREETYPE/include/ft2build.h>
#include FT_FREETYPE_H

// Include Fleet libraries
#include "texture.hpp"

namespace Fleet::Core::Graphics {

    struct Character {

        unsigned char character;
        glm::vec2 size;
        glm::vec2 bearing;
        glm::vec2 advance;
        float tc_offset;

        std::vector<glm::vec2> TexCoords;
    };

    class Font : public Texture {

        /// Font objects & rendering

    public:

        Font();
        Font(const std::string& _FontName, const std::string& _FontPath, int _FontSize = 48);
        ~Font();

        int init(const std::string& _FontName, const std::string& _FontPath, int _FontSize = 48);

        // Getters
        const std::map<char, Character>& GetCharacters() const;
        const int GetSize() const;
        std::vector<glm::vec2> GetTexCoords(char _character);

    private:

        std::string FontName;
        std::string FontPath;

        int FontSize;

        std::map<char, Character> characters;
    };

    class FontLibrary {

        /// Font library manages a particular font

    public:

        FontLibrary(const std::string& _FontName, const std::string& _FontPath);

        void AddSize(int _size);    // Creates a bew font object of the desired size
        const std::shared_ptr<Font>& GetFont(int _size);

    private:

        std::string FontName;
        std::string FontPath;

        std::map<int, std::shared_ptr<Font>> fl;
    };
}

#endif // !FLEET_ENGINE_GRAPHICS_FONT
