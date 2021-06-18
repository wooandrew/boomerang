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

int noise(int x, int y) {

    int n = x + y * 31;
    n ^= n >> 2 | n << 2;
    n %= 64;

    //int nX = n % 8;
    //int nY = n / 8;

    return n;
}

int main(int argc, char** args) {

    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 10; y++) {
            noise(x, y);
        }
    }
    
    return 0;
}