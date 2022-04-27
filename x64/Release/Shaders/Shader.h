//
// Created by amit on 4/22/2022.
//

#ifndef MINECRAFTPROJECT_SHADER_H
#define MINECRAFTPROJECT_SHADER_H
#include <unordered_map>
#include "glad/gl.h"
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
    void setInt(const string& name, int value);
    void setFloat(const string& name, float value);
    void setVec3(const string& name, float x, float y, float z);
    void setMat4(const string& name, mat4x4);
};


#endif //MINECRAFTPROJECT_SHADER_H
