// Fleet : objects/flagship.cpp (c) 2021 Andrew Woo

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

#include "flagship.hpp"

#include <GLFW/glfw3.h>
#include "../engine/input/keyboard.hpp"
#include "../engine/math/math.hpp"

namespace Fleet::Objects {

    Flagship::Flagship(const glm::vec3& _position, const glm::vec2& _size, const glm::vec4& _color, std::shared_ptr<Core::Graphics::Texture> _texture) 
        : Object(_position, _size, _color, _texture) {

        rotation = 90;

        // Rotation
        MaxRotationSpeed = 15.f;
        MinRotationSpeed = 5.f;
        CurrentRotationSpeed = 0.f;
        RotationAcceleration = 10.f;
        RotationDrag = 10.f;

        // Forward movement
        MaxVelocity = 50.f;
        MinVelocity = 0.f;
        CurrentVelocity = 0.f;
        acceleration = 17.f;
        drag = 17.f;

        MaxRotateInPlaceVelocity = 15.f;
        MinRotateInPlaceVelocity = 0.f;
        RotateInPlaceAcceleration = 9.f;
        RotateInPlaceDrag = 13.f;
    }

    void Flagship::update(float _dt) {

        bool MoveForward = Fleet::Core::Input::Keyboard::KeyIsPressed(GLFW_KEY_W);
        bool RotatePort = Fleet::Core::Input::Keyboard::KeyIsPressed(GLFW_KEY_A);
        bool RotateStarboard = Fleet::Core::Input::Keyboard::KeyIsPressed(GLFW_KEY_D);
        bool RotateInPlace = !MoveForward && (RotatePort || RotateStarboard);

        // Rotate
        if (RotatePort)
            CurrentRotationSpeed += RotationAcceleration * _dt;
        else if (RotateStarboard)
            CurrentRotationSpeed -= RotationAcceleration * _dt;
        else {

            if (CurrentRotationSpeed > 0)
                CurrentRotationSpeed -= RotationDrag * _dt;
            else if (CurrentRotationSpeed < 0)
                CurrentRotationSpeed += RotationDrag * _dt;
        }

        // Physics
        if (MoveForward)
            CurrentVelocity += acceleration * _dt;
        else if (RotateInPlace)
            CurrentVelocity += RotateInPlaceAcceleration * _dt;
        else
            CurrentVelocity -= drag * _dt;
        
        
        if (RotateInPlace) {
            
            if (CurrentRotationSpeed > MinRotationSpeed)
                CurrentRotationSpeed = MinRotationSpeed;
            else if (CurrentRotationSpeed < -MinRotationSpeed)
                CurrentRotationSpeed = -MinRotationSpeed;

            if (CurrentVelocity > MaxRotateInPlaceVelocity)
                CurrentVelocity = MaxRotateInPlaceVelocity;
            else if (CurrentVelocity <= MinRotateInPlaceVelocity)
                CurrentVelocity = MinRotateInPlaceVelocity;
        }
        else {

            // Handle overspeed/overrotate
            if (CurrentRotationSpeed > MaxRotationSpeed)
                CurrentRotationSpeed = MaxRotationSpeed;
            else if (CurrentRotationSpeed < -MaxRotationSpeed)
                CurrentRotationSpeed = -MaxRotationSpeed;

            if (CurrentVelocity > MaxVelocity)
                CurrentVelocity = MaxVelocity;
            else if (CurrentVelocity <= MinVelocity)
                CurrentVelocity = MinVelocity;
        }

        rotation += std::fmod(CurrentRotationSpeed * _dt, 360.f);

        // Update Position
        position.x += CurrentVelocity * std::cos(Core::Math::ConvertToRadians(rotation)) * _dt;
        position.y += CurrentVelocity * std::sin(Core::Math::ConvertToRadians(rotation)) * _dt;
    }
    void Flagship::interact() {

    }
    int Flagship::serialize() {

        return 0;
    }
}
