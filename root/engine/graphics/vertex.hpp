// Fleet : engine/graphics/vertex.hpp (c) 2021 Andrew Woo

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

#ifndef FLEET_ENGINE_GRAPHICS_VERTEX
#define FLEET_ENGINE_GRAPHICS_VERTEX

// Include standard library
#include <vector>
#include <memory>

// Include Fleet libraries
#include "buffer.hpp"

namespace Fleet::Core::Graphics {

    struct Vertex {
        glm::vec3 position;
        glm::vec2 texcoord;
        glm::vec4 color;
        float texslot;
    };

    class VertexArray {

        /// Vertex array class

    public:

        VertexArray();
        ~VertexArray();

        void Bind() const;
        void Unbind() const;

        void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vtxBuffer);
        void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& idxBuffer);

        const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const;
        const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const;

    private:

        unsigned int vtxaobj;
        unsigned int VertexBufferIndex;

        std::vector<std::shared_ptr<VertexBuffer>> VertexBuffers;
        std::shared_ptr<IndexBuffer> ptrIndexBuffer;
    };
}

#endif // !FLEET_ENGINE_GRAPHICS_VERTEX
