// Fleet : engine/physics/object.hpp (c) 2021 Andrew Woo

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

#ifndef FLEET_ENGINE_PHYSICS_OBJECT
#define FLEET_ENGINE_PHYSICS_OBJECT

// Include standard library
#include <vector>
#include <memory>

// Include dependencies
#include <GLM/glm/glm.hpp>

// Include Fleet libraries
#include "../graphics/texture.hpp"
#include "rigidbody.hpp"

namespace Fleet::Core::Physics {

    // TODO: Object class should inherit from Serializable class
    // TODO: Object class functions should all be virtual
    class Object {

        /// Inheritable object base class. All objects should inherit from this class.

        /*
         * This class is a virtual class. All video game objects 
         * should inherit from this class. No object should directly 
         * be instantiated using this class as its type; all objects
         * should be instantiated via inherited classes.
         */

    public:

        // Constructor
        Object(const glm::vec3& _position, const glm::vec2& _size, const glm::vec4& _color, std::shared_ptr<Graphics::Texture> _texture = nullptr);

        // Destructor
        virtual ~Object() = default;

        // Setters
        virtual void SetRotation(const float _rotation);
        virtual void SetSize(const glm::vec2& _size);
        virtual void SetScale(const glm::vec2& _scale);
        virtual void SetPosition(const glm::vec3& _position);
        virtual void SetColor(const glm::vec4& _color);
        virtual void SetVisible(bool _visible);

        // Getters
        const float GetRotation() const;
        const glm::vec2& GetSize() const;
        const glm::vec2& GetScale() const;
        const glm::vec3& GetPosition() const;
        const glm::vec4& GetColor() const;
        const std::vector<std::string>& GetTags() const;

        const bool GetVisible() const;
		
		// Functions
		virtual void update(float _dt) = 0;
        virtual void interact() = 0;
        virtual int serialize() = 0;
		
    protected:

        float rotation;         // in degrees
        glm::vec2 size;         // { w, h }
        glm::vec2 scale;        // { x, y }
        glm::vec3 position;     // { x, y, z }
        glm::vec4 color;        // { r, g, b, a } // ?? is color necessary ? Color should be handled via shaders

        std::shared_ptr<Fleet::Core::Graphics::Texture> texture;
        std::shared_ptr<Rigidbody> rigidbody;

        bool visible;           // visibility determines render mode
        bool DisplayVertices;   // to be deprecated; handled by Rigidbody

        int id;                             // Object ID
        std::string DisplayName;            // This Cool Item
        std::vector<std::string> tags;      // Object medadata
    };
}

#endif // !FLEET_ENGINE_PHYSICS_OBJECT
