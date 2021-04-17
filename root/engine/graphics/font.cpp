// Project Boomerang : engine/graphics/font.cpp (c) 2020-2021 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

#include "font.hpp"

// Include standard library
#include <iostream>
#include <algorithm>
#include <fstream>

// Include dependencies
#include <glad/glad.h>
#include <ASWL/logger.hpp>

namespace Boomerang::Core::Graphics {

    Font::Font() {
        FontSize = 48;
        AtlasDimensions = glm::vec2(0);
        TextureID = 0;
    }
    Font::~Font() {

    };

    int Font::init(const std::string& _FontName, const std::string& _FontPath, int _FontSize) {

        FontName = _FontName;
        FontPath = _FontPath;
        FontSize = _FontSize;
        
        FT_Library library;
        if (FT_Init_FreeType(&library)) {
            ASWL::Logger::logger("F0000", "Error: Failed to initialize FreeType2.");
            return 1;
        }

        FT_Face face;
        if (FT_New_Face(library, _FontPath.c_str(), 0, &face)) {
            ASWL::Logger::logger("F0001", "Error: Failed to load font face [", _FontPath, "].");
            return 2;
        }

        // Set Font size
        FT_Set_Pixel_Sizes(face, 0, FontSize);

        // Disable byte-alignment restriction
        glad_glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // Initialize temporary glyph container
        FT_GlyphSlot g = face->glyph;

        // Initialize Font atlas dimensions
        unsigned int w = 0;         // width
        unsigned int h = 0;         // height

        // Calculate Font atlas dimensions
        for (unsigned char c = 32; c < 128; c++) {

            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                ASWL::Logger::logger("F0002", "Error: Failed to load glyph [", std::to_string(c), "].");
                continue;
            }

            w += g->bitmap.width;
            h = std::max(h, g->bitmap.rows);
        }

        AtlasDimensions = { w, h };

        // Generate empty atlas
        unsigned int texture;
        glad_glGenTextures(1, &texture);
        glad_glBindTexture(GL_TEXTURE_2D, texture);
        glad_glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

        // Set texture options
        glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        for (int c = 32, x = 0; c < 127; c++) {

            // No need to log load failure here; if it 
            // fails, it should already have been logged
            // when calculating atlas dimensions.
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                continue;

            glad_glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

            GlyphData gd = {

                { g->bitmap.width, g->bitmap.rows },
                { g->advance.x >> 6, g->advance.y >> 6  },
                { g->bitmap_left, g->bitmap_top, },

                static_cast<float>(x / w)
            };

            umGlyphData.insert({ c, gd });

            x += g->bitmap.width;
        }

        TextureID = texture;

        FT_Done_Face(face);
        FT_Done_FreeType(library);

        return 0;
    }

    void Font::Bind(unsigned int _slot) {
        glad_glBindTextureUnit(_slot, TextureID);
    }

    // Getters
    std::unordered_map<char, Font::GlyphData> Font::GetGlyphData() const {
        return umGlyphData;
    }

    const glm::vec2& Font::GetAtlasDimensions() const {
        return AtlasDimensions;
    }

    const int Font::GetSize() const {
        return FontSize;
    }
}
