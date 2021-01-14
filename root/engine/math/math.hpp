// Project Boomerang : engine/math/math.hpp (c) 2020-2021 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

#ifndef BOOMERANG_ENGINE_MATH_MATH
#define BOOMERANG_ENGINE_MATH_MATH

// Include dependencies
#include <GLM/glm/glm.hpp>
#include <GLM/glm/gtc/constants.hpp>

namespace Boomerang::Core::Math {

    enum class AngleType {
        DEGREES,
        RADIANS
    };

    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
    T ConvertToRadians(const T degrees) {
        return (degrees * glm::pi<T>()) / static_cast<T>(180);
    }

    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
    T ConvertToDegrees(const T radians) {
        return (radians * static_cast<T>(180)) / glm::pi<T>();
    }

    const glm::vec3& RotatePoint(glm::vec3& point, const glm::vec3& pivot, float rotation, AngleType type = AngleType::DEGREES);
}

#endif // !BOOMERANG_ENGINE_MATH_MATH
