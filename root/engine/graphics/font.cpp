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

#include <stdlib.h>

// Include dependencies
#define STB_TRUETYPE_IMPLEMENTATION
#include <STB/stb_truetype.h>

// Include boomerang libraries
#include "../../misc/logger.hpp"

namespace Boomerang::Core::Graphics {

    Font::Font() {
        FontSize = 48;
        info = { };
    }
    Font::~Font() {

    };

    int Font::init(std::string _FontName, std::string _FontPath) {

        FontName = _FontName;
        FontPath = _FontPath;

        /*
        std::string bin;

        std::ifstream font(_FontPath, std::ios::in | std::ios::binary);
        if (font) {

            font.seekg(0, std::ios::end);
            bin.resize(font.tellg());

            font.seekg(0, std::ios::beg);
            font.read(&bin[0], bin.size());
            font.close();
        }*/

        /* load font file */
        long size;
        unsigned char* fontBuffer;

        FILE* fontFile = fopen("assets/fonts/raleway.ttf", "rb");
        fseek(fontFile, 0, SEEK_END);
        size = ftell(fontFile); /* how long is the file ? */
        fseek(fontFile, 0, SEEK_SET); /* reset */

        fontBuffer = (unsigned char*) malloc(size);

        fread(fontBuffer, size, 1, fontFile);
        fclose(fontFile);

        if (!stbtt_InitFont(&info, fontBuffer, 0)) {
            Boomerang::Misc::Logger::logger<std::string, std::string>("F0000", "Fatal Error: Failed to initialize font [", _FontPath, "].");
            return 1;
        }

        return 0;
    }

    int Font::GetStringLength(std::shared_ptr<Font> _font, std::string _string) {

        int length = 0;
        stbtt_ScaleForPixelHeight(&_font->GetFontInfo(), static_cast<float>(_font->GetFontSize()));

        for (std::string::iterator i = _string.begin(); i != _string.end(); i++) {

            int advance;
            int lsb;

            stbtt_GetCodepointHMetrics(&_font->GetFontInfo(), *i, &advance, &lsb);

            length += advance;
        }

        return length;
    }

    unsigned char* Font::GetTextureData() {

        unsigned char* ret = (unsigned char*)"A";
        
        return ret;
    }

    void Font::SetFontSize(int _FontSize) {
        FontSize = _FontSize;
    }

    // Getters
    const int Font::GetFontSize() const {
        return FontSize;
    }
    const stbtt_fontinfo& Font::GetFontInfo() const {
        return info;
    }
}
