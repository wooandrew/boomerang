// Project Boomerang : misc/utilties.cpp (c) 2020 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

#pragma warning(disable : 4996)

#include "utilities.hpp"

#include <ctime>
#include <chrono>
#include <sstream>
#include <iomanip>

#include "logger.hpp"

namespace Boomerang::Misc::Utilities {

    std::string ReadFile(const std::string& _path, std::ios::_Openmode _openMode) {

        std::string data = "";

        std::ifstream input(_path, _openMode);
        if (input) {

            input.seekg(0, std::ios::end);
            data.resize(input.tellg());

            input.seekg(0, std::ios::beg);
            input.read(&data[0], data.size());
            input.close();
        }
        else
            logger::logger<std::string, std::string>("     ", "Error reading file at ", _path);

        return data;
    }

    std::string GetDateTime(std::string format) {

        auto timeNow = std::chrono::system_clock::now();
        time_t timeNow_t = std::chrono::system_clock::to_time_t(timeNow);

        std::ostringstream oss;

        if (format == "ctd")
            oss << std::ctime(&timeNow_t);
        else
            oss << std::put_time(std::localtime(&timeNow_t), format.c_str());

        return oss.str();
    }

    DeltaTime::DeltaTime() {

        std::chrono::high_resolution_clock dtimer;

        start = dtimer.now();
        stop = dtimer.now();

        deltaTime = 0;
        lastTime = 0;
        firstCall = true;
    }

    void DeltaTime::update(bool reset) {

        std::chrono::high_resolution_clock dtimer;

        if (firstCall || reset) {
            start = dtimer.now();
            firstCall = false;
        }

        stop = dtimer.now();

        deltaTime = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(stop - start).count() / 1000;

        start = stop;
    }

    double DeltaTime::dt() {
        return deltaTime;
    }
}
