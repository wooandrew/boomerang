// Project Boomerang : engine/graphics/manager.hpp (c) 2020 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

#ifndef BOOMERANG_ENGINE_GRAPHICS_MANAGER
#define BOOMERANG_ENGINE_GRAPHICS_MANAGER

// Include standard library
#include <memory>

// Include dependencies
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm/vec4.hpp>

// Include boomerang libraries
#include "vertex.hpp"

namespace Boomerang::Core::Graphics {

    class Manager {

        /// Graphics manager

    public:

        Manager() = delete;

        static void init(const glm::vec4& color = glm::vec4(0));
        static void shutdown();

        static void SetViewPort(int x, int y, int width, int height);
        static void SetClearColor(const glm::vec4& color);
        static void Clear();

        static void BeginRender();
        static void EndRender(GLFWwindow* window);

        static void DrawIndexed(const std::shared_ptr<Vertex>& vtxArray);
    };
}

#endif // !BOOMERANG_ENGINE_GRAPHICS_MANAGER
