# Fleet : root (c) 2021 Andrew Woo

# Modified MIT License
# 
# Copyright 2021 Andrew Woo
# 
#  * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
#  * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
#  * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
#  *
#  * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#  *
#  * Restrictions:
#  >  The Software may not be sold unless significant, mechanics changing modifications are made by the seller, or unless the buyer
#  >  understands an unmodified version of the Software is available elsewhere free of charge, and agrees to buy the Software given
#  >  this knowledge.
#  *
#  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
#  * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
#  * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
#  * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

cmake_minimum_required (VERSION 3.8)

add_executable(main main.cpp)

# Add project dependencies

add_library(
    
    # Add Engine
    FleetEngine STATIC

    # Main
    "engine/engine.hpp"         "engine/engine.cpp"
    "engine/manager.hpp"        "engine/manager.cpp"
    "engine/settings.hpp"       "engine/settings.cpp"

    # Input
    "engine/input/mouse.hpp"                        "engine/input/mouse.cpp"
    "engine/input/keyboard.hpp"                     "engine/input/keyboard.cpp"

    # Graphics
    "engine/graphics/manager.hpp"                   "engine/graphics/manager.cpp"
    "engine/graphics/vertex.hpp"                    "engine/graphics/vertex.cpp"
    "engine/graphics/buffer.hpp"                    "engine/graphics/buffer.cpp"
    "engine/graphics/renderer.hpp"                  "engine/graphics/renderer.cpp"
    "engine/graphics/shaders.hpp"                   "engine/graphics/shaders.cpp"
    "engine/graphics/texture.hpp"                   "engine/graphics/texture.cpp"
    "engine/graphics/font.hpp"                      "engine/graphics/font.cpp"

    # Graphics / Camera
    "engine/graphics/camera/orthocam.hpp"           "engine/graphics/camera/orthocam.cpp"

    # Math
    "engine/math/math.hpp"                          "engine/math/math.cpp"

    # Physics
    "engine/physics/object.hpp"                     "engine/physics/object.cpp"
    "engine/physics/rigidbody.hpp"                  "engine/physics/rigidbody.cpp"
    "engine/physics/collision.hpp"                  "engine/physics/collision.cpp"
)

add_library(

    # Add Objects
    FleetObjects STATIC

    "objects/flagship.hpp"              "objects/flagship.cpp"
)

# Make Engine depend on GLFW, GLAD, ASWL, FREETYPE
add_dependencies(FleetEngine libaswl glfw glad freetype)
add_dependencies(FleetObjects FleetEngine)

# Link engine against main
target_link_libraries(main PRIVATE FleetEngine)
target_link_libraries(main PRIVATE FleetObjects)

# Link dependencies against project executable
target_link_libraries(main PRIVATE libaswl)                 # Link ASWL
target_link_libraries(main PRIVATE glfw)                    # Link GLFW
target_link_libraries(main PRIVATE glad)                    # Link GLAD
# GLM is header only, no library to link                    # Link GLM
# STB is header only, no library to link                    # Link STB
target_link_libraries(main PRIVATE freetype)                # Link FreeType2
