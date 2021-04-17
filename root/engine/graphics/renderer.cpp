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
#include <iostream>
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

        Manager::DrawIndexed(RenderData->__quad_vtx_array);
    }

    // Render text functions
    void Renderer::RenderText(const std::string& _string, const glm::vec2& _position, const glm::vec2& _scale, const glm::vec3& _color, const std::shared_ptr<Font>& _font) {
        RenderText(_string, glm::vec3(_position, 0), _scale, _color, _font);
    }
    void Renderer::RenderText(const std::string& _string, const glm::vec3& _position, const glm::vec2& _scale, const glm::vec3& _color, const std::shared_ptr<Font>& _font) {

        // Bind the shader and font atlas
        std::shared_ptr<Shader> shader = RenderData->__shader_library->GetMap().find("text")->second;
        shader->SetFloat3("u_Color", _color);
        _font->Bind();

        /*
         * Algorithm design
         *
         * Create a new float array. Each letter has 20 attributes
         *  => 12 positional attributes and 8 texcoord attributes.
         * Therefore, to render a string, there must be 20 * s.size
         *  amount of floats in the float array.
         *
         * The floats are stored in a VNVNVNVN (vertex / normal)
         *  format.
         *
         * When expanded:
         *
         *       V0x V0y V0z   N0x N0y     => Bottom Left
         *       V1x V1y V1z   N1x N1y     => Bottom Right
         *       V2x V2y V2z   N2x N2y     => Top Right
         *       V3x V3y V3z   N3x N3y     => Top Left
         *
         * Therefore, each set of attributes must be calculated
         *  and appended to the float array PER VERTEX in order
         *  for each character in the string to render.
         *
         * To calculate each vertex, use the attributes of each
         *  glyph stored in the appropriate container in fonts.
         *
         * Finally, calculate the index buffer for the entire
         *  string. Each character in the string has 6 index
         *  buffer attributes, and so the index buffer array
         *  must be of size 6 * s.size. The attribute values
         *  for each character is 4 plus that of the previous
         *  character, and begins with the following values:
         *
         *           =>$ { 0, 1, 2, 2, 3, 0 } $<=
         *
         * NOTE: All Vertex Buffer attributes must be normalized
         *        to the size of the quad that the texture will
         *        be projected on.
         */

         // Create a new float array of the correct size.
        float* __qvt = new float[20 * _string.size()];

        // Create a new uint32_t
        uint32_t* __qi = new uint32_t[6 * _string.size()]; // { 0, 1, 2, 2, 3, 0 };

        // Calculate the required width of the quad
        int wQuad = 0;
        int hQuad = 0;

        for (std::string::const_iterator i = _string.begin(); i < _string.end(); i++) {
            
            Font::GlyphData gc = _font->GetGlyphData()[*i];


            wQuad += gc.size.x;
            hQuad = std::max(hQuad, gc.size.y);
        }

        // Iterate through every character in the string
        for (int i = 0, iQVT = 0, iQI = 0, cursor = 0; i < _string.size(); i++) {

            Font::GlyphData gc = _font->GetGlyphData()[_string[i]];

            if (!gc.size.x || !gc.size.y)
                continue;

            // Bottom Left
            __qvt[iQVT++] = static_cast<float>(gc.bearing.x + cursor) / wQuad;
            __qvt[iQVT++] = static_cast<float>(gc.bearing.y + gc.size.y) / hQuad;
            __qvt[iQVT++] = 0.0f;
            __qvt[iQVT++] = gc.tx / _font->GetAtlasDimensions().x;
            __qvt[iQVT++] = 0.0f;

            // Bottom Right
            __qvt[iQVT++] = static_cast<float>(gc.bearing.x + gc.size.x + cursor) / wQuad;
            __qvt[iQVT++] = static_cast<float>(gc.bearing.y + gc.size.y) / hQuad;
            __qvt[iQVT++] = 0.0f;
            __qvt[iQVT++] = static_cast<float>(gc.tx + gc.size.x) / _font->GetAtlasDimensions().x;
            __qvt[iQVT++] = 0.0f;

            // Top Right
            __qvt[iQVT++] = static_cast<float>(gc.bearing.x + gc.size.x + cursor) / wQuad;
            __qvt[iQVT++] = static_cast<float>(gc.bearing.y) / hQuad;
            __qvt[iQVT++] = 0.0f;
            __qvt[iQVT++] = static_cast<float>(gc.tx + gc.size.x) / _font->GetAtlasDimensions().x;
            __qvt[iQVT++] = static_cast<float>(gc.size.y) / _font->GetAtlasDimensions().y;

            // Top Left
            __qvt[iQVT++] = static_cast<float>(gc.bearing.x + cursor) / wQuad;
            __qvt[iQVT++] = static_cast<float>(gc.bearing.y) / hQuad;
            __qvt[iQVT++] = 0.0f;
            __qvt[iQVT++] = gc.tx / _font->GetAtlasDimensions().x;
            __qvt[iQVT++] = static_cast<float>(gc.size.y) / _font->GetAtlasDimensions().y;

            cursor += gc.advance.x;

            __qi[iQI++] = 0 + 4 * i;
            __qi[iQI++] = 1 + 4 * i;
            __qi[iQI++] = 2 + 4 * i;
            __qi[iQI++] = 2 + 4 * i;
            __qi[iQI++] = 3 + 4 * i;
            __qi[iQI++] = 0 + 4 * i;
        }

        // Create vertex
        std::shared_ptr<Vertex> __vtx = std::make_shared<Vertex>();

        // Create and populate vertex buffer
        std::shared_ptr<VertexBuffer> __vxb = std::make_shared<VertexBuffer>(__qvt, 20 * _string.size());
        __vxb->SetLayout({ { ShaderDataType::Float3, "a_Position" }, { ShaderDataType::Float2, "a_TexCoord" } });
        __vtx->AddVertexBuffer(__vxb);

        // Create and populate index buffer
        std::shared_ptr<IndexBuffer> __ixb = std::make_shared<IndexBuffer>(__qi, 6 * _string.size());
        __vtx->SetIndexBuffer(__ixb);

        glm::vec2 _size = _font->GetAtlasDimensions();

        // Translate
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position) * glm::scale(glm::mat4(1.0f), { _size.x, _size.y, 1.0f });
        shader->SetMat4("u_Transform", transform);

        // Calculate
        __vtx->Bind();

        // Draw
        Manager::DrawIndexed(__vtx);
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

                Manager::DrawIndexed(RenderData->__quad_vtx_array);
            }
        }
    }

    void Renderer::RenderDebug(const glm::vec3& _position, const std::shared_ptr<Texture>& _texture) {

        std::shared_ptr<Shader> shader = RenderData->__shader_library->GetMap().find("basic")->second;
        shader->SetFloat4("u_Color", glm::vec4(1.f));
        _texture->Bind();

        float __qvt[] = {

            // Positions            // Tex Coords
            -0.5f, -0.5f, 0.0f,    0.0f, 0.0f,      // Bottom Left
             0.0f, -0.5f, 0.0f,    0.5f, 0.0f,      // Bottom Right
             0.0f,  0.5f, 0.0f,    0.5f, 1.0f,      // Top Right
            -0.5f,  0.5f, 0.0f,    0.0f, 1.0f,      // Top Left

            0.0f, -0.5f, 0.0f,    0.5f, 0.0f,      // Bottom Left
             0.5f, -0.5f, 0.0f,    1.0f, 0.0f,      // Bottom Right
             0.5f,  0.5f, 0.0f,    1.0f, 1.0f,      // Top Right
            0.0f,  0.5f, 0.0f,    0.5f, 1.0f,      // Top Left
        };

        std::shared_ptr<Vertex> __vtx = std::make_shared<Vertex>();

        std::shared_ptr<VertexBuffer> __vxb = std::make_shared<VertexBuffer>(__qvt, sizeof(__qvt));
        __vxb->SetLayout({ { ShaderDataType::Float3, "a_Position" }, { ShaderDataType::Float2, "a_TexCoord" } });
        __vtx->AddVertexBuffer(__vxb);

        uint32_t __qi[] = { 0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4 };
        std::shared_ptr<IndexBuffer> __ixb = std::make_shared<IndexBuffer>(__qi, sizeof(__qi) / sizeof(uint32_t));
        __vtx->SetIndexBuffer(__ixb);

        glm::vec2 _size = _texture->GetDimensions();
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position) * glm::scale(glm::mat4(1.0f), { _size.x, _size.y, 1.0f });

        shader->SetMat4("u_Transform", transform);
        __vtx->Bind();

        Manager::DrawIndexed(__vtx);
    }
}
