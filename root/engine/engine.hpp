// Fleet : engine/engine.hpp (c) 2021 Andrew Woo

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

#ifndef FLEET_ENGINE_ENGINE
#define FLEET_ENGINE_ENGINE

// Include standard library
#include <string>
#include <vector>

// Include dependencies
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm/glm.hpp>
#include <ASWL/utilities.hpp>

namespace Fleet::Core {

    class Engine {

        /// Fleet Engine

    public:

        // Constructor
        Engine(const std::string& _WindowTitle = "Fleet", const glm::vec2& _WindowDimensions = glm::vec2(1000, 618));

        // Destructor
        ~Engine();

        // Initialize Engine
        int init();

        // Change metadata later to settings
        struct Metadata {       // This struct contains the metadata for the engine/glfwWindow. The data inside should be set BEFORE the engine is initialized.

            // Engine metadata
            bool autoinit = false;              // Automatically initialize engine components
            bool debugmode = false;             // Enable debug mode
            bool loggingEnabled = true;         // Enable logging
            bool enableVSync = false;           // Enable VSync

            enum class VideoMode {
                WINDOWED,
                FULLSCREEN,
                WINDOWED_FULLSCREEN
            }; VideoMode vidmode = VideoMode::WINDOWED_FULLSCREEN;

            std::vector<float> clearcolor = { 1.f, 1.f, 1.f, 0.f };     // Render surface clear color
            //RendererType rendermode = RendererType::Render_2D;        // Engine render mode

            //Version version;                                // Application version

            // GLFW window initialization metadata
            std::vector<std::pair<int, int>> windowHints { { std::make_pair(GLFW_CLIENT_API, GLFW_OPENGL_API),      // Window hints _ default (GLFW_CLIENT_API, GLFW_OPENGL_API)
                                                             std::make_pair(GLFW_RESIZABLE, GLFW_TRUE),             // Window hints _ default (GLFW_RESIZABLE, GLFW_FALSE)
                                                             std::make_pair(GLFW_SAMPLES, 4) } };                   // Window hints _ default (GLFW_SAMPLES, 4)

        }; Metadata metadata;

        // Engine Functions
        void update() const;            // Update engine

        // Getters
        GLFWwindow* GetWindow();
        const glm::vec2& GetWindowDimensions() const;
        const glm::vec2& GetFramebufferDimensions() const;
        
        const int GetMaxTextureUnits() const;

    private:

        void SetWindowSize(int _width, int _height);

        GLFWwindow* window;

        std::string WindowTitle;
        glm::vec2 WindowDimensions;
        glm::vec2 FramebufferDimensions;

        int MaxTextureUnits;
    };
}

#endif // !FLEET_ENGINE_ENGINE
