// Project Boomerang : engine/graphics/renderer.cpp (c) 2020-2021 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

#include "renderer.hpp"

// Include standard library
#include <vector>

// Include dependencies
#include <GLM/glm/gtc/matrix_transform.hpp>
//#include <GLM/glm/gtc/color_space.hpp>

// Include boomerang libraries
#include "manager.hpp"
#include "shaders.hpp"
#include "vertex.hpp"
#include "../world/world.hpp"

namespace Boomerang::Core::Graphics::Renderer {

    struct RenderStorage {

        RenderStorage() = default;

        std::unique_ptr<ShaderLibrary> __shader_library;
        std::unique_ptr<Vertex> __quad_vtx_array;
        std::unique_ptr<Texture> __white;
    };

    static RenderStorage* RenderData;

    void init() {

        RenderData = new RenderStorage();
        RenderData->__quad_vtx_array = std::make_unique<Vertex>();

        float __quad_vertices[] = {

            // Positions            // Tex Coords
            -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, // Bottom Left
             0.5f, -0.5f, 0.0f,     1.0f, 0.0f, // Bottom Right
             0.5f,  0.5f, 0.0f,     1.0f, 1.0f, // Top Right
            -0.5f,  0.5f, 0.0f,     0.0f, 1.0f  // Top Left
        };

        std::shared_ptr<VertexBuffer> __quadVB = std::make_shared<VertexBuffer>(__quad_vertices, sizeof(__quad_vertices));
        __quadVB->SetLayout({ { ShaderDataType::Float3, "a_Position" }, { ShaderDataType::Float2, "a_TexCoord" } });            // GLSL Data
        RenderData->__quad_vtx_array->AddVertexBuffer(__quadVB);

        uint32_t __quad_indices[6] = { 0, 1, 2, 2, 3, 0 };
        std::shared_ptr<IndexBuffer> __quadIB = std::make_shared<IndexBuffer>(__quad_indices, sizeof(__quad_indices) / sizeof(uint32_t));
        RenderData->__quad_vtx_array->SetIndexBuffer(__quadIB);

        RenderData->__white = std::make_unique<Texture>(glm::vec2(1, 1));
        uint32_t __white_data = 0xffffffff;
        RenderData->__white->SetData(&__white_data, sizeof(uint32_t));

        RenderData->__shader_library = std::make_unique<ShaderLibrary>(ShaderLibrary("assets/shaders/.shaders"));
    }

    void shutdown() {
        delete RenderData;
    }

    void StartScene(const std::unique_ptr<OrthoCam>& camera, const std::string& _shader) {

        RenderData->__shader_library->GetMap().find(_shader)->second->Bind();
        RenderData->__shader_library->GetMap().find(_shader)->second->SetMat4("u_ViewProjection", camera->GetViewProjectionMatrix());
    }

    void FlushScene() {

    }

    void EndScene() { 
    
    }

    // Render texture functions
    void RenderTexture(const glm::vec2& _position, const glm::vec2& _scale, const std::shared_ptr<Texture>& _texture) {
        RenderTexture({ _position.x, _position.y, 0.0f }, _scale, _texture);
    }
    void RenderTexture(const glm::vec3& _position, const glm::vec2& _scale, const std::shared_ptr<Texture>& _texture) {

        RenderData->__shader_library->GetMap().find("basic")->second->SetFloat4("u_Color", glm::vec4(1.0f));
        _texture->Bind();

        float t_Width = static_cast<float>(_texture->GetDimensions().x) * _scale.x;
        float t_Height = static_cast<float>(_texture->GetDimensions().y) * _scale.y;

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position) * 
                              glm::scale(glm::mat4(1.0f), { t_Width, t_Height, 1.0f });

        RenderData->__shader_library->GetMap().find("basic")->second->SetMat4("u_Transform", transform);

