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
#include <bitset>
#include <vector>

#include "windows.h"
#include "psapi.h"

struct SetHash {

    SetHash() = default;
    SetHash(int _x, int _y) {

        x = std::bitset<32>(_x);
        y = std::bitset<32>(_y);

        xy = std::bitset<64>(0);

        for (int i = 0; i < xy.size(); i++) {

            if (i < x.size() && x[i])
                xy.set(i);

            if (i >= y.size() && y[i - y.size()])
                xy.set(i);
        }

        xy = xy >> 32 | xy << 32;
    };

    std::bitset<32> x;
    std::bitset<32> y;
    std::bitset<64> xy;

    friend bool operator< (const SetHash& left, const SetHash& right) {
        return left.xy.to_ullong() < right.xy.to_ullong();
    };

    friend std::ostream& operator<< (std::ostream& stream, const SetHash& sh) {

        std::vector<unsigned char> v = Bitset2Bytes<64>(sh.xy);

        for (int x = 0; x < v.size(); x++)
            stream << v[x];

        return stream;
    };
};

template<size_t N> std::vector<unsigned char> Bitset2Bytes(const std::bitset<N>& bs) {

    std::vector<unsigned char> result((N + 7) >> 3);

    for (int j = 0; j < static_cast<int>(N); j++)
        result[j >> 3] |= (bs[j] << (j & 7));

    return result;
}

template<size_t N> std::bitset<N> Bytes2Bitset(const std::vector<unsigned char>& buf) {

    assert(buf.size() == ((N + 7) >> 3));
    std::bitset<N> result;

    for (int j = 0; j<int(N); j++)
        result[j] = ((buf[j >> 3] >> (j & 7)) & 1);

    return result;
}

int main() {

    std::vector<SetHash> sh;

    // Intervals from x: -10,000 : 10,000
    //                y: -10,000 : 10,000

    // Checking for duplicates
    std::cout << "Hashing set from >>\n x: -100 : 100\n y: -100 : 100" << std::endl;
    for (int x = -100; x < 100; x++) {
        for (int y = -100; y < 100; y++) {
            sh.push_back(SetHash(x, y));
        }
    }

    std::cout << "\nRunning hash collision test..." << std::endl;
    for (int i = 0; i < sh.size(); i++) {
        for (int j = 0; j < sh.size(); j++) {

            if (sh[i].xy == sh[j].xy && i != j) {
                std::cout << (int)sh[i].x.to_ullong() << (int)sh[i].y.to_ullong() << std::endl;
            }
        }
    }

    std::cout << "\nOutput data: "
              << "\n element count: " << sh.size();
    std::cout << "\n binary: ";

    for (int i = 0; i < sh.size(); i++) {
        std::cout << sh[i];
    }


    // Memory diagnostics
    std::cout << "\nRunning memory diagnostics..." << std::endl;

    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;

    std::cout <<  "mem: " << virtualMemUsedByMe / 1000000 << " megabytes";

    Sleep(10000);

    return 0;
}
