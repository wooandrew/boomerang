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

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "STB/stb_image_write.h" /* http://nothings.org/stb/stb_image_write.h */

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

        unsigned char* bin = nullptr;

        std::basic_ifstream<unsigned char> font(_FontPath, std::ios::in | std::ios::binary);
        if (font) {

            font.seekg(0, std::ios::end);
            long size = font.tellg();
            font.seekg(0, std::ios::beg);

            bin = new unsigned char[size];      // This memory is never handled!

            font.read(&bin[0], size);
            font.close();
        }

        if (!stbtt_InitFont(&info, bin, 0)) {
            Boomerang::Misc::Logger::logger<std::string, std::string>("F0000", "Fatal Error: Failed to initialize font [", _FontPath, "].");
            return 1;
        }

        return 0;
    }

    int Font::GetStringLength(std::shared_ptr<Font> _font, std::string _string) {

        int length = 0;
        float scale = stbtt_ScaleForPixelHeight(&_font->GetFontInfo(), static_cast<float>(_font->GetFontSize()));

        for (std::string::iterator i = _string.begin(); i != _string.end(); i++) {

            int advance;
            int lsb;

            stbtt_GetCodepointHMetrics(&_font->GetFontInfo(), *i, &advance, &lsb);

            length += advance;
        }

        return length * scale;
    }

    unsigned char* Font::MakeTextureData(std::shared_ptr<Font> _font, std::string _string) {

        int ascent, descent, lineGap;
        stbtt_GetFontVMetrics(&_font->GetFontInfo(), &ascent, &descent, &lineGap);
        float scale = stbtt_ScaleForPixelHeight(&_font->GetFontInfo(), _font->GetFontSize());

        int b_w = GetStringLength(_font, _string);      // Bitmap width
        int b_h = ascent;                               // Bitmap height

        ascent = static_cast<int>(ascent * scale);
        descent = static_cast<int>(descent * scale);

        unsigned char* ret = new unsigned char[b_w * b_h];
        
        int x = 0;

        for (std::string::iterator i = _string.begin(); i != _string.end(); i++)
        {
            /* how wide is this character */
            int ax;
            int lsb;
            stbtt_GetCodepointHMetrics(&_font->GetFontInfo(), *i, &ax, &lsb);

            /* get bounding box for character (may be offset to account for chars that dip above or below the line */
            int c_x1, c_y1, c_x2, c_y2;
            stbtt_GetCodepointBitmapBox(&_font->GetFontInfo(), *i, scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

            /* compute y (different characters have different heights */
            int y = ascent + c_y1;

            /* render character (stride and offset is important here) */
            int byteOffset = x + roundf(lsb * scale) + (y * b_w);
            stbtt_MakeCodepointBitmap(&_font->GetFontInfo(), ret + byteOffset, c_x2 - c_x1, c_y2 - c_y1, b_w, scale, scale, *i);

            /* advance x */
            x += roundf(ax * scale);

            /* add kerning */
            int kern;
            kern = stbtt_GetCodepointKernAdvance(&_font->GetFontInfo(), *i, *i+1);
            x += roundf(kern * scale);
        }

        /* save out a 1 channel image */
        stbi_write_png("out.png", b_w, b_h, 1, ret, b_w);

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
