// Project Boomerang : engine/graphics/camera/orthocam.hpp (c) 2020 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

#ifndef BOOMERANG_ENGINE_GRAPHICS_CAMERA_ORTHOCAM
#define BOOMERANG_ENGINE_GRAPHICS_CAMERA_ORTHOCAM

// Include dependencies
#include <GLM/glm/glm.hpp>

// Include boomerang libraries
#include "../../../misc/utilities.hpp"

namespace Boomerang::Core::Graphics {

    class OrthoCam {

        /// Orthographic Camera System

    public:

        // Constructors
        OrthoCam() = default;
        OrthoCam(glm::mat4& _ProjectionMat, float _speed = 0);
        
        // Setters
        void SetZoom(float _zoom, const util::dimen2d<int>& windowSize);
        void SetSpeed(float _speed);
        void SetRotation(float _rotation);
        void SetPosition(glm::vec3& _position);

        void SetProjection(glm::mat4& _projection);

        // Getters
        const float GetSpeed() const;
        const glm::vec3& GetPosition() const;

        const glm::mat4& GetViewMatrix() const;
        const glm::mat4& GetProjectionMatrix() const;
        const glm::mat4& GetViewProjectionMatrix() const;

        // dt = delta time
        virtual void update(float dt);

    private:

        void RecalculateMatrix();

        float zoom;
        float speed;
        float rotation;

        glm::vec3 position;

        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;
        glm::mat4 ViewProjectionMatrix;
    };
}

#endif // !BOOMERANG_ENGINE_GRAPHICS_CAMERA_ORTHOCAM
