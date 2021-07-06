// Fleet : engine/physics/collision.cpp (c) 2021 Andrew Woo

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

#include "collision.hpp"

// Include standard library
#include <vector>
#include <algorithm>

// Include dependencies
#include <GLM/glm/gtx/projection.hpp>

namespace Fleet::Core::Physics {

    bool Collision::AABB(const Rigidbody& left, const Rigidbody& right) {
        return AABB<float>(left.x, left.y, left.w, left.h, right.x, right.y, right.x, right.h);
    }

    bool Collision::SAT(const Rigidbody& left, const Rigidbody& right) {

        glm::vec3 aUL = left.vt.UpperLeftVertex;        // A -> Upper Left
        glm::vec3 aLL = left.vt.LowerLeftVertex;        // A -> Lower Left
        glm::vec3 aUR = left.vt.UpperRightVertex;       // A -> Upper Right
        glm::vec3 aLR = left.vt.LowerRightVertex;       // A -> Lower Right

        glm::vec3 bUL = right.vt.UpperLeftVertex;       // B -> Upper Left
        glm::vec3 bLL = right.vt.LowerLeftVertex;       // B -> Lower Left
        glm::vec3 bUR = right.vt.UpperRightVertex;      // B -> Upper Right
        glm::vec3 bLR = right.vt.LowerRightVertex;      // B -> Lower Right

        float aMax = 0;
        float aMin = 0;
        float bMax = 0;
        float bMin = 0;

        std::vector<glm::vec3> axes;
        axes.push_back(aUR - aUL);
        axes.push_back(aUR - aLR);
        axes.push_back(bUL - bLL);
        axes.push_back(bUL - bUR);

        for (auto& axis : axes) {

            glm::vec3 aULProject = glm::proj(aUL, axis);
            glm::vec3 aLLProject = glm::proj(aLL, axis);
            glm::vec3 aURProject = glm::proj(aUR, axis);
            glm::vec3 aLRProject = glm::proj(aLR, axis);

            glm::vec3 bULProject = glm::proj(bUL, axis);
            glm::vec3 bLLProject = glm::proj(bLL, axis);
            glm::vec3 bURProject = glm::proj(bUR, axis);
            glm::vec3 bLRProject = glm::proj(bLR, axis);

            std::vector<float> aDots;
            aDots.push_back(glm::dot(aULProject, axis));
            aDots.push_back(glm::dot(aLLProject, axis));
            aDots.push_back(glm::dot(aURProject, axis));
            aDots.push_back(glm::dot(aLRProject, axis));
            aMax = *std::max_element(aDots.begin(), aDots.end());
            aMin = *std::min_element(aDots.begin(), aDots.end());

            std::vector<float> bDots;
            bDots.push_back(glm::dot(bULProject, axis));
            bDots.push_back(glm::dot(bLLProject, axis));
            bDots.push_back(glm::dot(bURProject, axis));
            bDots.push_back(glm::dot(bLRProject, axis));
            bMax = *std::max_element(bDots.begin(), bDots.end());
            bMin = *std::min_element(bDots.begin(), bDots.end());

            if (aMax >= bMin && bMax >= aMin)
                continue;
            else
                return false;
        }

        return true;
    }
}
