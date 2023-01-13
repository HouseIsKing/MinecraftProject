#include "Shader.h"
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>

void Shader::CompileShader(const GLuint shader)
{
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == 0)
    {
        std::array<GLchar, 512> infoLog{0};
        glGetShaderInfoLog(shader, 512, nullptr, infoLog.data());
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog.data() << std::endl;
    }
}

void Shader::LinkProgram(const GLuint program)
{
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == 0)
    {
        std::array<GLchar, 512> infoLog{0};
        glGetProgramInfoLog(program, 512, nullptr, infoLog.data());
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog.data() << std::endl;
    }
}

Shader::Shader(const std::string& vertPath, const std::string& fragPath) : Program(glCreateProgram())
{
    std::string vertCode;
    std::string fragCode;
    std::ifstream vertFile;
    std::ifstream fragFile;

    vertFile.exceptions(std::ifstream::badbit);
    fragFile.exceptions(std::ifstream::badbit);

    try
    {
        vertFile.open(vertPath);
        fragFile.open(fragPath);
        std::stringstream vertStream;
        std::stringstream fragStream;

        vertStream << vertFile.rdbuf();
        fragStream << fragFile.rdbuf();

        vertFile.close();
        fragFile.close();

        vertCode = vertStream.str();
        fragCode = fragStream.str();
    }
    catch (std::ifstream::failure& /*e*/)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    const char* vertShaderCode = vertCode.c_str();
    const char* fragShaderCode = fragCode.c_str();

    GLuint vertShader;
    GLuint fragShader;
    vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertShaderCode, nullptr);
    CompileShader(vertShader);

    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderCode, nullptr);
    CompileShader(fragShader);
    glAttachShader(Program, vertShader);
    glAttachShader(Program, fragShader);
    LinkProgram(Program);
    glDetachShader(Program, vertShader);
    glDetachShader(Program, fragShader);
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    GLint uniformCount;
    glGetProgramiv(Program, GL_ACTIVE_UNIFORMS, &uniformCount);
    for (GLint i = 0; i < uniformCount; i++)
    {
        GLsizei nameLength;
        GLsizei size;
        GLenum type;
        std::array<GLchar, 100> name{0};
        glGetActiveUniform(Program, static_cast<GLuint>(i), sizeof name, &nameLength, &size, &type, name.data());
        Uniforms.emplace(name.data(),glGetUniformLocation(Program, name.data()));
    }
}

void Shader::Use() const
{
    glUseProgram(Program);
}

GLuint Shader::GetUniformBlockIndex(const std::string& name) const
{
    return glGetUniformBlockIndex(Program, name.c_str());
}

int Shader::GetUniformInt(const std::string& name) const
{
    return Uniforms.at(name);
}

void Shader::SetInt(const int posUniform, const int value)
{
    glUniform1i(posUniform, value);
}

void Shader::SetUnsignedInt(const int posUniform, const GLuint value)
{
    glUniform1ui(posUniform, value);
}

void Shader::SetFloat(const int posUniform, const float value)
{
    glUniform1f(posUniform, value);
}

void Shader::SetVec3(const int posUniform, const float x, const float y, const float z)
{
    glUniform3f(posUniform, x, y, z);
}

void Shader::SetMat4(const int posUniform, glm::mat4x4 value)
{
    glUniformMatrix4fv(posUniform, 1, GL_FALSE, &value[0][0]);
}

Shader::~Shader()
{
    glDeleteProgram(Program);
}


