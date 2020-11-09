# Project Boomerang : scripts/nclass.py (c) 2020 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

# Modified MIT License
# 
# Copyright 2020 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan
# 
#  * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
#  * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
#  * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
#  *
#  * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#  *
#  * Restrictions:
#  >  The Software may not be sold unless significant, mechanics changing modifications are made by the seller, or unless the buyer
#  >  understands an unmodified version of the Software is available elsewhere free of charge, and agrees to buy the Software given
#  >  this knowledge.
#  *
#  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
#  * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
#  * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
#  * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# nclass.py creates new .cpp/.hpp class pair with proper copyright notice, license, pragma-header guard, namespace, and classname
# version _ 11/08/2020 @ 10:55 PM

className = input('Enter class name: ')
description = input('Enter class description: ')
path = input('Enter path: root/')

if path[-1] != '/':
    path += '/'

rpath = '../root/' + path

phpp = rpath + className + '.hpp'
pcpp = rpath + className + '.cpp'

# fixed class name
fixName = className[0].upper() + className[1:]

copyhead_hpp = f"""// Project Boomerang : {path}{className}.hpp (c) 2020 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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
*/"""

copyhead_cpp = f"""// Project Boomerang : {path}{className}.cpp (c) 2020 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

#include "{className}.hpp"
"""

GUARD_SECTIONS = path.upper().split('/')
DEFINE_SECTIONS = ''

for section in GUARD_SECTIONS:
    DEFINE_SECTIONS += '_' + section

namespace = input("Enter namespace: Boomerang::")

PRAGMA_GUARD = f"""#pragma once

#ifndef BOOMERANG{DEFINE_SECTIONS}{className.upper()}
#define BOOMERANG{DEFINE_SECTIONS}{className.upper()}

namespace Boomerang::{namespace} {{

    class {fixName} {{

        /// {description}

    public:

        {fixName}() = default;
        ~{fixName}();

    private:

    }};
}}

#endif // !BOOMERANG{DEFINE_SECTIONS}{className.upper()}
"""

with open(phpp, 'w') as file:
    file.write(copyhead_hpp + '\n\n' + PRAGMA_GUARD)

with open(pcpp, 'w') as file:
    file.write(copyhead_cpp)
