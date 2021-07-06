// Fleet : engine/graphics/renderer.cpp (c) 2021 Andrew Woo

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

#pragma warning(disable : 6386)     // Disable buffer overrun message: it wasn't overrun.

#include "renderer.hpp"

// Include standard library
#include <iostream>
#include <vector>
#include <unordered_map>

// Include dependencies
#include <GLM/glm/gtc/matrix_transform.hpp>
#include <ASWL/experimental.hpp>

// Include Fleet libraries
#include "manager.hpp"
#include "shaders.hpp"
#include "vertex.hpp"
#include "buffer.hpp"
#include "../math/math.hpp"

namespace Fleet::Core::Graphics::Renderer {

    struct RendererData {

        RendererData() = default;

        // Max per draw call
        const uint32_t MaxQuads = 10000;
        const uint32_t MaxVertices = MaxQuads * 4;
        const uint32_t MaxIndices = MaxQuads * 6;

        // Shaders
        std::unique_ptr<ShaderLibrary> __shader_library;
        
        // Vertex Array Data
        std::unique_ptr<VertexArray> __quad_vtx_array;
        std::shared_ptr<VertexBuffer> __quad_vtx_buffer;
        unsigned int __quad_index_count = 0;
        
        // Texture storage
        int __max_texture_units = 16;
        int __texslot = 1;

        std::shared_ptr<Texture> __white;
        std::vector<int> __samplers;
        ASWL::eXperimental::UnorderedSizedMap<int, std::shared_ptr<Texture>> __bound_texture_map;
        std::vector<std::shared_ptr<Texture>> __bound_texture_array;

        // Vertex data storage
        Graphics::Vertex* __quad_vtx_buf_base = nullptr;
        Graphics::Vertex* __quad_vtx_buf_ptr = nullptr;

        // Other Data
        glm::vec2 WindowSize = { 1000, 618 };
        glm::vec2 DefaultTexCoords[4] = { { 0.f, 0.f }, { 1.f, 0.f }, { 1.f, 1.f }, { 0.f, 1.f } };
    };

    static RendererData sData;

    std::vector<glm::vec3> CalculateVertexPositions(const glm::vec3& _position, const glm::vec2& _size) {

        std::vector<glm::vec3> __vp(4); // = { glm::vec3(), glm::vec3(), glm::vec3(), glm::vec3() };

        // Counter clockwise vertices
        __vp[0] = { _position.x - (_size.x / 2), _position.y - (_size.y / 2), _position.z };        // Bottom Left
        __vp[1] = { _position.x + (_size.x / 2), _position.y - (_size.y / 2), _position.z };        // Bottom Right
        __vp[2] = { _position.x + (_size.x / 2), _position.y + (_size.y / 2), _position.z };        // Top Right
        __vp[3] = { _position.x - (_size.x / 2), _position.y + (_size.y / 2), _position.z };        // Top Left

        return __vp;
    }
    std::vector<glm::vec3>  RotateVertices(const std::vector<glm::vec3>& _vertices, const glm::vec3& _position, const float _rotation) {

        std::vector<glm::vec3> __rv(4);

        __rv[0] = Core::Math::RotatePoint(_vertices[0], _position, _rotation);
        __rv[1] = Core::Math::RotatePoint(_vertices[1], _position, _rotation);
        __rv[2] = Core::Math::RotatePoint(_vertices[2], _position, _rotation);
        __rv[3] = Core::Math::RotatePoint(_vertices[3], _position, _rotation);

        return __rv;
    }

