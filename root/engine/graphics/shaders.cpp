// Fleet : engine/graphics/shader/shaders.cpp (c) 2021 Andrew Woo

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

#include "shaders.hpp"

// Include standard library
#include <array>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>

// Include dependencies
#include <GLM/glm/gtc/type_ptr.hpp>
#include <ASWL/utilities.hpp>
#include <ASWL/logger.hpp>

namespace Fleet::Core::Graphics {

    int ShaderDataTypeSize(ShaderDataType type) {

        switch (type) {

        case ShaderDataType::Float:         return 4;
        case ShaderDataType::Float2:        return 4 * 2;
        case ShaderDataType::Float3:        return 4 * 3;
        case ShaderDataType::Float4:        return 4 * 4;
        case ShaderDataType::Mat3:          return 4 * 3 * 3;   // Check validity of size
        case ShaderDataType::Mat4:          return 4 * 4 * 4;
        case ShaderDataType::Int:           return 4;
        case ShaderDataType::Int2:          return 4 * 2;
        case ShaderDataType::Int3:          return 4 * 3;
        case ShaderDataType::Int4:          return 4 * 4;
        case ShaderDataType::Bool:          return 1;

        default:

            ASWL::Logger::logger("S0000", "Error: Unknown shader data type.");

            return 0;
        }
    }
    GLenum ShaderTypeToGLBaseType(ShaderDataType type) {

        switch (type) {

        case ShaderDataType::Float:         return GL_FLOAT;
        case ShaderDataType::Float2:        return GL_FLOAT;
        case ShaderDataType::Float3:        return GL_FLOAT;
        case ShaderDataType::Float4:        return GL_FLOAT;
        case ShaderDataType::Mat3:          return GL_FLOAT;
        case ShaderDataType::Mat4:          return GL_FLOAT;
        case ShaderDataType::Int:           return GL_INT;
        case ShaderDataType::Int2:          return GL_INT;
        case ShaderDataType::Int3:          return GL_INT;
        case ShaderDataType::Int4:          return GL_INT;
        case ShaderDataType::Bool:          return GL_INT;

        default:

            ASWL::Logger::logger("S0001", "Error: Unknown shader data type.");

            return 0;
        }
    }

    Shader::Shader(const std::string& vtxPath, const std::string& frgPath) {
        
        init(vtxPath, frgPath);

        // --- Name shader

        // Find final slash in path -> /root/assets/shaders/shader-vert.glsl
        auto lastSlash = vtxPath.find_last_of("/\\");
        lastSlash = (lastSlash == std::string::npos) ? 0 : lastSlash + 1;

        // Find final period in path -> /root/assets/shaders/shader-vert.glsl
        auto lastDot = vtxPath.find_last_of('.');
        lastDot = (lastDot == std::string::npos) ? vtxPath.size() - lastSlash : lastDot - lastSlash;

        name = vtxPath.substr(lastSlash, lastDot);
    }

    Shader::Shader(const std::string& _name, const std::string& vtxPath, const std::string& frgPath) {
        init(_name, vtxPath, frgPath);
    }

    void Shader::init(const std::string& vtxPath, const std::string& frgPath) {

        std::string vtxSource = ASWL::Utilities::ReadFile(vtxPath, std::ios::binary);  // Read Vertex Shader
        std::string frgSource = ASWL::Utilities::ReadFile(frgPath, std::ios::binary);  // Read Fragment Shader

        // --- Shader processing
        std::unordered_map<GLenum, std::string> sources;
        sources[GL_VERTEX_SHADER] = vtxSource;
        sources[GL_FRAGMENT_SHADER] = frgSource;

        unsigned int program = glad_glCreateProgram();

        std::array<unsigned int, 2> ids = {};
        int idIndex = 0;

        for (auto& source : sources) {

            GLenum type = source.first;
            const std::string& tmpSrc = source.second;

            GLuint shader = glad_glCreateShader(type);

            const GLchar* srcCStr = tmpSrc.c_str();
            glad_glShaderSource(shader, 1, &srcCStr, 0);
            glad_glCompileShader(shader);

            GLint isCompiled = 0;
            glad_glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

            // If shader compilation fails, log the error
            if (isCompiled == GL_FALSE) {

                GLint logLength = 0;
                glad_glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
                
                std::vector<GLchar> log(logLength);
                glad_glGetShaderInfoLog(shader, logLength, &logLength, &log[0]);

                glad_glDeleteShader(shader);

                // Log
                ASWL::Logger::logger("S0002", "Error: Could not compile shader -> ", log.data());

                return;
            }

            glad_glAttachShader(program, shader);
            ids[idIndex++] = shader;
        }

        RendererID = program;

        glad_glLinkProgram(program);
        GLint isLinked = 0;

        glad_glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE) {

            GLint logLength = 0;
            glad_glGetShaderiv(program, GL_INFO_LOG_LENGTH, &logLength);

            std::vector<GLchar> log(logLength);
            glad_glGetShaderInfoLog(program, logLength, &logLength, &log[0]);

            glad_glDeleteProgram(program);

            for (auto id : ids)
                glad_glDeleteShader(id);

            // Log
            ASWL::Logger::logger("S0002", "Error: Failed to link shaders -> ", log.data());

            return;
        }

        for (auto id : ids) {
            glad_glDetachShader(program, id);
            glad_glDeleteShader(id);
        }

