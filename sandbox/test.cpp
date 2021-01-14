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
#include <fstream>
#include <sstream>
#include <utility>
#include <vector>
#include <string>
#include <bitset>

class Serialize {

public:

    template<typename T> T VariadicTypes(T arg) {
        return arg;
    }
    template<typename... Ts> int VariadicTypes(std::string path, Ts&& ... ts) {
        
        try {

            std::ofstream file(path, std::ios::binary);
            ((file << std::forward<Ts>(ts) << std::endl), ...);
            file.close();
        }
        catch (std::exception& e) {
            return 1;
        }

        return 0;
    }
    virtual void write() {

        // std::ofstream file("out.txt", std::ios::binary);
        // file << id << '\n' << name;
        // file.close();
        VariadicTypes("out.txt", id, name, value);
    }

private:

    int id = 3;
    std::string name = "Hello World";
    float value = 3.14159f;
};

template<typename ERRNUM, typename...ERRMSG> void Logger(ERRNUM num, ERRMSG&&... msgs) {

    std::clog << "STUFF HERE" << " |" << num << '|';
    ((std::clog << ' ' << std::forward<ERRMSG>(msgs)), ...) << std::endl;
}

  
int main() {

    Serialize ser;
    
    ser.write();

    //Logger("0x000", "Hello", "World", "Loser");
    //Logger("Hello", "Hello World");

    std::string s = "Hello World";
    std::vector<std::bitset<256>> sD;

    for (std::string::iterator x = s.begin(); x != s.end(); x++) {
        std::bitset<256> v = *x;
        std::cout << char(v.to_ulong()) << std::endl;
        sD.push_back(v << 2);
    }

    //std::ofstream out("out.bin", std::ios::binary);
    std::vector<char> g;

    for (const std::bitset<256>& val : sD)
        g.push_back(static_cast<unsigned char>(val.to_ulong()));

    for (char x : g) {
        std::bitset<256> v = x;
        std::cout << (v >> 2);
    }

    return 0;
}