    void init(const glm::vec2& _WindowSize, int _MaxTextureUnits) {

        // Set metadata
        sData.WindowSize = _WindowSize;

        // Create Vertex Array (dynamic)
        sData.__quad_vtx_array = std::make_unique<VertexArray>();
        sData.__quad_vtx_buf_base = new Graphics::Vertex[sData.MaxVertices];
        sData.__quad_vtx_buf_ptr = sData.__quad_vtx_buf_base;

        // Create Vertex Buffer (dynamic)
        sData.__quad_vtx_buffer = std::make_shared<VertexBuffer>();
        sData.__quad_vtx_buffer->Create(sData.MaxVertices * sizeof(Graphics::Vertex));

        sData.__quad_vtx_buffer->SetLayout({ { ShaderDataType::Float3, "a_Position" },
                                             { ShaderDataType::Float2, "a_TexCoord" },
                                             { ShaderDataType::Float4, "a_Color"},
                                             { ShaderDataType::Float, "a_TexSlot"} });

        sData.__quad_vtx_array->AddVertexBuffer(sData.__quad_vtx_buffer);

        // Create Index Buffer (dynamic)
        uint32_t* __quad_indices = new uint32_t[sData.MaxIndices];

        uint32_t offset = 0;
        for (uint32_t i = 0; i < sData.MaxIndices; i += 6) {

            __quad_indices[i + 0] = offset + 0;
            __quad_indices[i + 1] = offset + 1;
            __quad_indices[i + 2] = offset + 2;

            __quad_indices[i + 3] = offset + 2;
            __quad_indices[i + 4] = offset + 3;
            __quad_indices[i + 5] = offset + 0;

            offset += 4;
        }

        std::shared_ptr<IndexBuffer> __quad_ib = std::make_shared<IndexBuffer>(__quad_indices, sData.MaxIndices * sizeof(uint32_t));
        sData.__quad_vtx_array->SetIndexBuffer(__quad_ib);

        // Initialize Shader Library
        sData.__shader_library = std::make_unique<ShaderLibrary>(ShaderLibrary("assets/shaders/.shaders"));
        sData.__quad_vtx_array->Bind();

        // Texture data
        sData.__max_texture_units = (_MaxTextureUnits > 32) ? 32 : _MaxTextureUnits;

        for (int i = 0; i < sData.__max_texture_units; i++)
            sData.__samplers.push_back(i);

        // Create 1x1 White Texture
        uint32_t __white_data = 0xffffffff;
        sData.__white = std::make_shared<Texture>(glm::vec2(1, 1));
        sData.__white->SetData(&__white_data, sizeof(uint32_t));

        sData.__bound_texture_map = ASWL::eXperimental::UnorderedSizedMap<int, std::shared_ptr<Texture>>(sData.__max_texture_units);
        sData.__bound_texture_array.resize(sData.__max_texture_units);

        for (int i = 0; i < sData.__max_texture_units; i++)
            sData.__bound_texture_array[i] = sData.__white;

        delete[] __quad_indices;
    }
    void shutdown() {
        delete[] sData.__quad_vtx_buf_base;
    }

    void SetWindowSize(const glm::vec2& _WindowSize) {
        sData.WindowSize = _WindowSize;
    }

    // Add to batch
    void AddQuad(const std::vector<glm::vec3>& _vertices, const glm::vec4& _color, glm::vec2 _TexCoords[4], const float _texslot) {

        // Bottom Left
        sData.__quad_vtx_buf_ptr->position = _vertices[0];
        sData.__quad_vtx_buf_ptr->texcoord = _TexCoords[0];
        sData.__quad_vtx_buf_ptr->color = _color;
        sData.__quad_vtx_buf_ptr->texslot = _texslot;
        sData.__quad_vtx_buf_ptr++;

        // Bottom Right
        sData.__quad_vtx_buf_ptr->position = _vertices[1];
        sData.__quad_vtx_buf_ptr->texcoord = _TexCoords[1];
        sData.__quad_vtx_buf_ptr->color = _color;
        sData.__quad_vtx_buf_ptr->texslot = _texslot;
        sData.__quad_vtx_buf_ptr++;

        // Top Right
        sData.__quad_vtx_buf_ptr->position = _vertices[2];
        sData.__quad_vtx_buf_ptr->texcoord = _TexCoords[2];
        sData.__quad_vtx_buf_ptr->color = _color;
        sData.__quad_vtx_buf_ptr->texslot = _texslot;
        sData.__quad_vtx_buf_ptr++;

        // Top Left
        sData.__quad_vtx_buf_ptr->position = _vertices[3];
        sData.__quad_vtx_buf_ptr->texcoord = _TexCoords[3];
        sData.__quad_vtx_buf_ptr->color = _color;
        sData.__quad_vtx_buf_ptr->texslot = _texslot;
        sData.__quad_vtx_buf_ptr++;

        sData.__quad_index_count += 6;
    }

