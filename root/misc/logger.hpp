// Project Boomerang : misc/logger.hpp (c) 2020 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

#ifndef BOOMERANG_MISC_LOGGER
#define BOOMERANG_MISC_LOGGER

#include <iostream>
#include <mutex>
#include <ostream>
#include <fstream>

#include "utilities.hpp"

namespace Boomerang::Misc::Logger {

    // private namespace
    namespace {

        static std::streambuf* stream_buffer_clog_default__ = std::clog.rdbuf();

        //static bool silenced = false;
        static std::mutex mu;
    }

    void SetLogStream();
    void SetLogStream(std::fstream& file);

    template<typename ERRNUM, typename...ERRMSG> void logger(ERRNUM errnum, ERRMSG...errmsg) {

        std::lock_guard<std::mutex> lock(mu);
        std::clog << Boomerang::Misc::Utilities::GetDateTime() << " |" << errnum << "| " << Boomerang::Misc::Utilities::VariadicAdd(errmsg...) << std::endl;
    }

    template<typename ERRNUM, typename ERRMSG> void logger(ERRNUM errnum, ERRMSG errmsg) {

        std::lock_guard<std::mutex> lock(mu);
        std::clog << Boomerang::Misc::Utilities::GetDateTime() << " |" << errnum << "| " << errmsg << std::endl;
    }
}

namespace logger = Boomerang::Misc::Logger;

#endif // !BOOMERANG_MISC_LOGGER
