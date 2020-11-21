// Project Boomerang : engine/graphics/renderer.cpp (c) 2020 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

#include "renderer.hpp"

#include <vector>

#include <GLM/glm/gtc/matrix_transform.hpp>

#include "manager.hpp"
#include "vertex.hpp"
#include "texture.hpp"
#include "shaders.hpp"

#include "../../misc/utilities.hpp"

namespace Boomerang::Core::Graphics {

    struct RenderStorage {

        RenderStorage() = default;

        //std::shared_ptr<Vertex>
        std::shared_ptr<Vertex> __quad_vtx_array;
        std::shared_ptr<Shader> __shader;
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

        RenderData->__white = std::make_shared<Texture>(util::dimen2d<int>(1, 1));
        uint32_t __white_data = 0xffffffff;
        RenderData->__white->SetData(&__white_data, sizeof(uint32_t));

        RenderData->__shader = std::make_shared<Shader>("assets/shaders/basic-vert.glsl", "assets/shaders/basic-frag.glsl");
        RenderData->__shader->Bind();
        RenderData->__shader->SetInt("u_Texture", 0);
    }

    void Renderer::shutdown() {
        delete RenderData;
    }

    void Renderer::StartScene(const OrthoCam& camera) {
        RenderData->__shader->Bind();
        RenderData->__shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
    }

    void Renderer::EndScene() { }

    // Render texture functions
    void Renderer::RenderTexture(const glm::vec2& _position, const glm::vec2& _scale, const std::shared_ptr<Texture>& _texture) {
        RenderTexture({ _position.x, _position.y, 0.0f }, _scale, _texture);
    }
    void Renderer::RenderTexture(const glm::vec3& _position, const glm::vec2& _scale, const std::shared_ptr<Texture>& _texture) {

        RenderData->__shader->SetFloat4("u_Color", glm::vec4(1.0f));
        _texture->Bind();

        float t_Width = static_cast<float>(_texture->GetDimensions().x) * _scale.x;
        float t_Height = static_cast<float>(_texture->GetDimensions().y) * _scale.y;

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position) * glm::scale(glm::mat4(1.0f), { t_Width, t_Height, 1.0f });
        RenderData->__shader->SetMat4("u_Transform", transform);

        RenderData->__quad_vtx_array->Bind();
        Manager::DrawIndexed(RenderData->__quad_vtx_array);
    }

    // Static Quad Render functions
    void Renderer::DrawQuad(const glm::vec2& _position, const glm::vec2& _size, const glm::vec4& _color) {
        DrawQuad({ _position.x, _position.y, 0.0f }, _size, _color);
    }
    void Renderer::DrawQuad(const glm::vec3& _position, const glm::vec2& _size, const glm::vec4& _color) {
        
        RenderData->__shader->SetFloat4("u_Color", _color);
        RenderData->__white->Bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position) * glm::scale(glm::mat4(1.f), { _size.x, _size.y, 1.0f });
        RenderData->__shader->SetMat4("u_Transform", transform);
        RenderData->__quad_vtx_array->Bind();

        Manager::DrawIndexed(RenderData->__quad_vtx_array);
    }
}
