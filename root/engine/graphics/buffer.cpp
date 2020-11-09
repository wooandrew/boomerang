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

#include <GLAD/glad.h>

namespace Boomerang::Core::Graphics {

    BufferElement::BufferElement(ShaderDataType _type, const std::string& _name, bool _normalized) {
        
        type = _type;
        name = _name;
        size = ShaderDataTypeSize(_type);
        normalized = _normalized;
    }

    uint32_t BufferElement::GetComponentCount() const {

        switch (type) {

        case ShaderDataType::Float:         return 1;
        case ShaderDataType::Float2:        return 2;
        case ShaderDataType::Float3:        return 3;
        case ShaderDataType::Float4:        return 4;
        case ShaderDataType::Mat3:          return 3 * 3;   // Check validity of size
        case ShaderDataType::Mat4:          return 4 * 4;
        case ShaderDataType::Int:           return 1;
        case ShaderDataType::Int2:          return 2;
        case ShaderDataType::Int3:          return 3;
        case ShaderDataType::Int4:          return 4;
        case ShaderDataType::Bool:          return 1;
        default:

            Boomerang::Misc::Logger::logger("BE000", "Error: Unknown shader data type.");

            return 0;
        }
    }

    // ***********************************************************
    // *** Buffer Layout *****************************************
    // ***********************************************************
    BufferLayout::BufferLayout() {
        stride = 0;
    }
    BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements) : elements(elements) {
        CalculateOffsetsAndStride();
    }

    unsigned int BufferLayout::GetStride() const {
        return stride;
    }
    const std::vector<BufferElement>& BufferLayout::GetElements() const {
        return elements;
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

    // ***********************************************************
    // *** Vertex Buffer *****************************************
    // ***********************************************************
    VertexBuffer::VertexBuffer(float* vertices, unsigned int size) {

        glad_glCreateBuffers(1, &RendererID);
        glad_glBindBuffer(GL_ARRAY_BUFFER, RendererID);
        glad_glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    VertexBuffer::~VertexBuffer() {
        glad_glDeleteBuffers(1, &RendererID);
    }

    void VertexBuffer::Bind() const {
        glad_glBindBuffer(GL_ARRAY_BUFFER, RendererID);
    }

    void VertexBuffer::Unbind() const {
        glad_glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    const BufferLayout& VertexBuffer::GetLayout() const {
        return layout;
    }

    void VertexBuffer::SetLayout(const BufferLayout& _layout) {
        layout = _layout;
    }

    // ***********************************************************
    // *** Index Buffer ******************************************
    // ***********************************************************
    IndexBuffer::IndexBuffer(unsigned int* indices, unsigned int _count) {

        count = _count;
        glad_glCreateBuffers(1, &RendererID);
        glad_glBindBuffer(GL_ARRAY_BUFFER, RendererID);
        glad_glBufferData(GL_ARRAY_BUFFER, _count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    }
    IndexBuffer::~IndexBuffer() {
        glad_glDeleteBuffers(1, &RendererID);
    }

    void IndexBuffer::Bind() const {
        glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererID);
    }
    void IndexBuffer::Unbind() const {
        glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    unsigned int IndexBuffer::GetCount() const {
        return count;
    }
}
