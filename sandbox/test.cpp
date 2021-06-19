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
#include <string>
#include <unordered_map>
#include <stdexcept>

// map size limiter
template<typename K, typename T> class UnorderedSizedMap {

public:

    UnorderedSizedMap(int _size) {
        size = static_cast<unsigned int>(_size);
    }
    ~UnorderedSizedMap() { }

    void insert(K key, T type) {

        if (map.size() < size)
            map.insert({ key, type });
        else
            throw std::out_of_range("Map size exceeded!");
    }

    bool insert_or_assign(K key, T type) {
        
        if (map.find(key) != map.end())
            map[key] = type;
        else
            insert(key, type);
    }

    T& operator[](K key) {

        if (map.find(key) != map.end())
            return map[key];
        else {

            if (map.size() < size)
                insert(key, "");
            else
                throw std::out_of_range("Map size exceeded!");
        }
           
        return map[key];
    }

    const std::unordered_map<K, T>& GetMap() const {
        return map;
    }

private:

    std::unordered_map<K, T> map;
    unsigned int size;
};

int main(int argc, char** args) {

    UnorderedSizedMap<int, std::string> usm(2);

    try {
        usm[0] = "Hello World";
        usm[1] = "Hello World";
        usm[2] = "Hello World";
        usm[4] = "Hello World";
        //usm.insert(10, "Hello World");
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    
    for (auto& [key, val] : usm.GetMap())
        std::cout << "Key: " << key << "\tType: " << val << std::endl;

    return 0;
}