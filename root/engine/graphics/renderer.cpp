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

#pragma warning(disable : 6386)     // Disable buffer overrun message: it wasn't overrun.

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
#include "buffer.hpp"
#include "../world/world.hpp"

namespace Boomerang::Core::Graphics::Renderer {

    struct RendererData {

        RendererData() = default;

        // Max per draw call
        const uint32_t MaxQuads = 10000;
        const uint32_t MaxVertices = MaxQuads * 4;
        const uint32_t MaxIndices = MaxQuads * 6;

        // Shaders
        std::unique_ptr<ShaderLibrary> __shader_library;
        
        // Vertex Array Data
        std::unique_ptr<VertexArray> __quad_vtx_array;
        std::shared_ptr<VertexBuffer> __quad_vtx_buffer;
        std::unique_ptr<VertexArray> __quad_vtx_array_fixed;
        unsigned int __quad_index_count = 0;
        
        // Texture storage
        std::shared_ptr<Texture> __white;
        std::shared_ptr<Texture> __test;
        std::vector<int> __samplers;

        // Vertex data storage
        Graphics::Vertex* __quad_vtx_buf_base = nullptr;
        Graphics::Vertex* __quad_vtx_buf_ptr = nullptr;
    };

    static RendererData sData;

    std::vector<glm::vec3> CalculateVertexPositions(const glm::vec3& _position, const glm::vec2& _size) {

        std::vector<glm::vec3> __vp;

        // Counter clockwise vertices
        __vp.push_back({ _position.x - (_size.x / 2), _position.y - (_size.y / 2), _position.z });       // Bottom Left
        __vp.push_back({ _position.x + (_size.x / 2), _position.y - (_size.y / 2), _position.z });       // Bottom Right
        __vp.push_back({ _position.x + (_size.x / 2), _position.y + (_size.y / 2), _position.z });       // Top Right
        __vp.push_back({ _position.x - (_size.x / 2), _position.y + (_size.y / 2), _position.z });       // Top Left

        return __vp;
    }

    std::vector<glm::vec3> RotateVertices(const std::vector<glm::vec3>& _vertices, const glm::vec3& _position, const float _rotation) {

        std::vector<glm::vec3> __rv;

        const glm::mat4 matT = glm::translate(glm::identity<glm::mat4>(), -_position);
        const glm::mat4 matR = glm::rotate(glm::identity<glm::mat4>(), glm::radians(_rotation), { 0.f, 0.f, 1.f });
        const glm::mat4 matRT = glm::translate(glm::identity<glm::mat4>(), _position);

        for (auto& vtx : _vertices)
            __rv.push_back(glm::vec3(matRT * matR * matT * glm::vec4(vtx, 1.0f)));

        return __rv;
    }

    void init() {

        // Create Vertex Array (dynamic)
        sData.__quad_vtx_array = std::make_unique<VertexArray>();
        sData.__quad_vtx_buf_base = new Graphics::Vertex[sData.MaxVertices];

        // Create Vertex Buffer (dynamic)
        sData.__quad_vtx_buffer = std::make_shared<VertexBuffer>();
        sData.__quad_vtx_buffer->Create(sData.MaxVertices * sizeof(Graphics::Vertex));

        sData.__quad_vtx_buffer->SetLayout({ { ShaderDataType::Float3, "a_Position" },
                                             { ShaderDataType::Float2, "a_TexCoord" },
                                             { ShaderDataType::Float4, "a_Color"},
                                             { ShaderDataType::Float, "a_TexSlot"} });

        sData.__quad_vtx_array->AddVertexBuffer(sData.__quad_vtx_buffer);

        // Create Index Buffer (dynamic)
        uint32_t* __quad_indices = new uint32_t[sData.MaxIndices];

        uint32_t offset = 0;
        for (uint32_t i = 0; i < sData.MaxIndices; i += 6) {

            __quad_indices[i + 0] = offset + 0;
            __quad_indices[i + 1] = offset + 1;
            __quad_indices[i + 2] = offset + 2;

            __quad_indices[i + 3] = offset + 2;
            __quad_indices[i + 4] = offset + 3;
            __quad_indices[i + 5] = offset + 0;

            offset += 4;
        }

        std::shared_ptr<IndexBuffer> __quad_ib = std::make_shared<IndexBuffer>(__quad_indices, sData.MaxIndices * sizeof(uint32_t));
        sData.__quad_vtx_array->SetIndexBuffer(__quad_ib);

        // Create Vertex Array (fixed)
        sData.__quad_vtx_array_fixed = std::make_unique<VertexArray>();

        float __quad_vertices[] = {

            // Positions            // Tex Coords
            -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, // Bottom Left
             0.5f, -0.5f, 0.0f,     1.0f, 0.0f, // Bottom Right
             0.5f,  0.5f, 0.0f,     1.0f, 1.0f, // Top Right
            -0.5f,  0.5f, 0.0f,     0.0f, 1.0f  // Top Left
        };

        // Create Vertex Buffer (fixed)
        std::shared_ptr<VertexBuffer> __quad_vb_fixed = std::make_shared<VertexBuffer>();
        __quad_vb_fixed->Create(__quad_vertices, sizeof(__quad_vertices));
        __quad_vb_fixed->SetLayout({ { ShaderDataType::Float3, "a_Position" }, { ShaderDataType::Float2, "a_TexCoord" }});
        sData.__quad_vtx_array_fixed->AddVertexBuffer(__quad_vb_fixed);

        // Create Index Buffer (fixed)
        uint32_t __quad_indices_fixed[6] = { 0, 1, 2, 2, 3, 0 };
        std::shared_ptr<IndexBuffer> __quad_ib_fixed = std::make_shared<IndexBuffer>(__quad_indices_fixed, sizeof(__quad_indices_fixed));
        sData.__quad_vtx_array_fixed->SetIndexBuffer(__quad_ib_fixed);
        
        // Create 1x1 White Texture
        uint32_t __white_data = 0xffffffff;
        sData.__white = std::make_shared<Texture>(glm::vec2(1, 1));
        sData.__white->SetData(&__white_data, sizeof(uint32_t));

        // Initialize Shader Library
        sData.__shader_library = std::make_unique<ShaderLibrary>(ShaderLibrary("assets/shaders/.shaders"));

        sData.__white->Bind(0);

        sData.__test = std::make_shared<Texture>("assets/nodes/ocean_125.png");

        sData.__quad_vtx_array->Bind();

        for (int i = 0; i < 32; i++)
            sData.__samplers.push_back(i);

        delete[] __quad_indices;
    }

