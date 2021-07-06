// Fleet : engine/input/keyboard.cpp (c) 2021 Andrew Woo

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

#include "keyboard.hpp"

namespace Fleet::Core::Input {

	bool Keyboard::Keys[GLFW_KEY_LAST] = { 0 };
	bool Keyboard::KeysDown[GLFW_KEY_LAST] = { 0 };
	bool Keyboard::KeysUp[GLFW_KEY_LAST] = { 0 };

	void Keyboard::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers) {

		if (key < 0)
			return;

		if ((action != GLFW_RELEASE) && (!Keys[key])) {
			KeysDown[key] = true;
			KeysUp[key] = false;
		}

		if ((action == GLFW_RELEASE) && (Keys[key])) {
			KeysDown[key] = false;
			KeysUp[key] = true;
		}

		Keys[key] = (action != GLFW_RELEASE);
	}

	bool Keyboard::KeyDown(int key) {

		bool down = KeysDown[key];
		KeysDown[key] = false;

		return down;
	}

	bool Keyboard::KeyUp(int key) {

		bool up = KeysUp[key];
		KeysUp[key] = false;

		return up;
	}

	bool Keyboard::KeyIsPressed(int key) {
		return Keys[key];
	}
}
