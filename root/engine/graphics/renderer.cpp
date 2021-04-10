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

namespace Boomerang::Core::Graphics {

    struct RenderStorage {

        RenderStorage() = default;

        std::shared_ptr<ShaderLibrary> __shader_library;
        std::shared_ptr<Vertex> __quad_vtx_array;
        std::shared_ptr<Texture> __white;
    };

    static RenderStorage* RenderData;

    void Renderer::init() {

        RenderData = new RenderStorage();
        RenderData->__quad_vtx_array = std::make_shared<Vertex>();

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

        RenderData->__white = std::make_shared<Texture>(glm::vec2(1, 1));
        uint32_t __white_data = 0xffffffff;
        RenderData->__white->SetData(&__white_data, sizeof(uint32_t));

        RenderData->__shader_library = std::make_shared<ShaderLibrary>(ShaderLibrary("assets/shaders/.shaders"));
    }

    void Renderer::shutdown() {
        delete RenderData;
    }

    void Renderer::StartScene(const std::shared_ptr<OrthoCam>& camera, const std::string& _shader) {

        RenderData->__shader_library->GetMap().find(_shader)->second->Bind();
        RenderData->__shader_library->GetMap().find(_shader)->second->SetMat4("u_ViewProjection", camera->GetViewProjectionMatrix());
    }

    void Renderer::EndScene() { }

    // Render texture functions
    void Renderer::RenderTexture(const glm::vec2& _position, const glm::vec2& _scale, const std::shared_ptr<Texture>& _texture) {
        RenderTexture({ _position.x, _position.y, 0.0f }, _scale, _texture);
    }
    void Renderer::RenderTexture(const glm::vec3& _position, const glm::vec2& _scale, const std::shared_ptr<Texture>& _texture) {

        RenderData->__shader_library->GetMap().find("basic")->second->SetFloat4("u_Color", glm::vec4(1.0f));
        _texture->Bind();

        float t_Width = static_cast<float>(_texture->GetDimensions().x) * _scale.x;
        float t_Height = static_cast<float>(_texture->GetDimensions().y) * _scale.y;

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position) * 
                              glm::scale(glm::mat4(1.0f), { t_Width, t_Height, 1.0f });

        RenderData->__shader_library->GetMap().find("basic")->second->SetMat4("u_Transform", transform);

