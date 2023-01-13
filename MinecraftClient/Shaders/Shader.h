#pragma once
#include "glad/glad.h"
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

class Shader
{
    std::unordered_map<std::string, GLint> Uniforms;
    static void CompileShader(GLuint shader);
    static void LinkProgram(GLuint program);

public:
    ~Shader();
    Shader(Shader&& other) noexcept = default;
    Shader& operator=(Shader&& other) noexcept = delete;
    Shader(const Shader& other) = default;
    Shader& operator=(const Shader& other) = delete;
    const GLuint Program;
    Shader(const std::string& vertPath, const std::string& fragPath);
    void Use() const;
    [[nodiscard]] GLuint GetUniformBlockIndex(const std::string& name) const;
    [[nodiscard]] int GetUniformInt(const std::string& name) const;
    static void SetInt(int posUniform, int value);
    static void SetUnsignedInt(int posUniform, GLuint value);
    static void SetFloat(int posUniform, float value);
    static void SetVec3(int posUniform, float x, float y, float z);
    static void SetMat4(int posUniform, glm::mat4x4 value);
};
