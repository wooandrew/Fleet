// Fleet : engine/input/keyboard.hpp (c) 2021 Andrew Woo

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

#pragma once

#ifndef FLEET_ENGINE_INPUT_KEYBOARD
#define FLEET_ENGINE_INPUT_KEYBOARD

#include <GLFW/glfw3.h>

namespace Fleet::Core::Input {

    class Keyboard {

        /// Callback to hardware: keyboard

    public:

        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers);

        static bool KeyDown(int key);
        static bool KeyUp(int key);
        static bool KeyIsPressed(int key);

    private:

        static bool Keys[];
        static bool KeysDown[];
        static bool KeysUp[];
    };
}

#endif // !FLEET_ENGINE_INPUT_KEYBOARD
