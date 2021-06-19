// Project Boomerang : engine/world/node.hpp (c) 2020-2021 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

#ifndef BOOMERANG_ENGINE_WORLD_NODE
#define BOOMERANG_ENGINE_WORLD_NODE

// Include standard library
#include <memory>

// Include dependencies
#include <GLM/glm/glm.hpp>

// Include boomerang libraries
#include "world.hpp"
#include "../physics/rigidbody.hpp"
#include "../graphics/texture.hpp"

namespace Boomerang::Core::World {

    class Node {

        /// Represents a node in the world grid.

    public:

        Node(const glm::vec3& _position, const glm::vec2& _size, const glm::vec2& _scale);
        ~Node();

        // Setters
        void SetPosition(const glm::vec3& _position);
        void SetSize(const glm::vec2& _size);
        void SetScale(const glm::vec2& _scale);
        void SetTexture(const std::shared_ptr<Boomerang::Core::Graphics::Texture>& _texture);
        void SetBiome(const BIOME _biome, const std::shared_ptr<Boomerang::Core::Graphics::Texture>& _texture);
        
        // Getters
        const glm::vec3& GetPosition() const;
        const glm::vec3& GetTruePosition() const;
        const glm::vec2& GetSize() const;
        const glm::vec2& GetScale() const;
        const std::shared_ptr<Boomerang::Core::Graphics::Texture> GetTexture() const;
        const glm::mat4& GetTransform() const;
        bool InFrame(const glm::vec3& _position, const glm::vec2& _windowSize);

        friend std::ostream& operator<< (std::ostream& stream, const Node& node);

    private:

        void UpdateTransform();

        glm::vec3 position;     // Node's position on the grid in grid coords
        glm::vec2 size;
        glm::vec2 scale;

        std::shared_ptr<Boomerang::Core::Graphics::Texture> texture;
        std::shared_ptr<Boomerang::Core::Physics::Rigidbody> rigidbody;

        glm::mat4 transform;

        BIOME biome;
    };
}

#endif // !BOOMERANG_ENGINE_WORLD_NODE
