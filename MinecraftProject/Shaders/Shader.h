//
// Created by amit on 4/22/2022.
//

#ifndef MINECRAFTPROJECT_SHADER_H
#define MINECRAFTPROJECT_SHADER_H
#include <unordered_map>
#include "glad/glad.h"
#include <string>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

using std::string;
using std::unordered_map;
using glm::mat4x4;
using std::cout;
using std::endl;
using std::ifstream;
using std::stringstream;

class Shader {
private:
    unordered_map<string, int> uniforms;
    static void compileShader(GLuint shader);
    static void linkProgram(GLuint program);
public:
    ~Shader();
    const GLuint program;
    Shader(const string& vertPath, const string& fragPath);
    void use() const;
    [[nodiscard]] GLuint getAttribLocation(const string& name) const;
    GLuint GetUniformBlockIndex(const string& name) const;
    int getUniformInt(const string& name) const;
    void setInt(int posUniform, int value);
    void setFloat(int posUniform, float value);
    void setVec3(int posUniform, float x, float y, float z);
    void setMat4(int posUniform, mat4x4);
};


#endif //MINECRAFTPROJECT_SHADER_H
