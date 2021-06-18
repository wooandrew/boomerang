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
#include <map>
#include <fstream>
//#include <algorithm>

// Include dependencies
#include <glad/glad.h>
#include <ASWL/logger.hpp>

namespace Boomerang::Core::Graphics {

    Font::Font() {
        FontName = "null";
        FontPath = "null";
        FontSize = 48;
        dimensions = { 0, 0 };
    }
    Font::Font(const std::string& _FontName, const std::string& _FontPath, int _FontSize) {
        init(_FontName, _FontPath, _FontSize);
    }
    Font::~Font() {

    };

    int Font::init(const std::string& _FontName, const std::string& _FontPath, int _FontSize) {

        FontName = _FontName;
        FontPath = _FontPath;
        FontSize = _FontSize;
        dimensions = { 0, 0 };
        
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

        for (unsigned char c = 32; c < 128; c++) {

            // Load glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                ASWL::Logger::logger("F0002", "Error: Failed to load glyph [", std::to_string(c), "].");
                continue;
            }

            // Calculate atlas dimensions
            dimensions.x += face->glyph->bitmap.width;
            dimensions.y = std::max(dimensions.y, static_cast<float>(face->glyph->bitmap.rows));
        }

        // Generate texture
        glad_glGenTextures(1, &TextureID);
        glad_glBindTexture(GL_TEXTURE_2D, TextureID);
        glad_glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, dimensions.x, dimensions.y, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

        // Set texture options
        glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glad_glBindTexture(GL_TEXTURE_2D, TextureID);

        int x = 0;
        for (unsigned char c = 32; c < 128; c++) {

            // Load glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))      // No need to log error here, if error occurs, it will 
                continue;                                   // already have been logged by the previous loop.

            // Sub in glyph data
            glad_glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

            characters[c] = {
                c,
                { face->glyph->bitmap.width, face->glyph->bitmap.rows },
                { face->glyph->bitmap_left, face->glyph->bitmap_top },
                { face->glyph->advance.x, face->glyph->advance.y },
                static_cast<float>(x) / dimensions.x
            };

            x += face->glyph->bitmap.width;
        }

        FT_Done_Face(face);
        FT_Done_FreeType(library);

        return 0;
    }

    // Getters
    const std::map<char, Character>& Font::GetCharacters() const {
        return characters;
    }

    const int Font::GetSize() const {
        return FontSize;
    }

    // Font Library
    FontLibrary::FontLibrary(const std::string& _FontName, const std::string& _FontPath) {
        
        FontName = _FontName;
        FontPath = _FontPath;

        fl.insert({ 48, std::make_shared<Font>(FontName, FontPath) });
    }

    void FontLibrary::AddSize(int _size) {
        fl.insert({ _size, std::make_shared<Font>(FontName, FontPath) });
    }

    const std::shared_ptr<Font>& FontLibrary::GetFont(int _size) {

        if (fl.count(_size) == 0)
            AddSize(_size);

        return fl[_size];
    }
}
