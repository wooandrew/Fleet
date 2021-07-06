// Fleet : main.cpp (c) 2021 Andrew Woo

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

#include <iostream>

// Engine
#include "engine/engine.hpp"
#include "engine/manager.hpp"

#include "engine/graphics/manager.hpp"
#include "engine/graphics/renderer.hpp"

// Objects
#include "objects/flagship.hpp"

// Dependencies
#include <ASWL/logger.hpp>

using namespace Fleet::Core::Graphics::Renderer::RENDER_LAYER;

int main(int argc, char* argv[]) {

    ASWL::Logger::logger("     ", "Hello, Fleet!");

    // Initialize Engine components
    Fleet::Core::Manager manager;

    int ret = manager.init();
    if (ret != 0) {
        ASWL::Logger::logger("  E  ", "FATAL ERROR: Failed to initialize game manager.");
        return ret;
    }

    std::shared_ptr<Fleet::Core::Graphics::Texture> tFlagship = std::make_shared<Fleet::Core::Graphics::Texture>("assets/boat1.png");
    Fleet::Objects::Flagship flagship( { 0.f, 0.f, 0.f }, { 0.5f, 0.5f }, glm::vec4(1.f), tFlagship);

    manager.GetCamera("main_0")->SetLock(false);
    manager.GetCamera("main_0")->SetSpeed(0);

    while (manager.run()) {

        manager.update();
        manager.GetCamera("main_0")->SetPosition(flagship.GetPosition());

        flagship.update(manager.dt());

        Fleet::Core::Graphics::Manager::BeginRender();

        Fleet::Core::Graphics::Renderer::StartScene(manager.GetCamera("main_0"));
        Fleet::Core::Graphics::Renderer::RenderTexture({ flagship.GetPosition(), flagship.GetSize(), glm::vec4(1.f), flagship.GetRotation() }, tFlagship);
        Fleet::Core::Graphics::Renderer::EndScene();

        Fleet::Core::Graphics::Renderer::StartScene(manager.GetCamera("grid_0"), "grid");
        Fleet::Core::Graphics::Renderer::RenderGrid(manager.GetCamera("main_0")->GetPosition(), 40);
        Fleet::Core::Graphics::Renderer::EndScene();
        
        Fleet::Core::Graphics::Renderer::StartScene(manager.GetCamera("text_0"), "text");
        Fleet::Core::Graphics::Renderer::RenderText(Fleet::Core::BUILD_VERSION, { { 0, 290, LAYER1 }, { 1.f, 1.f }, glm::vec4(1.f) }, manager.GetFont("nsjpl", 25));
        Fleet::Core::Graphics::Renderer::RenderText(manager.ft_str(), { { 820, 520, LAYER1 }, { 1.f, 1.f }, {0.f, 1.f, 0.f, 1.f} }, manager.GetFont("nsjpl", 32));
        Fleet::Core::Graphics::Renderer::RenderText(std::to_string((int)manager.fps()), { { 920, 520, LAYER1 }, { 1.f, 1.f }, {0.f, 1.f, 0.f, 1.f} }, manager.GetFont("nsjpl", 32));
        Fleet::Core::Graphics::Renderer::EndScene();

        Fleet::Core::Graphics::Manager::EndRender(manager.GetWindow());
    }

    manager.shutdown();

    return 0;
}
