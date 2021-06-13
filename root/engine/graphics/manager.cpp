// Project Boomerang : engine/graphics/manager.cpp (c) 2020-2021 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

#include "manager.hpp"
#include <iostream>

namespace Boomerang::Core::Graphics::Manager {

    void init(const glm::vec4& color) {

        glad_glEnable(GL_BLEND);
        glad_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glad_glEnable(GL_DEPTH_TEST);
        glad_glEnable(GL_MULTISAMPLE);      // 4x MSAA

        //glad_glEnable(GL_CULL_FACE);

        SetClearColor(color);
    }
    void shutdown() {

    }

    void SetViewPort(int x, int y, int width, int height) {
        glad_glViewport(x, y, width, height);
    }
    void SetClearColor(const glm::vec4& color) {
        glad_glClearColor(color.r, color.g, color.b, color.a);
    }

    void Clear() {
        glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void BeginRender() {
        Clear();
    }
    void EndRender(GLFWwindow* window) {
        glfwSwapBuffers(window);
    }

    void DrawIndexed(const std::unique_ptr<VertexArray>& vtxArray, unsigned int _count) {

        unsigned int count = (_count == 0) ? vtxArray->GetIndexBuffer()->GetCount() * 6 : _count;

        glad_glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
        glad_glBindTexture(GL_TEXTURE_2D, 0);
    }
}
