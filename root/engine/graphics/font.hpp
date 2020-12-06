// Project Boomerang : engine/graphics/font.hpp (c) 2020 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

/* Modified MIT License
 *
 * Copyright 2020 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan
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

#ifndef BOOMERANG_ENGINE_GRAPHICS_FONT
#define BOOMERANG_ENGINE_GRAPHICS_FONT

// Include standard library
#include <string>
#include <map>

// Include dependencies
#include <GLM/glm/glm.hpp>
#include <FREETYPE/include/ft2build.h>
#include FT_FREETYPE_H

namespace Boomerang::Core::Graphics {

    struct Character {

        unsigned int TextureID;
        glm::ivec2 size;
        glm::ivec2 bearing;
        signed long advance;

        void Bind(unsigned int slot = 0) const;
    };

    class Font {

        /// Font objects & rendering

    public:

        Font();
        ~Font();

        int init(std::string _FontName, std::string _FontPath);

        // Getters
        std::map<char, Character> GetCharacters() const;

    private:

        std::string FontName;
        std::string FontPath;

        int FontSize;

        std::map<char, Character> characters;
    };
}

#endif // !BOOMERANG_ENGINE_GRAPHICS_FONT