        // --- End shader processing
    }

    void Shader::init(const std::string& _name, const std::string& vtxPath, const std::string& frgPath) {

        init(vtxPath, frgPath);
        name = _name;
    }

    const std::string& Shader::GetName() const {
        return name;
    }
    const unsigned int Shader::GetRendererID() const {
        return RendererID;
    }

    void Shader::Bind() const {
        glad_glUseProgram(RendererID);
    }

    void Shader::Unbind() const {
        glad_glUseProgram(0);
    }

    void Shader::SetInt(const std::string& _name, int _value) {
        UploadUniformInt(_name, _value);
    }
    void Shader::SetFloat(const std::string& _name, const float _value) {
        UploadUniformFloat(_name, _value);
    }
    void Shader::SetFloat2(const std::string& _name, const glm::vec2& _value) {
        UploadUniformFloat2(_name, _value);
    }
    void Shader::SetFloat3(const std::string& _name, const glm::vec3& _value) {
        UploadUniformFloat3(_name, _value);
    }
    void Shader::SetFloat4(const std::string& _name, const glm::vec4& _value) {
        UploadUniformFloat4(_name, _value);
    }
    void Shader::SetMat3(const std::string& _name, const glm::mat3& _value) {
        UploadUniformMat3(_name, _value);
    }
    void Shader::SetMat4(const std::string& _name, const glm::mat4& _value) {
        UploadUniformMat4(_name, _value);
    }
    void Shader::SetBool(const std::string& _name, const bool _value) {
        UploadUniformBool(_name, _value);
    }

    void Shader::SetInt1v(const std::string& _name, const int _count, const int* _values) {
        UploadUniformInt1v(_name, _count, _values);
    }

    void Shader::UploadUniformInt(const std::string& _name, int _value) {
        GLuint location = glad_glGetUniformLocation(RendererID, _name.c_str());
        glad_glUniform1i(location, _value);
    }

    void Shader::UploadUniformFloat(const std::string& _name, float _value) {
        GLuint location = glad_glGetUniformLocation(RendererID, _name.c_str());
        glad_glUniform1f(location, _value);
    }
    void Shader::UploadUniformFloat2(const std::string& _name, const glm::vec2& _value) {
        GLuint location = glad_glGetUniformLocation(RendererID, _name.c_str());
        glad_glUniform2f(location, _value.x, _value.y);
    }
    void Shader::UploadUniformFloat3(const std::string& _name, const glm::vec3& _value) {
        GLuint location = glad_glGetUniformLocation(RendererID, _name.c_str());
        glad_glUniform3f(location, _value.x, _value.y, _value.z);
    }
    void Shader::UploadUniformFloat4(const std::string& _name, const glm::vec4& _value) {
        GLuint location = glad_glGetUniformLocation(RendererID, _name.c_str());
        glad_glUniform4f(location, _value.x, _value.y, _value.z, _value.w);
    }

    void Shader::UploadUniformMat3(const std::string& _name, const glm::mat3& _matrix) {
        GLuint location = glad_glGetUniformLocation(RendererID, _name.c_str());
        glad_glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(_matrix));
    }
    void Shader::UploadUniformMat4(const std::string& _name, const glm::mat4& _matrix) {
        GLuint location = glad_glGetUniformLocation(RendererID, _name.c_str());
        glad_glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(_matrix));
    }
    void Shader::UploadUniformBool(const std::string& _name, const bool _value) {
        GLuint location = glad_glGetUniformLocation(RendererID, _name.c_str());
        glad_glUniform1i(location, _value);
    }

    void Shader::UploadUniformInt1v(const std::string& _name, const int _count, const int* _values) {
        GLuint location = glad_glGetUniformLocation(RendererID, _name.c_str());
        glad_glUniform1iv(location, _count, _values);
    }

    ShaderLibrary::ShaderLibrary(const std::string& _libraryPath) {
        init(_libraryPath);
    }
    ShaderLibrary::~ShaderLibrary() {

    }

    int ShaderLibrary::init(const std::string& _LibraryPath) {

        LibraryPath = _LibraryPath;
        AddLibrary(_LibraryPath);

        return 0;
    }

    void ShaderLibrary::AddShader(std::shared_ptr<Shader>& _shader) {
        map.insert({ _shader->GetName(), _shader });
    }
    void ShaderLibrary::AddShader(const std::string& _name, const std::string& _vtxPath, const std::string& _frgPath) {
        map.insert({ _name, std::make_shared<Shader>(Shader(_name, _vtxPath, _frgPath)) });
    }
    void ShaderLibrary::AddLibrary(const std::string& _LibraryPath) {

        std::string raw = ASWL::Utilities::ReadFile(_LibraryPath, std::ios::binary);
        std::istringstream iss(raw);

        for (std::string __line; std::getline(iss, __line); ) {

            std::vector<std::string> __data = ASWL::Utilities::split(__line, ';');
            std::shared_ptr<Shader> shader = std::make_shared<Shader>(Shader(__data[0], ASWL::Utilities::strip(__data[2], "\r"), __data[1]));
            map.insert({ __data[0], shader });
        }
    }

    const std::map<std::string, std::shared_ptr<Shader>>& ShaderLibrary::GetMap() const {
        return std::ref(map);
    }
}
