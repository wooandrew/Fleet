// Fleet : engine/graphics/shaders.hpp (c) 2021 Andrew Woo

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

#ifndef FLEET_ENGINE_GRAPHICS_SHADERS
#define FLEET_ENGINE_GRAPHICS_SHADERS

// Include standard library
#include <map>
#include <string>
#include <memory>

// Include dependencies
#include <glad/glad.h>
#include <GLM/glm/glm.hpp>

namespace Fleet::Core::Graphics {

    // Shader data type
    enum class ShaderDataType {

        None = 0,
        Float, Float2, Float3, Float4,          // Type Float
        Mat3, Mat4,                             // Type Matrix
        Int, Int2, Int3, Int4,                  // Type Int
        Bool                                    // Type Bool
    };

    // Shader type to GL base type
    int ShaderDataTypeSize(ShaderDataType type);
    GLenum ShaderTypeToGLBaseType(ShaderDataType type);

    class Shader {

        /// Shader loader class

    public:

        Shader() = default;
        Shader(const std::string& vtxPath, const std::string& frgPath);
        Shader(const std::string& _name, const std::string& vtxPath, const std::string& frgPath);

        void init(const std::string& vtxPath, const std::string& frgPath);
        void init(const std::string& _name, const std::string& vtxPath, const std::string& frgPath);

        void Bind() const;
        void Unbind() const;

        void SetInt(const std::string& _name, int _value);
        void SetFloat(const std::string& _name, const float _value);
        void SetFloat2(const std::string& _name, const glm::vec2& _value);
        void SetFloat3(const std::string& _name, const glm::vec3& _value);
        void SetFloat4(const std::string& _name, const glm::vec4& _value);
        void SetMat3(const std::string& _name, const glm::mat3& _value);
        void SetMat4(const std::string& _name, const glm::mat4& _value);
        void SetBool(const std::string& _name, const bool _value);

        void SetInt1v(const std::string& _name, const int _count, const int* _values);

        // Getters
        const std::string& GetName() const;
        const unsigned int GetRendererID() const;

    private:

        void UploadUniformInt(const std::string& _name, int _value);
        void UploadUniformFloat(const std::string& _name, float _value);
        void UploadUniformFloat2(const std::string& _name, const glm::vec2& _value);
        void UploadUniformFloat3(const std::string& _name, const glm::vec3& _value);
        void UploadUniformFloat4(const std::string& _name, const glm::vec4& _value);
        void UploadUniformMat3(const std::string& _name, const glm::mat3& _matrix);
        void UploadUniformMat4(const std::string& _name, const glm::mat4& _matrix);
        void UploadUniformBool(const std::string& _name, const bool _value);

        void UploadUniformInt1v(const std::string& _name, const int _count, const int* _values);

        unsigned int RendererID;
        std::string name;
    };


    // TODO
    class ShaderLibrary {
        
    public:

        ShaderLibrary() = default;
        ShaderLibrary(const std::string& _LibraryPath);
        ~ShaderLibrary();

        int init(const std::string& _LibraryPath);

        void AddShader(std::shared_ptr<Shader>& _shader);
        void AddShader(const std::string& _name, const std::string& _vtxPath, const std::string& _frgPath);
        void AddLibrary(const std::string& _LibraryPath);

        const std::map<std::string, std::shared_ptr<Shader>>& GetMap() const;

    private:

        std::string LibraryPath;
        std::map<std::string, std::shared_ptr<Shader>> map;
    };
}

#endif // !FLEET_ENGINE_GRAPHICS_SHADERS
