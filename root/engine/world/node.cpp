// Project Boomerang : engine/world/node.cpp (c) 2020-2021 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

#include "node.hpp"

// Include dependencies
#include <GLM/glm/gtc/matrix_transform.hpp>

// Include boomerang libraries
#include "world.hpp"

namespace Boomerang::Core::World {

    Node::Node(const glm::vec3& _position, const glm::vec2& _size, const glm::vec2& _scale) {
        
        position = _position;
        size = _size;
        scale = _scale;

        rigidbody = std::make_shared<Boomerang::Core::Physics::Rigidbody>(GridToPixelCoord(position, size.x), size, scale);

        if (texture != nullptr)
            UpdateTransform();

        biome = BIOME::OCEAN;
    }

    Node::~Node() { };

    void Node::UpdateTransform() {

        transform = glm::translate(glm::mat4(1.0f), GridToPixelCoord(position, size.x))
                  * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
    }

    // Setters
    void Node::SetPosition(const glm::vec3& _position) {
        position = _position;
        UpdateTransform();
    }
    void Node::SetSize(const glm::vec2& _size) {
        size = _size;
        UpdateTransform();
    }
    void Node::SetScale(const glm::vec2& _scale) {
        scale = _scale;
        UpdateTransform();
    }
    void Node::SetTexture(const std::shared_ptr<Boomerang::Core::Graphics::Texture>& _texture) {
        texture = _texture;
        UpdateTransform();
    }
    void Node::SetBiome(const BIOME _biome, const std::shared_ptr<Boomerang::Core::Graphics::Texture>& _texture) {
        biome = _biome;
        SetTexture(_texture);
    }

    // Getters
    const glm::vec3& Node::GetPosition() const {
        return position;
    }
    const glm::vec3& Node::GetTruePosition() const {
        return GridToPixelCoord(position, size.x);
    }
    const glm::vec2& Node::GetSize() const {
        return size;
    }
    const glm::vec2& Node::GetScale() const {
        return scale;
    }
    const std::shared_ptr<Boomerang::Core::Graphics::Texture> Node::GetTexture() const {
        return texture;
    }
    const glm::mat4& Node::GetTransform() const {
        return transform;
    }

    bool Node::InFrame(const glm::vec3& _position, const glm::vec2& _windowSize) {
        
        glm::vec3 p = PixelToGridCoord(_position, size.x);

        float xMax = std::round((_position.x + _windowSize.x / 2) / size.x) + 1;
        float xMin = std::round((_position.x - _windowSize.x / 2) / size.x) - 1;
        float yMax = std::round((_position.y + _windowSize.y / 2) / size.y) + 1;
        float yMin = std::round((_position.y - _windowSize.y / 2) / size.y) - 1;

        if (position.x > xMax || position.x < xMin || position.y > yMax || position.y < yMin)
            return false;

        return true;
    }
}
