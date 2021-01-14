// Project Boomerang : engine/graphics/vertex.cpp (c) 2020-2021 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

#include "vertex.hpp"

#include <ASWL/logger.hpp>

namespace Boomerang::Core::Graphics {

    Vertex::Vertex() {
        VertexBufferIndex = 0;
        glad_glCreateVertexArrays(1, &RendererID);
    }
    Vertex::~Vertex() {
        glad_glDeleteVertexArrays(1, &RendererID);
    }

    void Vertex::Bind() const {
        glad_glBindVertexArray(RendererID);
    }
    void Vertex::Unbind() const {
        glad_glBindVertexArray(0);
    }

    void Vertex::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vtxBuffer) {

        if (vtxBuffer->GetLayout().GetElements().size() == 0) {
            ASWL::Logger::logger("VA001", "Error: Vertex Buffer has no layout.");
            return;
        }

        glad_glBindVertexArray(RendererID);
        vtxBuffer->Bind();

        const auto& layout = vtxBuffer->GetLayout();
        for (const auto& element : layout) {

            glad_glEnableVertexAttribArray(VertexBufferIndex);
            glad_glVertexAttribPointer(VertexBufferIndex, element.GetComponentCount(), ShaderTypeToGLBaseType(element.type),
                                       element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.offset);
            VertexBufferIndex++;
        }

        VertexBuffers.push_back(vtxBuffer);
    }

    void Vertex::SetIndexBuffer(const std::shared_ptr<Boomerang::Core::Graphics::IndexBuffer>& idxBuffer) {

        glad_glBindVertexArray(RendererID);
        idxBuffer->Bind();

        ptrIndexBuffer = idxBuffer;
    }

    const std::vector<std::shared_ptr<VertexBuffer>>& Vertex::GetVertexBuffers() const {
        return VertexBuffers;
    }
    const std::shared_ptr<IndexBuffer>& Vertex::GetIndexBuffer() const {
        return ptrIndexBuffer;
    }
}
