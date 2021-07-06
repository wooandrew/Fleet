// Fleet : objects/flagship.hpp (c) 2021 Andrew Woo

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

#ifndef FLEET_OBJECTS_FLAGSHIP
#define FLEET_OBJECTS_FLAGSHIP

#include <GLM/glm/glm.hpp>

#include "../engine/graphics/texture.hpp"
#include "../engine/physics/object.hpp"

namespace Fleet::Objects {

    class Flagship : public Core::Physics::Object {

        /// Flagship object

    public:

        Flagship(const glm::vec3& _position, const glm::vec2& _size, const glm::vec4& _color, std::shared_ptr<Core::Graphics::Texture> _texture = nullptr);
        ~Flagship() = default;

        void update(float _dt) override;
        void interact() override;
        int serialize() override;
        
    private:

        // Rotation
        float MaxRotationSpeed;
        float MinRotationSpeed;
        float CurrentRotationSpeed;
        float RotationAcceleration;
        float RotationDrag;
        
        // Forward Movement
        float MaxVelocity;
        float MinVelocity;
        float CurrentVelocity;
        float acceleration;
        float drag;

        // Rotate in place
        float MaxRotateInPlaceVelocity;
        float MinRotateInPlaceVelocity;
        float RotateInPlaceAcceleration;
        float RotateInPlaceDrag;

        // Boats dont move backwards.
    };
}

#endif // !FLEET_OBJECTS_FLAGSHIP
