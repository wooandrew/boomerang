// Project Boomerang : engine/graphics/renderer.hpp (c) 2020-2021 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

#pragma once

#ifndef BOOMERANG_ENGINE_GRAPHICS_RENDERER
#define BOOMERANG_ENGINE_GRAPHICS_RENDERER

// Include standard library
#include <string>
#include <memory>

// Include dependencies
#include <GLM/glm/glm.hpp>

// Include boomerang libraries
#include "font.hpp"
#include "texture.hpp"
#include "camera/orthocam.hpp"
#include "../world/chunk.hpp"

namespace Boomerang::Core::Graphics {

    class Renderer {

        /// Static rendering functions

    public:

        Renderer() = delete;
        
        static void init();
        static void shutdown();

        static void StartScene(const std::unique_ptr<OrthoCam>& camera, const std::string& _shader = "basic");
        static void EndScene();

        // Render Texture
        static void RenderTexture(const glm::vec2& _position, const glm::vec2& _scale, const std::shared_ptr<Texture>& _texture);
        static void RenderTexture(const glm::vec3& _position, const glm::vec2& _scale, const std::shared_ptr<Texture>& _texture);

        // Render Text
        static void RenderText(const std::string& _string, const glm::vec2& _position, const glm::vec2& _scale, const glm::vec3& _color, const Font& _font);
        static void RenderText(const std::string& _string, const glm::vec3& _position, const glm::vec2& _scale, const glm::vec3& _color, const Font& _font);

        // Draw Static Quad
        static void DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const glm::vec4& _color);
        static void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const glm::vec4& _color);
        static void DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const float _rotation, const glm::vec4& _color);
        static void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const float _rotation, const glm::vec4& _color);

        // Render Grid (debug_mode)
        static void RenderGrid(const glm::vec2& _WindowSize, const glm::vec3& _CameraPosition, const float _CellSize, const float _zoom = 1.f);

        // Render Chunk (debug_mode) -> this should be called from render world
        static void RenderChunk(const std::shared_ptr<Boomerang::Core::World::Chunk>& chunk, const glm::vec2& _WindowSize, const glm::vec3& _CameraPosition, const float _CellSize, const float _zoom = 1.f);
    };
}

#endif // !BOOMERANG_ENGINE_GRAPHICS_RENDERER