    void shutdown() {
        delete[] sData.__quad_vtx_buf_base;
    }

    // Add to batch
    void AddQuad(const std::vector<glm::vec3>& _vertices, const glm::vec4& _color, const float _texslot) {

        // Bottom Left
        sData.__quad_vtx_buf_ptr->position = _vertices[0];
        sData.__quad_vtx_buf_ptr->texcoord = { 0.f, 0.f };
        sData.__quad_vtx_buf_ptr->color = _color;
        sData.__quad_vtx_buf_ptr->texslot = 0.f;
        sData.__quad_vtx_buf_ptr++;

        // Bottom Right
        sData.__quad_vtx_buf_ptr->position = _vertices[1];
        sData.__quad_vtx_buf_ptr->texcoord = { 1.f, 0.f };
        sData.__quad_vtx_buf_ptr->color = _color;
        sData.__quad_vtx_buf_ptr->texslot = 0.f;
        sData.__quad_vtx_buf_ptr++;

        // Top Right
        sData.__quad_vtx_buf_ptr->position = _vertices[2];
        sData.__quad_vtx_buf_ptr->texcoord = { 1.f, 1.f };
        sData.__quad_vtx_buf_ptr->color = _color;
        sData.__quad_vtx_buf_ptr->texslot = 0.f;
        sData.__quad_vtx_buf_ptr++;

        // Top Left
        sData.__quad_vtx_buf_ptr->position = _vertices[3];
        sData.__quad_vtx_buf_ptr->texcoord = { 0.f, 1.f };
        sData.__quad_vtx_buf_ptr->color = _color;
        sData.__quad_vtx_buf_ptr->texslot = 0.f;
        sData.__quad_vtx_buf_ptr++;

        sData.__quad_index_count += 6;
    }

    // Render commands
    void StartScene(const std::unique_ptr<OrthoCam>& camera, const std::string& _shader) {

        sData.__quad_index_count = 0;
        sData.__quad_vtx_buf_ptr = sData.__quad_vtx_buf_base;

        sData.__shader_library->GetMap().find(_shader)->second->Bind();
        sData.__shader_library->GetMap().find(_shader)->second->SetInt1v("u_Textures", 32, sData.__samplers.data());
        sData.__shader_library->GetMap().find(_shader)->second->SetMat4("u_ViewProjection", camera->GetViewProjectionMatrix());
        sData.__shader_library->GetMap().find(_shader)->second->SetFloat4("u_Color", glm::vec4(1.f));
        sData.__shader_library->GetMap().find(_shader)->second->SetMat4("u_Transform", glm::mat4(1.0f));
    }
    void FlushScene() {

        sData.__test->Bind(0);

        uint32_t __size = reinterpret_cast<uint8_t*>(sData.__quad_vtx_buf_ptr) - 
                          reinterpret_cast<uint8_t*>(sData.__quad_vtx_buf_base);

        sData.__quad_vtx_buffer->SetData(sData.__quad_vtx_buf_base, __size);

        Manager::DrawIndexed(sData.__quad_vtx_array, sData.__quad_index_count);
    }
    void EndScene() {
        FlushScene();
    }

