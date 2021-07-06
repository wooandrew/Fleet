// Fleet : engine/manager.cpp (c) 2021 Andrew Woo

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

#include "manager.hpp"

// Include standard library
#include <atomic>
#include <chrono>
#include <thread>
#include <sstream>
#include <iomanip>

// Include dependencies
#include <GLM/glm/gtc/matrix_transform.hpp>
#include <ASWL/logger.hpp>

// Include boomerang libraries
#include "input/mouse.hpp"
#include "input/keyboard.hpp"
#include "graphics/manager.hpp"
#include "graphics/renderer.hpp"

namespace Fleet::Core {

    Manager::Manager() {

        DefaultCameraOrtho = glm::ortho(-500, 500, -309, 309);
        state = GAME_STATE::RUN;
    }

    Manager::~Manager() {
        state = GAME_STATE::STOP;
        //shutdown();
    }

    int Manager::init() {

        // Initialize the game engine
        if (engine.init() != 0) {
            ASWL::Logger::logger("  E  ", "Fatal Error: Failed to initialize game engine.");
            return -1;
        }
        else
            ASWL::Logger::logger("  E  ", "Engine initialization success. All systems go!");

        // Initialize mouse with origin
        Input::Mouse::init({ engine.GetWindowDimensions().x / 2, engine.GetWindowDimensions().y / 2 });

        // Initialize graphics manager
        Graphics::Manager::init();
        Graphics::Manager::SetClearColor({ 85.f / 255.f, 204.f / 255.f, 212.f / 255.f, 1.f });

        // Initialize 2d renderer
        Graphics::Renderer::init(engine.GetWindowDimensions(), engine.GetMaxTextureUnits());

        // Set default camera ortho to fit window dimensions
        DefaultCameraOrtho = glm::ortho(-engine.GetWindowDimensions().x / 2.f, engine.GetWindowDimensions().x / 2.f,
                                        -engine.GetWindowDimensions().y / 2.f, engine.GetWindowDimensions().y / 2.f);

        auto NormalizedCameraOrtho = glm::ortho(-1, 1, -1, 1);

        // Create default cameras (main, text, debug/grid)
        cameras.insert({ "main_0", std::make_unique<Graphics::OrthoCam>(DefaultCameraOrtho, 500.f) });
        cameras.insert({ "grid_0", std::make_unique<Graphics::OrthoCam>(DefaultCameraOrtho, 500.f) });
        cameras.insert({ "text_0", std::make_unique<Graphics::OrthoCam>(DefaultCameraOrtho, 500.f) });

        // Lock all cameras initially
        for (auto const& [key, val] : cameras)
            val->SetLock(true);

        // Create default fonts
        FontLibrary.insert({ "nsjpl", std::make_unique<Graphics::FontLibrary>("nsjpl", "assets/fonts/nsjpl.otf") });
        FontLibrary["nsjpl"]->AddSize(32);
        FontLibrary["nsjpl"]->AddSize(56);

        _fps.Update();

        return 0;
    }

    void Manager::shutdown() {
        Graphics::Renderer::shutdown();
        Graphics::Manager::shutdown();
    }

    const bool Manager::run() {
        return !glfwWindowShouldClose(engine.GetWindow()) && state == GAME_STATE::RUN;
    }

    void Manager::update() {

        if (Fleet::Core::Input::Keyboard::KeyIsPressed(GLFW_KEY_ESCAPE))        // QUIT
            state = Fleet::Core::Manager::GAME_STATE::STOP;

        // Update clocks
        DeltaTime.UpdateDeltaTime();
        _fps.Update();
        
        // Update engine (poll events)
        engine.update();

        // Update cameras
        for (auto const& [key, val] : cameras) {
            if (!val->locked())
                val->update(dt());
        }
    }

    const float Manager::dt() {
        return static_cast<float>(DeltaTime.GetDeltaTime());
    }
    const float Manager::ft() {
        return static_cast<float>(_fps.GetFT());
    }
    const float Manager::fps() {
        return static_cast<float>(_fps.GetFPS());
    }

    std::string Manager::ft_str() {
        
        std::stringstream oss;

        oss << std::fixed << std::setprecision(2) << _fps.GetFT();

        return oss.str() + "ms";
    }

    const std::unique_ptr<Graphics::OrthoCam>& Manager::GetCamera(const std::string& _name) {
        return cameras[_name];
    }
    const std::shared_ptr<Graphics::Font>& Manager::GetFont(const std::string & _name, int _size) {
        return FontLibrary[_name]->GetFont(_size);
    }

    const glm::vec2& Manager::GetWindowDimensions() const {
        return engine.GetWindowDimensions();
    }
    GLFWwindow* Manager::GetWindow() {
        return engine.GetWindow();
    }
}
