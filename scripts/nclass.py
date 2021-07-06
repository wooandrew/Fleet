# Fleet : scripts/nclass.py (c) 2021 Andrew Woo
# Modified MIT License
# 
# Copyright 2021 Andrew Woo
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
# version _ 07/05/2021 @ 03:45 PM

className = input('Enter class name: ').lower()
description = input('Enter class description: ')
path = input('Enter path: root/')
namespace = input("Enter namespace: Fleet::")

if path[-1] != '/':
    path += '/'

rpath = '../root/' + path

phpp = rpath + className + '.hpp'
pcpp = rpath + className + '.cpp'

# fixed class name
fixName = className[0].upper() + className[1:]

copyhead_hpp = f"""// Fleet : {path}{className}.hpp (c) 2021 Andrew Woo

/* Modified MIT License
 *
 * Copyright 2021 Andrew Woo
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

copyhead_cpp = f"""// Fleet : {path}{className}.cpp (c) 2021 Andrew Woo

/* Modified MIT License
 *
 * Copyright 2021 Andrew Woo
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

namespace Fleet::{namespace} {{

}}
"""

GUARD_SECTIONS = path.upper().split('/')
DEFINE_SECTIONS = ''

for section in GUARD_SECTIONS:
    DEFINE_SECTIONS += '_' + section

PRAGMA_GUARD = f"""#pragma once

#ifndef FLEET{DEFINE_SECTIONS}{className.upper()}
#define FLEET{DEFINE_SECTIONS}{className.upper()}

namespace Fleet::{namespace} {{

    class {fixName} {{

        /// {description}

    public:

        {fixName}() = default;
        ~{fixName}();

    private:

    }};
}}

#endif // !FLEET{DEFINE_SECTIONS}{className.upper()}
"""

with open(phpp, 'w') as file:
    file.write(copyhead_hpp + '\n\n' + PRAGMA_GUARD)

with open(pcpp, 'w') as file:
    file.write(copyhead_cpp)