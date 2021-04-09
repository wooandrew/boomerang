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

namespace Boomerang::Core::Graphics {

    void Manager::init(const glm::vec4& color) {

        glad_glEnable(GL_BLEND);
        glad_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glad_glEnable(GL_DEPTH_TEST);
        glad_glEnable(GL_MULTISAMPLE);      // 4x MSAA

        //glad_glEnable(GL_CULL_FACE);

        SetClearColor(color);
    }
    void Manager::shutdown() {

    }

    void Manager::SetViewPort(int x, int y, int width, int height) {
        glad_glViewport(x, y, width, height);
    }
    void Manager::SetClearColor(const glm::vec4& color) {
        glad_glClearColor(color.r, color.g, color.b, color.a);
    }

    void Manager::Clear() {
        glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Manager::BeginRender() {
        Clear();
    }
    void Manager::EndRender(GLFWwindow* window) {
        glfwSwapBuffers(window);
    }

    void Manager::DrawIndexed(const std::shared_ptr<Vertex>& vtxArray) {
        glad_glDrawElements(GL_TRIANGLES, vtxArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
        glad_glBindTexture(GL_TEXTURE_2D, 0);
    }
}
