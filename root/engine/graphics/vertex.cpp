// Fleet : engine/graphics/vertex.cpp (c) 2021 Andrew Woo

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

#include "vertex.hpp"

#include <ASWL/logger.hpp>

namespace Fleet::Core::Graphics {

    VertexArray::VertexArray() {
        VertexBufferIndex = 0;
        glad_glCreateVertexArrays(1, &vtxaobj);
    }
    VertexArray::~VertexArray() {
        glad_glDeleteVertexArrays(1, &vtxaobj);
    }

    void VertexArray::Bind() const {
        glad_glBindVertexArray(vtxaobj);
    }
    void VertexArray::Unbind() const {
        glad_glBindVertexArray(0);
    }

    void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vtxBuffer) {

        if (vtxBuffer->GetLayout().GetElements().size() == 0) {
            ASWL::Logger::logger("VA001", "Error: Vertex Buffer has no layout.");
            return;
        }

        glad_glBindVertexArray(vtxaobj);
        vtxBuffer->Bind();

        const auto& layout = vtxBuffer->GetLayout();

        for (const auto& element : layout) {

            glad_glEnableVertexAttribArray(VertexBufferIndex);
            glad_glVertexAttribPointer(VertexBufferIndex, element.GetComponentCount(), ShaderTypeToGLBaseType(element.type),
                                       element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), reinterpret_cast<const void*>(element.offset));
            VertexBufferIndex++;
        }

        VertexBuffers.push_back(vtxBuffer);
    }

    void VertexArray::SetIndexBuffer(const std::shared_ptr<Fleet::Core::Graphics::IndexBuffer>& idxBuffer) {

        glad_glBindVertexArray(vtxaobj);
        idxBuffer->Bind();

        ptrIndexBuffer = idxBuffer;
    }

    const std::vector<std::shared_ptr<VertexBuffer>>& VertexArray::GetVertexBuffers() const {
        return VertexBuffers;
    }
    const std::shared_ptr<IndexBuffer>& VertexArray::GetIndexBuffer() const {
        return ptrIndexBuffer;
    }
}
