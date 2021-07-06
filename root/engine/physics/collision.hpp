// Fleet : engine/physics/collision.hpp (c) 2021 Andrew Woo

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

#ifndef FLEET_ENGINE_PHYSICS_COLLISION
#define FLEET_ENGINE_PHYSICS_COLLISION

// Include dependencies
#include <GLM/glm/glm.hpp>

// Include Fleet libraries
#include "rigidbody.hpp"
#include "../math/math.hpp"

namespace Fleet::Core::Physics {

    class Collision {

        /// Static collision detection functions

    public:

        Collision() = delete;

        // Axis Aligned Bounding Box    -> Usage: for fast collision detection between parallel quads
        template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr> 
        static bool AABB(T x, T y, T w, T h, T x2, T y2, T w2, T h2) {

            if (x < x2 + w2 && x + w > x2 && y < y2 + h2 && y + h > y2)
                return true;

            return false;
        }
        static bool AABB(const Rigidbody& left, const Rigidbody& right);

        // Separating Axis Theorem      -> Usage: for collision detection between rotated n-gons
        static bool SAT(const Rigidbody& left, const Rigidbody& right);
    };
}

#endif // !FLEET_ENGINE_PHYSICS_COLLISION
