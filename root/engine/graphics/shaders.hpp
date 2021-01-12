// Project Boomerang : engine/graphics/shaders.hpp (c) 2020 Andrew Woo, Porter Squires, Brandon Yau, and Awrish Khan

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

#pragma once

#ifndef BOOMERANG_ENGINE_GRAPHICS_SHADERS
#define BOOMERANG_ENGINE_GRAPHICS_SHADERS

// Include standard library
#include <map>
#include <string>

// Include dependencies
#include <glad/glad.h>
#include <GLM/glm/glm.hpp>

namespace Boomerang::Core::Graphics {

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
        void SetFloat3(const std::string& _name, const glm::vec3& _value);
        void SetFloat4(const std::string& _name, const glm::vec4& _value);
        void SetMat4(const std::string& _name, const glm::mat4& _value);

        void UploadUniformInt(const std::string& _name, int _value);

        void UploadUniformFloat(const std::string& _name, float _value);
        void UploadUniformFloat2(const std::string& _name, const glm::vec2& _value);
        void UploadUniformFloat3(const std::string& _name, const glm::vec3& _value);
        void UploadUniformFloat4(const std::string& _name, const glm::vec4& _value);

        void UploadUniformMat3(const std::string& _name, const glm::mat3& _matrix);
        void UploadUniformMat4(const std::string& _name, const glm::mat4& _matrix);

        // Getters
        const std::string& GetName() const;

    private:

        unsigned int RendererID;
        std::string name;
    };


    class ShaderLibrary {
        
    public:

        ShaderLibrary() = default;
        ShaderLibrary(const std::string& _libraryPath);
        ~ShaderLibrary();

        int init(const std::string& _libraryPath);

        void AddShader(Shader _shader);
        void AddShader(const std::string& _name, const std::string& _path);
        void AddLibrary(const std::string& _libraryPath);

    private:

        std::string LibraryPath;
        std::map<std::string, Shader> list;
    };
}

#endif // !BOOMERANG_ENGINE_GRAPHICS_SHADERS
