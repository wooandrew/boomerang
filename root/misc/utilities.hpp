// Project Boomerang : misc/utilties.hpp (c) 2020 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

#ifndef BOOMERANG_MISC_UTILITIES
#define BOOMERANG_MISC_UTILITIES

#include <string>
#include <chrono>
#include <fstream>
#include <type_traits>

namespace Boomerang::Misc::Utilities {

    template<typename T> T VariadicAdd(T value) {
        return value;
    }
    template<typename T, typename...Args> T VariadicAdd(T value, Args...args) {
        return value + VariadicAdd<T>(args...);
    }

    template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type> struct dimen2d {

        dimen2d() : x(0), y(0) { }
        dimen2d(T _u) : x(_u), y(_u) { }
        dimen2d(T _x, T _y) : x(_x), y(_y) { }
        T x;
        T y;
    };

    std::string ReadFile(const std::string& _path, std::ios::_Openmode _openMode);

    std::string GetDateTime(std::string format = "%Y%m%d _ %T");

    struct DeltaTime {

        DeltaTime();
        void update(bool reset = false);
        double dt();

    private:

        std::chrono::time_point<std::chrono::steady_clock> start;
        std::chrono::time_point<std::chrono::steady_clock> stop;

        double deltaTime;
        double lastTime;

        bool firstCall;
    };
}

namespace util = Boomerang::Misc::Utilities;

#endif // !BOOMERANG_MISC_UTILITIES
