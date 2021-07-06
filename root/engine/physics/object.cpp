// Fleet : engine/physics/object.cpp (c) 2021 Andrew Woo

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

#include <vector>
#include <memory>

#include <GLM/glm/glm.hpp>

#include "../graphics/texture.hpp"
#include "../graphics/vertex.hpp"
#include "object.hpp"

namespace Fleet::Core::Physics { 

	// Constructor
    Object::Object(const glm::vec3& _position, const glm::vec2& _size, const glm::vec4& _color, std::shared_ptr<Graphics::Texture> _texture) {

        position = _position;
        size = _size;
        color = _color;
        rotation = 0.f;

        if(_texture != nullptr)
            texture = _texture;

		// initialize the children and vertices vectors to empty vectors
        //parent = nullptr;

        // for now
        //vertices = nullptr;

        visible = true;
        DisplayVertices = false;
        scale = { 1, 1 };
    }

    // Setters
    void Object::SetRotation(const float _rotation) {
        rotation = _rotation;
    }
    void Object::SetSize(const glm::vec2& _size) {
        size = _size;
        rigidbody->SetSize(size);
    }
    void Object::SetScale(const glm::vec2& _scale) {
        scale = _scale;
        rigidbody->SetScale(scale);
    }
    void Object::SetPosition(const glm::vec3& _position) {
        position = _position;
        rigidbody->SetPosition(_position);
    }
    void Object::SetColor(const glm::vec4& _color) {
        color = _color;
    }
    void Object::SetVisible(bool _visible) {
        visible = _visible;
    }

    // Getters
    const float Object::GetRotation() const {
        return rotation;
    }
    const glm::vec2& Object::GetSize() const {
        return size;
    }
    const glm::vec2& Object::GetScale() const {
        return scale;
    }
    const glm::vec3& Object::GetPosition() const {
        return position;
    }
    const glm::vec4& Object::GetColor() const {
        return color;
    }
    const std::vector<std::string>& Object::GetTags() const {
        return tags;
    }

    const bool Object::GetVisible() const {
        return visible;
    }
}

