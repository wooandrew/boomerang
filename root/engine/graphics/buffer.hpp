// Project Boomerang : engine/graphics/buffer.hpp (c) 2020 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

#ifndef BOOMERANG_ENGINE_GRAPHICS_BUFFER
#define BOOMERANG_ENGINE_GRAPHICS_BUFFER

/// Various required rendering buffers

// Include standard library
#include <string>
#include <vector>
#include <initializer_list>

// Include boomerang libraries
#include "shaders/shaders.hpp"

namespace Boomerang::Core::Graphics {

    struct BufferElement {

        std::string name;
        ShaderDataType type;
        unsigned int size;
        size_t offset;
        bool normalized;

        BufferElement() = default;
        BufferElement(ShaderDataType _type, const std::string& _name, bool _normalized = false);

        uint32_t GetComponentCount() const;
    };

    class BufferLayout {

        /// Buffer layout class

    public:

        BufferLayout();
        BufferLayout(const std::initializer_list<BufferElement>& elements);

        unsigned int GetStride() const;
        const std::vector<BufferElement>& GetElements() const;

        // Vector iterators
        std::vector<BufferElement>::iterator begin();
        std::vector<BufferElement>::iterator end();
        std::vector<BufferElement>::const_iterator begin() const;
        std::vector<BufferElement>::const_iterator end() const;

    private:
        
        //
        // https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_buffer_offset_and_stride
        //
        void CalculateOffsetsAndStride();

        std::vector<BufferElement> elements;
        unsigned int stride;
    };

    class VertexBuffer {

        /// Vertex buffer class

    public:

        VertexBuffer(float* vertices, unsigned int size);
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;

        const BufferLayout& GetLayout() const;
        void SetLayout(const BufferLayout& _layout);

    private:

        unsigned int RendererID;
        BufferLayout layout;
    };

    class IndexBuffer {

        /// Index buffer class

    public:

        IndexBuffer(unsigned int* indices, unsigned int _count);
        ~IndexBuffer();

        void Bind() const;
        void Unbind() const;

        unsigned int GetCount() const;

    private:

        unsigned int RendererID;
        unsigned int count;
    };
}

#endif // !BOOMERANG_ENGINE_GRAPHICS_BUFFER
