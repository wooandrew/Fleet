// Fleet : engine/physics/rigidbody.cpp (c) 2021 Andrew Woo

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

#include "rigidbody.hpp"

#include "../math/math.hpp"

namespace Fleet::Core::Physics {

    Rigidbody::Rigidbody(const glm::vec3& _position, const glm::vec2& _size, const glm::vec2& _scale) {

        position = _position;
        size = _size;
        scale = _scale;

        LastRotation = 0;

        x = position.x - ((size.x * scale.x) / 2.f);
        y = position.y - ((size.y * scale.y) / 2.f);
        w = size.x * scale.x;
        h = size.y * scale.y;

        UpdateVertices();
    }

    Rigidbody::~Rigidbody() {

    }

    void Rigidbody::update(const glm::vec3& _position, float _rotation) {
        
        SetPosition(_position);

        if (LastRotation != _rotation) {

            vt.UpperLeftVertex = Math::RotatePoint(vt.UpperLeftVertex, position, _rotation - LastRotation);
            vt.LowerLeftVertex = Math::RotatePoint(vt.LowerLeftVertex, position, _rotation - LastRotation);
            vt.UpperRightVertex = Math::RotatePoint(vt.UpperRightVertex, position, _rotation - LastRotation);
            vt.LowerRightVertex = Math::RotatePoint(vt.LowerRightVertex, position, _rotation - LastRotation);

            LastRotation = _rotation;
        }
    }

    void Rigidbody::UpdateVertices() {

        x = position.x - ((size.x * scale.x) / 2.f);
        y = position.y - ((size.y * scale.y) / 2.f);
        w = size.x * scale.x;
        h = size.y * scale.y;

        vt.UpperLeftVertex = glm::vec3(position.x - ((size.x * scale.x) / 2.f), position.y + ((size.y * scale.y) / 2.f), 0);
        vt.LowerLeftVertex = glm::vec3(position.x - ((size.x * scale.x) / 2.f), position.y - ((size.y * scale.y) / 2.f), 0);;
        vt.UpperRightVertex = glm::vec3(position.x + ((size.x * scale.x) / 2.f), position.y + ((size.y * scale.y) / 2.f), 0);;
        vt.LowerRightVertex = glm::vec3(position.x + ((size.x * scale.x) / 2.f), position.y - ((size.y * scale.y) / 2.f), 0);;

        vt.UpperLeftVertex = Math::RotatePoint(vt.UpperLeftVertex, position, LastRotation);
        vt.LowerLeftVertex = Math::RotatePoint(vt.LowerLeftVertex, position, LastRotation);
        vt.UpperRightVertex = Math::RotatePoint(vt.UpperRightVertex, position, LastRotation);
        vt.LowerRightVertex = Math::RotatePoint(vt.LowerRightVertex, position, LastRotation);
    }


    // Setters
    void Rigidbody::SetSize(const glm::vec2& _size) {
        size = _size;
        UpdateVertices();
    }
    void Rigidbody::SetScale(const glm::vec2& _scale) {
        scale = _scale;
        UpdateVertices();
    }
    void Rigidbody::SetRotation(const float _rotation) {
        update(position, _rotation);
    }
    void Rigidbody::SetPosition(const glm::vec3& _position) {
        position = _position;
        UpdateVertices();
    }
    void Rigidbody::AdvancePositionBy(const glm::vec3& _advance) {
        position += _advance;
        UpdateVertices();
    }

    // Getters
    const bool Rigidbody::GetVisible() const {
        return visible;
    }
    const glm::vec2& Rigidbody::GetSize() const {
        return size;
    }
    const glm::vec2& Rigidbody::GetScale() const {
        return scale;
    }
    const glm::vec3& Rigidbody::GetPosition() const {
        return position;
    }
    const Rigidbody::Vertices Rigidbody::GetVertices() const {
        return vt;
    }

    // Debug Functions
    void Rigidbody::SetVisible(bool _visible) {
        visible = _visible;
    }
}
