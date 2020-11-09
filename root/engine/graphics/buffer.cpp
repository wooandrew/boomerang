// Project Boomerang : engine/graphics/buffer.cpp (c) 2020 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

#include "buffer.hpp"

namespace Boomerang::Core::Graphics {

    BufferLayout::BufferLayout() {
        stride = 0;
    }
    BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements) : elements(elements) {
        CalculateOffsetsAndStride();
    }

    std::vector<BufferElement>::iterator BufferLayout::begin() {
        return elements.begin();
    }
    std::vector<BufferElement>::iterator BufferLayout::end() {
        return elements.end();
    }
    std::vector<BufferElement>::const_iterator BufferLayout::begin() const {
        return elements.begin();
    }
    std::vector<BufferElement>::const_iterator BufferLayout::end() const {
        return elements.end();
    }

    void BufferLayout::CalculateOffsetsAndStride() {

        size_t offset = 0;
        stride = 0;

        for (auto& element : elements) {
            
            element.offset = offset;

            offset += element.size;
            stride += element.size;
        }
    }
}