        RenderData->__quad_vtx_array->Bind();
        Manager::DrawIndexed(RenderData->__quad_vtx_array);
    }

    // Render text functions
    void Renderer::RenderText(const std::string _string, glm::vec2 _position, const glm::vec2& _scale, const glm::vec3& _color, const std::shared_ptr<Font>& _font) {
        RenderText(_string, glm::vec3(_position, 0), _scale, _color, _font);
    }
    void Renderer::RenderText(const std::string _string, glm::vec3 _position, const glm::vec2& _scale, const glm::vec3& _color, const std::shared_ptr<Font>& _font) {

        std::shared_ptr<Shader> shader = RenderData->__shader_library->GetMap().find("text")->second;
        shader->SetFloat3("u_Color", _color);

        struct point {
            float x;
            float y;
            float s;
            float t;
        }; 
        std::vector<point> coords;
        coords.resize(6 * _string.size());

        int n = 0;

        for (std::string::const_iterator i = _string.begin(); i != _string.end(); ++i) {

            GlyphData gd = _font->GetGlyphData()[*i];

            float x2 = _position.x + gd.topLeft.x * _scale.x;
            float y2 = -_position.y - gd.topLeft.y * _scale.y;
            float w = gd.size.x * _scale.x;
            float h = gd.size.y * _scale.y;

            // Advance the cursor to the start of the next character
            _position.x += gd.advance.x * _scale.x;
            _position.y += gd.advance.y * _scale.y;

            // Skip glyphs that have no pixels
            if (!w || !h)
                continue;

            coords[n++] = point({ x2,     -y2    , gd.tx, 0 });
            coords[n++] = point({ x2 + w, -y2    , gd.tx + gd.size.x / _font->GetAtlasDimensions().x, 0 });
            coords[n++] = point({ x2,     -y2 - h, gd.tx,  gd.size.y / _font->GetAtlasDimensions().y });
            coords[n++] = point({ x2 + w, -y2    , gd.tx + gd.size.x / _font->GetAtlasDimensions().x, 0 });
            coords[n++] = point({ x2,     -y2 - h, gd.tx,  gd.size.y / _font->GetAtlasDimensions().y });
            coords[n++] = point({ x2 + w, -y2 - h, gd.tx + gd.size.x / _font->GetAtlasDimensions().x, gd.size.y / _font->GetAtlasDimensions().y });
        }


        glad_glBufferData(GL_ARRAY_BUFFER, sizeof(coords) / 6, coords.data(), GL_DYNAMIC_DRAW);
        glad_glDrawArrays(GL_TRIANGLES, 0, n);
    }

    // Draw static quad functions
    void Renderer::DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const glm::vec4& _color) {
        DrawQuad({ _position.x, _position.y, 0.0f }, _size, _color);
    }
    void Renderer::DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const glm::vec4& _color) {

        RenderData->__shader_library->GetMap().find("basic")->second->SetFloat4("u_Color", _color);
        RenderData->__white->Bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position) * 
                              glm::scale(glm::mat4(1.0f), { _size.x, _size.y, 1.0f });

        RenderData->__shader_library->GetMap().find("basic")->second->SetMat4("u_Transform", transform);
        RenderData->__quad_vtx_array->Bind();

        Manager::DrawIndexed(RenderData->__quad_vtx_array);
    }
    void Renderer::DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const float _rotation, const glm::vec4& _color) {
        DrawQuad({ _position.x, _position.y, 0.0f }, _size, _rotation, _color);
    }
    void Renderer::DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const float _rotation, const glm::vec4& _color) {

        RenderData->__shader_library->GetMap().find("basic")->second->SetFloat4("u_Color", _color);
        RenderData->__white->Bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position) * 
                              glm::scale(glm::mat4(1.0f), { _size.x, _size.y, 1.0f });

        transform = glm::rotate(transform, glm::radians(_rotation), { 0.f, 0.f, 1.f });
        RenderData->__shader_library->GetMap().find("basic")->second->SetMat4("u_Transform", transform);
        RenderData->__quad_vtx_array->Bind();

        Manager::DrawIndexed(RenderData->__quad_vtx_array);
    }

    // Render Grid (debug_mode)
    void Renderer::RenderGrid(const glm::vec2& _WindowSize,  const glm::vec3& _CameraPosition, const float _CellSize, const float _zoom) {

        std::shared_ptr<Shader> shader = RenderData->__shader_library->GetMap().find("grid")->second;
        shader->SetFloat4("u_Color", glm::vec4(1.f));

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 1.f)) * 
                              glm::scale(glm::mat4(1.0f), { _WindowSize.x, _WindowSize.y, 1.0f });

        shader->SetMat4("u_Transform", transform);
        shader->SetFloat("u_CellSize", _CellSize);
        shader->SetFloat2("u_Resolution", _WindowSize);
        shader->SetFloat3("u_CameraPosition", _CameraPosition);

        RenderData->__quad_vtx_array->Bind();

        Manager::DrawIndexed(RenderData->__quad_vtx_array);
    }

    // Render Chunk (debug_mode) -> this should be called from render world
    void Renderer::RenderChunk(const std::shared_ptr<Boomerang::Core::World::Chunk>& chunk, const float _CellSize, const glm::vec2& _WindowSize, const glm::vec3& _CameraPosition, const float _zoom) {

        std::shared_ptr<Shader> shader = RenderData->__shader_library->GetMap().find("basic")->second;
        shader->SetFloat4("u_Color", glm::vec4(1.f));

        for (auto const& [key, node] : chunk->GetMap()) {

            if (node->InFrame(_CameraPosition, _WindowSize)) {

                node->GetTexture()->Bind();
                shader->SetMat4("u_Transform", node->GetTransform());

                RenderData->__quad_vtx_array->Bind();
                Manager::DrawIndexed(RenderData->__quad_vtx_array);
            }
        }
    }
}