    // Render commands
    void StartScene(const std::unique_ptr<OrthoCam>& camera, const std::string& _shader) {

        sData.__shader_library->GetMap().find(_shader)->second->Bind();
        sData.__shader_library->GetMap().find(_shader)->second->SetInt1v("u_Textures", 32, sData.__samplers.data());
        sData.__shader_library->GetMap().find(_shader)->second->SetMat4("u_ViewProjection", camera->GetViewProjectionMatrix());
        sData.__shader_library->GetMap().find(_shader)->second->SetFloat4("u_Color", glm::vec4(1.f));
        sData.__shader_library->GetMap().find(_shader)->second->SetMat4("u_Transform", glm::mat4(1.f));
    }
    void FlushScene() {

        if (sData.__quad_index_count <= 0)
            return;

        for (int i = 0; i < sData.__texslot; i++)
            sData.__bound_texture_array[i]->Bind(i);

        uint32_t __size = reinterpret_cast<uint8_t*>(sData.__quad_vtx_buf_ptr) - 
                          reinterpret_cast<uint8_t*>(sData.__quad_vtx_buf_base);

        sData.__quad_vtx_buffer->SetData(sData.__quad_vtx_buf_base, __size);

        Manager::DrawIndexed(sData.__quad_vtx_array, sData.__quad_index_count);

        sData.__texslot = 1;
        sData.__quad_index_count = 0;
        sData.__quad_vtx_buf_ptr = sData.__quad_vtx_buf_base;
    }
    void EndScene() {
        FlushScene();
    }

    // Draw static quad functions
    void DrawQuad(const render_data& _data) {

        if (_data.rotation != 0) {
            auto cvp = CalculateVertexPositions(_data.position, _data.scale);
            AddQuad(RotateVertices(cvp, _data.position, _data.rotation), _data.color, sData.DefaultTexCoords);
        }
        else
            AddQuad(CalculateVertexPositions(_data.position, _data.scale), _data.color, sData.DefaultTexCoords);
    }

    // Render texture functions
    void RenderTexture(const render_data& _data, const std::shared_ptr<Texture>& _texture) {

        if (sData.__texslot > sData.__max_texture_units - 1 || sData.__quad_index_count > sData.MaxIndices)
            FlushScene();

        int texslot = 0;

        for(int i = 1; i < sData.__texslot; i++) {
            if (*sData.__bound_texture_array[i].get() == *_texture.get()) {
                texslot = i;
                break;
            }
        }
        if (texslot == 0) {
            sData.__bound_texture_array[sData.__texslot] = _texture;
            texslot = sData.__texslot++;
        }

        float t_Width = static_cast<float>(_texture->GetDimensions().x) * _data.scale.x;
        float t_Height = static_cast<float>(_texture->GetDimensions().y) * _data.scale.y;

        if (_data.rotation != 0.f) {
            auto cvp = CalculateVertexPositions(_data.position, {t_Width, t_Height});
            AddQuad(RotateVertices(cvp, _data.position, _data.rotation), _data.color, sData.DefaultTexCoords, static_cast<float>(texslot));
        }
        else
            AddQuad(CalculateVertexPositions(_data.position, { t_Width, t_Height }), { 1.f, 1.f, 1.f, 1.f }, sData.DefaultTexCoords, static_cast<float>(texslot));
    }

