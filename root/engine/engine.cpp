// Fleet : engine/engine.cpp (c) 2021 Andrew Woo

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

#include "engine.hpp"

#include "input/mouse.hpp"
#include "input/keyboard.hpp"

#include <ASWL/logger.hpp>

namespace Fleet::Core {

    // Define default engine constructor
    Engine::Engine(const std::string& _WindowTitle, const glm::vec2& _WindowDimensions) {

        WindowTitle = _WindowTitle;
        WindowDimensions = _WindowDimensions;
        FramebufferDimensions = glm::vec2(0);

        if (metadata.autoinit)
            init();
    }
    // Define defalt engine destructor
    Engine::~Engine() {
        glfwDestroyWindow(window);
    }

    // Initialize engine
    int Engine::init() {

        // Initialize GLFW
        if (!glfwInit()) {
            ASWL::Logger::logger("E0001", "Fatal Error: Failed to initialize GLFW -> !glfwInit().");
            return 1;
        }

        // Set the window hints using engine metadata. Defaulted to (GLFW_CLIENT_API, GLFW_OPENGL_API), 
        // (GLFW_RESIZABLE, GLFW_FALSE), and (GLFW_SAMPLES, 4).
        for (const auto& hint : metadata.windowHints)
            glfwWindowHint(hint.first, hint.second);

        // Create Window
        window = glfwCreateWindow(WindowDimensions.x, WindowDimensions.y, WindowTitle.c_str(), NULL, NULL);
        if (!window) {
            
            ASWL::Logger::logger("E0002", "Fatal Error: Failed to create window -> !glfwCreateWindow().");
            glfwTerminate();

            return 2;
        }

        // Create Window Context
        glfwMakeContextCurrent(window);

        // Initialize GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {

            ASWL::Logger::logger("E0003", "Fatal Error: Failed to initialize GLAD -> !gladLoadGLLoader().");
            glfwTerminate();

            return 3;
        }

        // Framebuffer initialization
        int width = 0;
        int height = 0;        
        glfwGetFramebufferSize(window, &width, &height);
        FramebufferDimensions = glm::vec2(width, height);
        glfwSwapInterval(metadata.enableVSync);

        // Window Setup
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        int xPos = (mode->width - WindowDimensions.x) / 2;
        int yPos = (mode->height - WindowDimensions.y) / 2;
        glfwSetWindowPos(window, xPos, yPos);

        // Full screen
        if (metadata.vidmode == Metadata::VideoMode::FULLSCREEN) {

            glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
            SetWindowSize(mode->width, mode->height);
            glad_glViewport(0, 0, mode->width, mode->height);
        }
        else if (metadata.vidmode == Metadata::VideoMode::WINDOWED_FULLSCREEN) {

            glfwSetWindowMonitor(window, nullptr, 0, 0, mode->width, mode->height, mode->refreshRate);
            SetWindowSize(mode->width, mode->height);
            glad_glViewport(0, 0, mode->width, mode->height);
        }
        else // To be deprecated
            glfwSetWindowSizeLimits(window, WindowDimensions.x, WindowDimensions.y, WindowDimensions.x, WindowDimensions.y);

        // Input Callback
        glfwSetCursorPosCallback(window, Fleet::Core::Input::Mouse::MousePositionCallback);
        glfwSetMouseButtonCallback(window, Fleet::Core::Input::Mouse::MouseButtonCallback);
        glfwSetKeyCallback(window, Fleet::Core::Input::Keyboard::KeyCallback);

        // Get maximum number of textures that can be bound at once
        glad_glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MaxTextureUnits);

        // Log System Info
        ASWL::Logger::logger(" OGL ", "GL version:", glad_glGetString(GL_VERSION));
        ASWL::Logger::logger(" OGL ", "GPU vendor:", glad_glGetString(GL_VENDOR));

        return 0;
    }

    // Update Engine
    void Engine::update() const {
        glfwPollEvents();
    }

    // Private member functions
    void Engine::SetWindowSize(int _width, int _height) {
        glfwSetWindowSize(window, _width, _height);
        WindowDimensions = { _width, _height };
    }

    // Getters
    GLFWwindow* Engine::GetWindow() {
        return window;
    }

    const glm::vec2& Engine::GetWindowDimensions() const {
        return WindowDimensions;
    }

    const glm::vec2& Engine::GetFramebufferDimensions() const {
        return FramebufferDimensions;
    }

    const int Engine::GetMaxTextureUnits() const {
        return MaxTextureUnits;
    }
}
