// Project Boomerang : engine/graphics/font.hpp (c) 2020-2021 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

/* Modified MIT License
 *
 * Copyright 2020-2021 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan
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
#include <unordered_map>

// Include dependencies
#include <GLM/glm/glm.hpp>
#include <FREETYPE/include/ft2build.h>
#include FT_FREETYPE_H

namespace Boomerang::Core::Graphics {

    // TODO: Make Font inherit from Texture
    class Font {

        /// Font objects & rendering

    public:

        Font();
        ~Font();

        int init(const std::string& _FontName, const std::string& _FontPath, int _FontSize = 48);
        void Bind(unsigned int _slot = 0);

        struct GlyphData {

            glm::ivec2 size;
            glm::ivec2 advance;
            glm::ivec2 bearing;

            float tx;                   // x offset in atlas of the glyph
        };

        // Getters
        std::unordered_map<char, GlyphData> GetGlyphData() const;
        const glm::vec2& GetAtlasDimensions() const;
        const int GetSize() const;

    private:

        std::string FontName;
        std::string FontPath;

        int FontSize;
        glm::vec2 AtlasDimensions;

        unsigned int TextureID;     // Atlas ID *** should be inherited from Boomerang::Graphics::Texture in the future.

        std::unordered_map<char, GlyphData> umGlyphData;
    };
}

#endif // !BOOMERANG_ENGINE_GRAPHICS_FONT
