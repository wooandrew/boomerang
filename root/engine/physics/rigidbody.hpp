// Project Boomerang : engine/physics/rigidbody.hpp (c) 2020 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

#ifndef BOOMERANG_ENGINE_PHYSICS_RIGIDBODY
#define BOOMERANG_ENGINE_PHYSICS_RIGIDBODY

// Include dependencies
#include <GLM/glm/glm.hpp>

namespace Boomerang::Core::Physics {

    class Rigidbody {

        /// Object rigidbody

    public:

        // Constructors
        Rigidbody() = default;
        Rigidbody(const glm::vec3& _position, const glm::vec2& _size, const glm::vec2& _scale = glm::vec2(1.f));

        // Destructor
        ~Rigidbody();

        void update(const glm::vec3& _position, float _rotation);
        
        // Setters
        void SetScale(const glm::vec2& _scale);
        void SetPosition(const glm::vec3& _position);
        void AdvancePositionBy(const glm::vec3& _advance);

        // Getters
        const glm::vec3& GetPosition() const;
        const glm::vec2& GetSize() const;
        const glm::vec2& GetScale() const;

        // Debug functions
        void SetDisplayMode(bool _display);


    private:

        // ***** Variables to be used by collision system *****
        friend class Collision;

        float x;        // x position
        float y;        // y position
        float w;        // width
        float h;        // height

        glm::vec3 UpperLeftVertex;
        glm::vec3 LowerLeftVertex;
        glm::vec3 UpperRightVertex;
        glm::vec3 LowerRightVertex;
        // ***** Variables to be used by collision system *****

        void UpdateVertices();

        float LastRotation;     // in degrees

        glm::vec3 position;     // centered at texture midpoint
        glm::vec2 size;
        glm::vec2 scale;

        // Debug Mode
        bool display;
    };
}

#endif // !BOOMERANG_ENGINE_PHYSICS_RIGIDBODY
