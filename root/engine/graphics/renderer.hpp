// Fleet : engine/graphics/renderer.hpp (c) 2021 Andrew Woo

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

#ifndef FLEET_ENGINE_GRAPHICS_RENDERER
#define FLEET_ENGINE_GRAPHICS_RENDERER

// Include standard library
#include <string>
#include <chrono>
#include <memory>

// Include dependencies
#include <GLM/glm/glm.hpp>

// Include Fleet libraries
#include "font.hpp"
#include "texture.hpp"
#include "camera/orthocam.hpp"

namespace Fleet::Core::Graphics::Renderer {

    namespace RENDER_LAYER {

        constexpr float LAYER0 = 0.0f;
        constexpr float LAYER1 = 0.1f;
        constexpr float LAYER2 = 0.2f;
        constexpr float LAYER3 = 0.3f;
        constexpr float LAYER4 = 0.4f;
    }

    struct render_data {
        glm::vec3 position;
        glm::vec2 scale;
        glm::vec4 color;
        float rotation = 0;
    };

    std::vector<glm::vec3> CalculateVertexPositions(const glm::vec3& _position, const glm::vec2& _size);
    std::vector<glm::vec3> RotateVertices(const std::vector<glm::vec3>& _vertices, const glm::vec3& _position, const float _rotation);

    // Renderer Control
    void init(const glm::vec2& _WindowSize, int _MaxTextureUnits = 16);
    void shutdown();

    void SetWindowSize(const glm::vec2& _WindowSize);

    // Add to batch
    void AddQuad(const std::vector<glm::vec3>& _vertices, const glm::vec4& _color, glm::vec2 _TexCoords[4], const float _texslot = 0);

    // Render commands+
    void StartScene(const std::unique_ptr<OrthoCam>& camera, const std::string& _shader = "basic");
    void FlushScene();
    void EndScene();

    // Draw Static Quad
    void DrawQuad(const render_data& _data);

    // Render Texture
    void RenderTexture(const render_data& _data, const std::shared_ptr<Texture>& _texture);

    // Render Text
    void RenderText(const std::string& _string, const render_data& _data, const std::shared_ptr<Font>& _font);

    // TODO: RenderObject

    // Render Loading Indicator
    void LoadingDots(const int _count, const float _spacing, const float _radius, const render_data& _data, const std::chrono::steady_clock::duration& _clock);

    // Render Grid (debug_mode)
    void RenderGrid(const glm::vec3& _CameraPosition, const float _CellSize, const float _zoom = 1.f);
}

#endif // !FLEET_ENGINE_GRAPHICS_RENDERER