    // Render text functions
    void RenderText(const std::string& _string, const render_data& _data, const std::shared_ptr<Font>& _font) {

        sData.__shader_library->GetMap().find("text")->second->SetBool("u_Debug", false);

        if (sData.__texslot > sData.__max_texture_units - 1 || sData.__quad_index_count > sData.MaxIndices)
            FlushScene();

        int texslot = 0;

        for (int i = 1; i < sData.__texslot; i++) {
            if (sData.__bound_texture_array[i]->GetTextureID() == _font->GetTextureID()) {
                texslot = i;
                break;
            }
        }
        if (texslot == 0) {
            sData.__bound_texture_array[sData.__texslot] = _font;
            texslot = sData.__texslot++;
        }

        float px = 0;
        float pz = _data.position.z;

        // Calculate string offset
        glm::vec2 offset = { 0.f, 0.f };
        for (std::string::const_iterator i = _string.begin(); i != _string.end(); ++i) {

            Character ch = _font->GetCharacters().find(*i)->second;

            float x = ((px + ch.bearing.x) + (ch.size.x / 2.f)) * _data.scale.x - offset.x;

            if (i == _string.end() - 1)
                offset.x = x + (ch.size.x / 2.f) + (_font->GetCharacters().find(_string[0])->second.bearing.x / 2.f) + 4;

            offset.y = std::max(offset.y, ch.size.y);

            px += ((static_cast<int>(ch.advance.x) >> 6) - (ch.bearing.x / 2.f)) * _data.scale.x;
        }
        offset /= 2.f;

        px = _data.position.x;

        for (std::string::const_iterator i = _string.begin(); i != _string.end(); ++i) {

            if (sData.__quad_index_count > sData.MaxIndices)
                FlushScene();

            Character ch = _font->GetCharacters().find(*i)->second;

            float xPos = ((px + ch.bearing.x) + (ch.size.x / 2.f)) * _data.scale.x - offset.x;

            // yPos offset calculations are inverted, because font is inverted, then corrected in the shader.
            // Offsets:  inverted y axis position    glyph baseline bearing       glyph centering     string height
            float yPos = (_data.position.y * -1.f) + (ch.size.y - ch.bearing.y) - (ch.size.y / 2.f) + (offset.y - 1.f);

            float t_Width = static_cast<float>(ch.size.x) * _data.scale.x;
            float t_Height = static_cast<float>(ch.size.y) * _data.scale.y;

            AddQuad(CalculateVertexPositions({ xPos, yPos, pz += 0.00001 }, { t_Width, t_Height }), _data.color, ch.TexCoords.data(), static_cast<float>(texslot));

            px += ((static_cast<int>(ch.advance.x) >> 6) - (ch.bearing.x / 2.f)) * _data.scale.x;
        }
    }

    // Render Loading Indicator
    void LoadingDots(const int _count, const float _spacing, const float _radius, const render_data& _data, const std::chrono::steady_clock::duration& _clock) {

        AddQuad(CalculateVertexPositions(_data.position, _data.scale), _data.color, sData.DefaultTexCoords);

        std::shared_ptr<Shader> shader = sData.__shader_library->GetMap().find("dots")->second;

        float runtime = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(_clock).count() / 1000.f;

        shader->SetFloat2("u_Resolution", sData.WindowSize);
        shader->SetInt("u_CircleCount", _count);
        shader->SetFloat("u_Spacing", _spacing);
        shader->SetFloat("u_Radius", _radius);
        shader->SetFloat("u_RunTime", runtime);
    }

    // Render Grid (debug_mode)
    void RenderGrid(const glm::vec3& _CameraPosition, const float _CellSize, const float _zoom) {

        AddQuad(CalculateVertexPositions({ 0, 0, 1.f }, sData.WindowSize), glm::vec4(1.f), sData.DefaultTexCoords);
        std::shared_ptr<Shader> shader = sData.__shader_library->GetMap().find("grid")->second;

        shader->SetFloat("u_CellSize", _CellSize);
        shader->SetFloat2("u_Resolution", sData.WindowSize);
        shader->SetFloat3("u_CameraPosition", _CameraPosition);
    }
}
