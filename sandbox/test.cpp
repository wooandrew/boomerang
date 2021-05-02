// Project Boomerang : test.cpp (c) 2020-2021 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

// Modified MIT License
// 
// Copyright 2020-2021 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan
// 
//  * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
//  * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//  *
//  * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//  *
//  * Restrictions:
//  >  The Software may not be sold unless significant, mechanics changing modifications are made by the seller, or unless the buyer
//  >  understands an unmodified version of the Software is available elsewhere free of charge, and agrees to buy the Software given
//  >  this knowledge.
//  *
//  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
//  * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
//  * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#include <iostream>
#include <cmath>

struct vec3 {

    vec3() {
        x = 0;
        y = 0;
        z = 0;
    }

    vec3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    float x;
    float y;
    float z;
};

struct vec2 {

    vec2() {
        x = 0;
        y = 0;
    }

    vec2(float x, float y) {
        this->x = x;
        this->y = y;
    }

    float x;
    float y;
};


vec3 PixelToGridCoord(const vec3& _PixlCoord, const float _CellSize) {

    vec3 ret;

    ret.x = std::round(_PixlCoord.x / _CellSize);
    ret.y = std::round(_PixlCoord.y / _CellSize);
    ret.z = _PixlCoord.z;

    return ret;
}

void test(const vec3& _position, const vec2& _windowSize) {

    // Return pixel position as grid position
    vec3 gridCoord = PixelToGridCoord(_position, 40.f);

    // Calculate xMax, xMin, yMax, yMin in terms of grid coords.
    float xMax = std::round((_position.x + _windowSize.x / 2) / 40.f);
    float xMin = std::round((_position.x - _windowSize.x / 2) / 40.f) + 1.f;        // +1 to normalize negative x
    float yMax = std::round((_position.y + _windowSize.y / 2) / 40.f) - 1.f;        // -1 to normalize positive y
    float yMin = std::round((_position.y - _windowSize.y / 2) / 40.f);
    std::cout << "xMax: " << xMax << "\nxMin: " << xMin << "\nyMax: " << yMax << "\nyMin: " << yMin << std::endl;

    // Calculate which chunk coord xMax, xMin, yMax, yMin are in
    float xMaxCC = std::round(xMax / 8) + 1;
    float xMinCC = std::round(xMin / 8) - 1;
    float yMaxCC = std::round(yMax / 8) + 1;
    float yMinCC = std::round(yMin / 8) - 1;
    std::cout << "\nxMaxCC: " << xMaxCC << "\nxMinCC: " << xMinCC << "\nyMaxCC: " << yMaxCC << "\nyMinCC: " << yMinCC << '\n' << std::endl;

    
    //<< "\nChunks to generate:"

    // Calculate chunks to generate
    for (int y = yMinCC * 8; y <= yMaxCC * 8; y += 8) {
        
        for (int x = xMinCC * 8; x <= xMaxCC * 8; x += 8) {
            std::cout << " [" << x << ", " << y << "],\t";
        }

        std::cout << '\n';
    }
}

int main() {

    vec2 w(1920.f, 1080.f);
    vec3 p(0.f, 0.f, 0.f);

    test(p, w);

    return 0;
}
