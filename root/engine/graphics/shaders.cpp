// Project Boomerang : engine/graphics/shader/shaders.cpp (c) 2020 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

/* Modified MIT License
 *
 * Copyright 2020 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan
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
#include <fstream>
#include <unordered_map>

// Include dependencies
#include <GLM/glm/gtc/type_ptr.hpp>

namespace Boomerang::Core::Graphics {

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

            Boomerang::Misc::Logger::logger("S0000", "Error: Unknown shader data type.");

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

            Boomerang::Misc::Logger::logger("S0001", "Error: Unknown shader data type.");

            return 0;
        }
    }

    Shader::Shader(const std::string& vtxPath, const std::string& frgPath) {
        init(vtxPath, frgPath);
    }

    void Shader::init(const std::string& vtxPath, const std::string& frgPath) {

        std::string vtxSource;
        std::string frgSource;

        // Lambda to read vtxSource & frgSource
        auto read = [](std::string path, std::string& source) {

            // Read Vertex Shader Source
            std::ifstream input(path, std::ios::in | std::ios::binary);
            if (input) {

                input.seekg(0, std::ios::end);
                source.resize(input.tellg());

                input.seekg(0, std::ios::beg);
                input.read(&source[0], source.size());
                input.close();
            }
        };

        // TODO: Check for read errors
        read(vtxPath, std::ref(vtxSource));  // Read Vertex Shader
        read(frgPath, std::ref(frgSource));  // Read Fragment Shader


        // --- Shader processing
        std::unordered_map<GLenum, std::string> sources;
        sources[GL_VERTEX_SHADER] = vtxSource;
        sources[GL_FRAGMENT_SHADER] = frgSource;

        unsigned int program = glad_glCreateProgram();

        std::array<unsigned int, 2> ids;
        int idIndex = 0;

        for (auto& source : sources) {

            //GLenum type = source.first;
            const std::string& tmpSrc = source.second;

            GLuint shader = glad_glCreateShader(source.first);

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
                Boomerang::Misc::Logger::logger<std::string, std::string>("S0002", "Error: Could not compile shader -> ", std::string(log.data()));

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
            Boomerang::Misc::Logger::logger<std::string, std::string>("S0002", "Error: Failed to link shaders -> ", std::string(log.data()));

            return;
        }

        for (auto id : ids) {
            glad_glDetachShader(program, id);
            glad_glDeleteShader(id);
        }

        // --- End shader processing

        // --- Name shader

        // Find final slash in path -> /root/assets/shaders/shader-vert.glsl
        auto lastSlash = vtxPath.find_last_of("/\\");
        lastSlash = (lastSlash == std::string::npos) ? 0 : lastSlash + 1;

        // Find final period in path -> /root/assets/shaders/shader-vert.glsl
        auto lastDot = vtxPath.find_last_of('.');
        lastDot = (lastDot == std::string::npos) ? vtxPath.size() - lastSlash : lastDot - lastSlash;

        name = vtxPath.substr(lastSlash, lastDot);
    }

    std::string Shader::GetName() const {
        return name;
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
    void Shader::SetFloat3(const std::string& _name, const glm::vec3& _value) {
        UploadUniformFloat3(_name, _value);
    }
    void Shader::SetFloat4(const std::string& _name, const glm::vec4& _value) {
        UploadUniformFloat4(_name, _value);
    }
    void Shader::SetMat4(const std::string& _name, const glm::mat4& _value) {
        UploadUniformMat4(_name, _value);
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
}