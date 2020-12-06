// Project Boomerang : engine/graphics/font.cpp (c) 2020 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

#include "font.hpp"

// Include standard library
#include <iostream>
#include <fstream>

// Include dependencies
#include <GLAD/glad.h>

// Include boomerang libraries
#include "../../misc/logger.hpp"

namespace Boomerang::Core::Graphics {

    void Character::Bind(unsigned int _slot) const {
        glad_glBindTextureUnit(_slot, TextureID);
    }

    Font::Font() {
        FontSize = 172;
    }
    Font::~Font() {

    };

    int Font::init(std::string _FontName, std::string _FontPath) {

        FontName = _FontName;
        FontPath = _FontPath;
        
        FT_Library library;
        if (FT_Init_FreeType(&library)) {
            Boomerang::Misc::Logger::logger("F0000", "Error: Failed to initialize FreeType2.");
            return 1;
        }

        FT_Face face;
        if (FT_New_Face(library, _FontPath.c_str(), 0, &face)) {
            Boomerang::Misc::Logger::logger<std::string, std::string>("F0001", "Error: Failed to load font face [", _FontPath, "].");
            return 2;
        }

        // Set Font size
        FT_Set_Pixel_Sizes(face, 0, FontSize);

        // Disable byte-alignment restriction
        glad_glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for (unsigned char c = 0; c < 128; c++) {

            // Load glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                Boomerang::Misc::Logger::logger<std::string, std::string>("F0002", "Error: Failed to load glyph [", std::to_string(c), "].");
                continue;
            }

            // Generate texture
            unsigned int texture;
            glad_glCreateTextures(GL_TEXTURE_2D, 1, &texture);
            glad_glTextureStorage2D(texture, 1, GL_R8, face->glyph->bitmap.width, face->glyph->bitmap.rows);
            glad_glTextureSubImage2D(texture, 0, 0, 0, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
            //glTexImage2D(texture, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

            // Set texture options
            glad_glTexParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glad_glTexParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glad_glTexParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glad_glTexParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };

            characters.insert(std::pair<char, Character>(c, character));
        }

        FT_Done_Face(face);
        FT_Done_FreeType(library);

        return 0;
    }

    // Getters
    std::map<char, Character> Font::GetCharacters() const {
        return characters;
    }
}