    // Render texture functions
    void RenderTexture(const glm::vec2& _position, const glm::vec2& _scale, const std::shared_ptr<Texture>& _texture) {
        RenderTexture({ _position.x, _position.y, 0.0f }, _scale, _texture);
    }
    void RenderTexture(const glm::vec3& _position, const glm::vec2& _scale, const std::shared_ptr<Texture>& _texture) {

        sData.__shader_library->GetMap().find("basic")->second->SetFloat4("u_Color", glm::vec4(1.0f));
        _texture->Bind();

        float t_Width = static_cast<float>(_texture->GetDimensions().x) * _scale.x;
        float t_Height = static_cast<float>(_texture->GetDimensions().y) * _scale.y;

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position) * 
                              glm::scale(glm::mat4(1.0f), { t_Width, t_Height, 1.0f });

        sData.__shader_library->GetMap().find("basic")->second->SetMat4("u_Transform", transform);

        Manager::DrawIndexed(sData.__quad_vtx_array_fixed);
    }

    // Render text functions
    void RenderText(const std::string& _string, const glm::vec2& _position, const glm::vec2& _scale, const glm::vec3& _color, const Font& _font) {
        RenderText(_string, glm::vec3(_position, 0), _scale, _color, _font);
    }
    void RenderText(const std::string& _string, const glm::vec3& _position, const glm::vec2& _scale, const glm::vec3& _color, const Font& _font) {

        std::shared_ptr<Shader> shader = sData.__shader_library->GetMap().find("text")->second;
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

            Manager::DrawIndexed(sData.__quad_vtx_array_fixed);
        }
    }

    // Draw static quad functions
    void DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const glm::vec4& _color) {
        DrawQuad({ _position.x, _position.y, 0.0f }, _size, _color);
    }
    void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const glm::vec4& _color) {
        AddQuad(CalculateVertexPositions(_position, _size), _color);
    }
    void DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const float _rotation, const glm::vec4& _color) {
        DrawQuad({ _position.x, _position.y, 0.0f }, _size, _rotation, _color);
    }
    void DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const float _rotation, const glm::vec4& _color) {
        AddQuad(RotateVertices(CalculateVertexPositions(_position, _size), _position, _rotation), _color);
    }

    // Shader Only Rendering
    //void Renderer::LoadingDots(const int _count, const float _spacing, const float _radius, const glm::vec4& _color, const float _runtime) {
    //    LoadingDots({ _position.x, _position.y, 0.0f }, _radius, _color);
    //}
    void LoadingDots(const glm::vec2& _WindowSize, const int _count, const float _spacing, const float _radius, const glm::vec4& _color, const std::chrono::steady_clock::duration& _clock) {

        AddQuad(CalculateVertexPositions({ 0.f, 0.f, 1.f }, { 100.f, 100.f }), _color);

        std::shared_ptr<Shader> shader = sData.__shader_library->GetMap().find("dots")->second;

        float runtime = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(_clock).count() / 1000.f;

        shader->SetFloat2("u_Resolution", _WindowSize);
        //shader->SetMat4("u_Transform", transform);
        shader->SetInt("u_CircleCount", _count);
        shader->SetFloat("u_Spacing", _spacing);
        shader->SetFloat("u_Radius", _radius);
        shader->SetFloat("u_RunTime", runtime);
    }

    // Render Grid (debug_mode)
    void RenderGrid(const glm::vec2& _WindowSize,  const glm::vec3& _CameraPosition, const float _CellSize, const float _zoom) {

        AddQuad(CalculateVertexPositions({ 0, 0, 1.f }, { _WindowSize.x, _WindowSize.y }), glm::vec4(1.f));
        std::shared_ptr<Shader> shader = sData.__shader_library->GetMap().find("grid")->second;

        shader->SetFloat("u_CellSize", _CellSize);
        shader->SetFloat2("u_Resolution", _WindowSize);
        shader->SetFloat3("u_CameraPosition", _CameraPosition);
    }

    // Render Chunk (debug_mode) -> this should be called from render world
    void RenderChunk(const std::shared_ptr<Boomerang::Core::World::Chunk>& chunk, const glm::vec2& _WindowSize, const glm::vec3& _CameraPosition, const float _CellSize, const float _zoom) {

        std::shared_ptr<Shader> shader = sData.__shader_library->GetMap().find("basic")->second;
        shader->SetFloat4("u_Color", glm::vec4(1.f));

        for (auto const& [key, node] : chunk->GetMap()) {

            if (node->InFrame(_CameraPosition, _WindowSize)) {

                node->GetTexture()->Bind();
                shader->SetMat4("u_Transform", node->GetTransform());

                Manager::DrawIndexed(sData.__quad_vtx_array_fixed);
            }
        }
    }
}
