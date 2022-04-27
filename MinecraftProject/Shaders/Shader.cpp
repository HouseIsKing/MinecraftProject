//
// Created by amit on 4/22/2022.
//

#include "Shader.h"

void Shader::compileShader(GLuint shader) {
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }
}

void Shader::linkProgram(GLuint program) {
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }
}

Shader::Shader(const string& vertPath, const string& fragPath) : program(glCreateProgram()) {
    string vertCode, fragCode;
    ifstream vertFile, fragFile;

    vertFile.exceptions(ifstream::badbit);
    fragFile.exceptions(ifstream::badbit);

    try {
        vertFile.open(vertPath);
        fragFile.open(fragPath);
        stringstream vertStream, fragStream;

        vertStream << vertFile.rdbuf();
        fragStream << fragFile.rdbuf();

        vertFile.close();
        fragFile.close();

        vertCode = vertStream.str();
        fragCode = fragStream.str();
    } catch (ifstream::failure& e) {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
    }

    const char *vertShaderCode = vertCode.c_str();
    const char *fragShaderCode = fragCode.c_str();

    GLuint vertShader, fragShader;
    vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertShaderCode, nullptr);
    compileShader(vertShader);

    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderCode, nullptr);
    compileShader(fragShader);
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    linkProgram(program);
    glDetachShader(program, vertShader);
    glDetachShader(program, fragShader);
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    GLint uniformCount;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniformCount);
    for (GLint i = 0; i < uniformCount; i++) {
        GLsizei nameLength;
        GLsizei size;
        GLenum type;
        GLchar name[100];
        glGetActiveUniform(program, i, sizeof(name), &nameLength, &size, &type, name);
        uniforms.emplace(name,glGetUniformLocation(program, name));
    }
}

void Shader::use() const {
    glUseProgram(program);
}

GLuint Shader::getAttribLocation(const string& name) const {
    return glGetAttribLocation(program, name.c_str());
}

int Shader::getUniformInt(const string& name) const {
	return uniforms.at(name);
}

void Shader::setInt(int posUniform, int value) {
    glUniform1i(posUniform, value);
}

void Shader::setFloat(int posUniform, float value) {
    glUniform1f(posUniform, value);
}

void Shader::setVec3(int posUniform, float x, float y, float z) {
    glUniform3f(posUniform, x, y, z);
}

void Shader::setMat4(int posUniform, mat4x4 value) {
    glUniformMatrix4fv(posUniform, 1, GL_FALSE, &value[0][0]);
}

Shader::~Shader() {
    glDeleteProgram(program);
}


