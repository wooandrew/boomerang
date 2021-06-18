// Project Boomerang : perlin.cpp (c) 2020-2021 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

// Perlin Noise unit test

#include <iostream>
#include <functional>
#include <numbers>
#include <random>
#include <vector>
#include <chrono>

const float SmoothNoise2D(float x, float y, std::function<float(int, int)> Noise2D) {

    float corners = (Noise2D(x - 1, y - 1) + Noise2D(x + 1, y - 1) + Noise2D(x - 1, y + 1) + Noise2D(x + 1, y + 1)) / 16.f;
    float sides = (Noise2D(x - 1, y) + Noise2D(x + 1, y) + Noise2D(x, y - 1) + Noise2D(x, y + 1)) / 8.f;
    float center = Noise2D(x, y) / 4.f;

    return corners + sides + center;
}

const float Interpolate(float a, float b, float x) {

    float f = (1.0f - std::cos(x * std::numbers::pi)) * 0.5f;
    return a * (1.0f - f) + b * f;
}

const float Interpolate2D(float x, float y, std::function<float(int, int)> Noise2D) {

    int xi = static_cast<int>(x);   // Integer x
    int yi = static_cast<int>(y);   // Integer y

    float xf = x - xi;  // Fractional x
    float yf = y - yi;  // Fractional y

    float v1 = SmoothNoise2D(xi, yi, Noise2D);
    float v2 = SmoothNoise2D(xi + 1, yi, Noise2D);
    float v3 = SmoothNoise2D(xi, yi + 1, Noise2D);
    float v4 = SmoothNoise2D(xi + 1, yi + 1, Noise2D);

    float i1 = Interpolate(v1, v2, xf);
    float i2 = Interpolate(v3, v4, xf);

    return Interpolate(i1, i2, yf);
}

const float Perlin2D(const float x, const float y, const float persistence, const float offset,
    const std::vector<std::function<float(int, int)>>& Noises) {

    float total = 0;

    for (int i = 0; i < Noises.size() - 1; i++) {

        float frequency = std::pow(2, i);
        float amplitude = std::pow(persistence, i);

        total += Interpolate2D(x * frequency, y * frequency, Noises[i]) * amplitude;
    }

    return total += offset;
}

const int IsPrime(unsigned long i) {

    long si;

    si = std::sqrt(i);

    for (long j = 2; j <= si; j++) {

        if (i % j == 0)
            return 0;
    }

    return 1;
}

const std::vector<int> GenPrime(unsigned long lo, unsigned long hi) {

    std::vector<int> loHiPrimes;

    for (long i = lo; i < hi; i++) {
        if (IsPrime(i))
            loHiPrimes.push_back(i);
    }

    return loHiPrimes;
}

std::vector<std::function<float(int, int)>> NoiseFactory(std::mt19937_64& mte, int n) {

    std::vector<std::function<float(int, int)>> Noises;

    unsigned long cRangeLo = mte() % 10000000000;
    unsigned long cRangeHi = cRangeLo + (mte() % (cRangeLo / 1000));

    std::vector<int> aPrimes = GenPrime(10000, 99999);
    std::vector<int> bPrimes = GenPrime(100000, 999999);
    std::vector<int> cPrimes = GenPrime(cRangeLo, cRangeHi);

    for (int i = 0; i < n; i++) {

        int a = aPrimes[mte() % aPrimes.size()];
        int b = bPrimes[mte() % bPrimes.size()];
        int c = cPrimes[mte() % cPrimes.size()];

        std::function<float(int, int)> nt = [=](int x, int y) {

            int q = x + y * 57;
            q = (q << 13) ^ q;

            return (1.0 - ((q * (q * q * a + b) + c) & 0x7fffffff) / 1073741824.0);
        };

        Noises.push_back(nt);
    }

    return Noises;
}


int main() {

    std::mt19937_64 mte(std::chrono::system_clock::now().time_since_epoch().count());

    std::vector<std::function<float(int, int)>> n1 = NoiseFactory(mte, 16);
    std::vector<std::function<float(int, int)>> n2 = NoiseFactory(mte, 9);
    std::vector<std::function<float(int, int)>> n3 = NoiseFactory(mte, 6);

    float max = 0.0f;
    float min = 0.0f;

    for (int x = -10000; x < 10000; x++) {
        for (int y = -10000; y < 10000; y++) {
            float t = Perlin2D(x, y, 0.25, 0.0f, n1);
            max = t > max ? t : max;
            min = t < min ? t : min;
        }
    }

    std::cout << "n1: " << max << " , " << min << std::endl;

    max = 0.0f;
    min = 0.0f;

    for (int x = -10000; x < 10000; x++) {
        for (int y = -10000; y < 10000; y++) {
            float t = Perlin2D(x, y, 0.25, 0.0f, n2);
            max = t > max ? t : max;
            min = t < min ? t : min;
        }
    }

    std::cout << "n2: " << max << " , " << min << std::endl;

    max = 0.0f;
    min = 0.0f;

    for (int x = -10000; x < 10000; x++) {
        for (int y = -10000; y < 10000; y++) {
            float t = Perlin2D(x, y, 0.25, 0.0f, n3);
            max = t > max ? t : max;
            min = t < min ? t : min;
        }
    }

    std::cout << "n3: " << max << " , " << min << std::endl;

    return 0;
}