        Manager::DrawIndexed(RenderData->__quad_vtx_array);
    }

    // Render text functions
    void RenderText(const std::string& _string, const glm::vec2& _position, const glm::vec2& _scale, const glm::vec3& _color, const Font& _font) {
        RenderText(_string, glm::vec3(_position, 0), _scale, _color, _font);
    }
    void RenderText(const std::string& _string, const glm::vec3& _position, const glm::vec2& _scale, const glm::vec3& _color, const Font& _font) {

        std::shared_ptr<Shader> shader = RenderData->__shader_library->GetMap().find("text")->second;
        shader->SetFloat3("u_Color", _color);

        /* 
         * TODO: Rewrite this horrible code. Why did I ever think it was a good idea to
         *       bind/render a single glyph at a time? Stupid, silly Andrew. What we
         *       SHOULD be doing is batch rendering the glyphs. Actually, we should be
         *       batch rendering EVERYTHING.
         */

        float px = _position.x;
        float pz = _position.z;

        float offset = 0;

        // Calculate string offset
        for (std::string::const_iterator i = _string.begin(); i != _string.end(); ++i) {

            Character ch = _font.GetCharacters().find(*i)->second;

            offset = std::abs((px + ch.bearing.x + (ch.size.x / 2.f)) * _scale.x - _position.x);
            px += ((ch.advance >> 6) - (ch.bearing.x / 2.f));
        }

        offset /= 2;

        px = _position.x;

        for (std::string::const_iterator i = _string.begin(); i != _string.end(); ++i) {

            Character ch = _font.GetCharacters().find(*i)->second;
            ch.Bind();

            float xPos = (px + ch.bearing.x + (ch.size.x / 2.f)) * _scale.x - offset;
            float yPos = (_position.y + (ch.size.y / 2.f) - (ch.size.y - ch.bearing.y) - (_font.GetSize() / 2.f) * 0.75f) * _scale.y;

            float t_Width = static_cast<float>(ch.size.x) * _scale.x;
            float t_Height = static_cast<float>(ch.size.y) * _scale.y;

            glm::mat4 transform = glm::translate(glm::mat4(1.0f), { xPos, yPos, pz += 0.00001 }) * 
                                  glm::scale(glm::mat4(1.0f), { t_Width, t_Height, 1.0f });

            shader->SetMat4("u_Transform", transform);

            px += ((ch.advance >> 6) - (ch.bearing.x / 2.f));

            Manager::DrawIndexed(RenderData->__quad_vtx_array);
        }
    }

    // Draw static quad functions
    void DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const glm::vec4& _color) {
        DrawQuad({ _position.x, _position.y, 0.0f }, _size, _color);
    }
    void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const glm::vec4& _color) {

        RenderData->__shader_library->GetMap().find("basic")->second->SetFloat4("u_Color", _color);
        RenderData->__white->Bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position) * 
                              glm::scale(glm::mat4(1.0f), { _size.x, _size.y, 1.0f });

        RenderData->__shader_library->GetMap().find("basic")->second->SetMat4("u_Transform", transform);

        Manager::DrawIndexed(RenderData->__quad_vtx_array);
    }
    void DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const float _rotation, const glm::vec4& _color) {
        DrawQuad({ _position.x, _position.y, 0.0f }, _size, _rotation, _color);
    }
    void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const float _rotation, const glm::vec4& _color) {

        RenderData->__shader_library->GetMap().find("basic")->second->SetFloat4("u_Color", _color);
        RenderData->__white->Bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position) * 
                              glm::scale(glm::mat4(1.0f), { _size.x, _size.y, 1.0f });

        transform = glm::rotate(transform, glm::radians(_rotation), { 0.f, 0.f, 1.f });
        RenderData->__shader_library->GetMap().find("basic")->second->SetMat4("u_Transform", transform);

        auto s = glm::translate(glm::mat4(1.0f), _position);
        

        Manager::DrawIndexed(RenderData->__quad_vtx_array);
    }

    // Shader Only Rendering
    //void Renderer::LoadingDots(const int _count, const float _spacing, const float _radius, const glm::vec4& _color, const float _runtime) {
    //    LoadingDots({ _position.x, _position.y, 0.0f }, _radius, _color);
    //}
    void LoadingDots(const glm::vec2& _WindowSize, const int _count, const float _spacing, const float _radius, const glm::vec4& _color, const std::chrono::steady_clock::duration& _clock) {

        std::shared_ptr<Shader> shader = RenderData->__shader_library->GetMap().find("dots")->second;
        shader->SetFloat4("u_Color", _color);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 1.f)) *
                              glm::scale(glm::mat4(1.0f), { 100, 100, 1.0f });

        float runtime = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(_clock).count() / 1000.f;

        shader->SetFloat2("u_Resolution", _WindowSize);
        shader->SetMat4("u_Transform", transform);
        shader->SetInt("u_CircleCount", _count);
        shader->SetFloat("u_Spacing", _spacing);
        shader->SetFloat("u_Radius", _radius);
        shader->SetFloat("u_RunTime", runtime);

        Manager::DrawIndexed(RenderData->__quad_vtx_array);
    }

    // Render Grid (debug_mode)
    void RenderGrid(const glm::vec2& _WindowSize,  const glm::vec3& _CameraPosition, const float _CellSize, const float _zoom) {

        std::shared_ptr<Shader> shader = RenderData->__shader_library->GetMap().find("grid")->second;
        shader->SetFloat4("u_Color", glm::vec4(1.f));

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 1.f)) * 
                              glm::scale(glm::mat4(1.0f), { _WindowSize.x, _WindowSize.y, 1.0f });

        shader->SetMat4("u_Transform", transform);
        shader->SetFloat("u_CellSize", _CellSize);
        shader->SetFloat2("u_Resolution", _WindowSize);
        shader->SetFloat3("u_CameraPosition", _CameraPosition);

        Manager::DrawIndexed(RenderData->__quad_vtx_array);
    }

    // Render Chunk (debug_mode) -> this should be called from render world
    void RenderChunk(const std::shared_ptr<Boomerang::Core::World::Chunk>& chunk, const glm::vec2& _WindowSize, const glm::vec3& _CameraPosition, const float _CellSize, const float _zoom) {

        std::shared_ptr<Shader> shader = RenderData->__shader_library->GetMap().find("basic")->second;
        shader->SetFloat4("u_Color", glm::vec4(1.f));

        for (auto const& [key, node] : chunk->GetMap()) {

            if (node->InFrame(_CameraPosition, _WindowSize)) {

                node->GetTexture()->Bind();
                shader->SetMat4("u_Transform", node->GetTransform());

                Manager::DrawIndexed(RenderData->__quad_vtx_array);
            }
        }
    }
}
