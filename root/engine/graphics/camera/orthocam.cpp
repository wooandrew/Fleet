// Fleet : engine/graphics/camera/orthocam.cpp (c) 2021 Andrew Woo

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

#include "orthocam.hpp"

// Include dependencies
#include <GLFW/glfw3.h>
#include <GLM/glm/gtc/matrix_transform.hpp>

// Include Fleet libraries
#include "../../input/keyboard.hpp"

namespace Fleet::Core::Graphics {

    OrthoCam::OrthoCam(const glm::mat4& _ProjectionMat, float _speed, bool _lock) {

        zoom = 1.f;
        speed = _speed;
        rotation = 0;

        lock = _lock;

        position = glm::vec3(0);

        ViewMatrix = glm::mat4(1.f);
        ProjectionMatrix = _ProjectionMat;

        ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
    }

    // Setters
    void OrthoCam::SetZoom(float _zoom, const glm::vec2& windowSize) {

        if (_zoom > 0.0f) {

            zoom = _zoom;

            float width = static_cast<float>(windowSize.x) / 2.f;
            float height = static_cast<float>(windowSize.y) / 2.f;

            // Set projection matrix after changing camera zoom
            SetProjection(glm::ortho(-width * _zoom, width * _zoom, -height * _zoom, height * _zoom));
        }
    }
    void OrthoCam::SetSpeed(float _speed) {
        speed = _speed;
    }
    void OrthoCam::SetRotation(float _rotation) {
        rotation = _rotation;
    }
    void OrthoCam::SetPosition(const glm::vec3& _position) {
        position = _position;
    }

    void OrthoCam::SetLock(bool _lock) {
        lock = _lock;
    }

    void OrthoCam::SetProjection(const glm::mat4& _projection) {
        ProjectionMatrix = _projection;
        ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
    }

    // Getters
    const float OrthoCam::GetSpeed() const {
        return speed;
    }
    const glm::vec3& OrthoCam::GetPosition() const {
        return position;
    }

    const glm::mat4& OrthoCam::GetViewMatrix() const {
        return ViewMatrix;
    }
    const glm::mat4& OrthoCam::GetProjectionMatrix() const {
        return ProjectionMatrix;
    }
    const glm::mat4& OrthoCam::GetViewProjectionMatrix() const {
        return ViewProjectionMatrix;
    }

    const bool OrthoCam::locked() const {
        return lock;
    }

    void OrthoCam::RecalculateMatrix() {

        // Scale, rotate, translate
        glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1));
        transform *= glm::translate(glm::mat4(1.0f), position);

        ViewMatrix = glm::inverse(transform);
        ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
    }

    // dt = delta time
    void OrthoCam::update(float dt) {

        if (!lock) {

            if (Fleet::Core::Input::Keyboard::KeyIsPressed(GLFW_KEY_W))                 // UP
                position.y += speed * dt;
            else if (Fleet::Core::Input::Keyboard::KeyIsPressed(GLFW_KEY_S))            // DOWN
                position.y -= speed * dt;

            if (Fleet::Core::Input::Keyboard::KeyIsPressed(GLFW_KEY_A))                 // LEFT
                position.x -= speed * dt;
            else if (Fleet::Core::Input::Keyboard::KeyIsPressed(GLFW_KEY_D))            // RIGHT
                position.x += speed * dt;

            RecalculateMatrix();
        }
    }
}
