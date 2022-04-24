//
// Created by amit on 4/22/2022.
//

#ifndef MINECRAFTPROJECT_TRANSFORM_H
#define MINECRAFTPROJECT_TRANSFORM_H
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

using glm::vec3;
using glm::mat4x4;
using glm::translate;

class Transform {
private:
    vec3 position;
    vec3 rotation;
    vec3 scale;
    mat4x4 transformMatrix;
    bool isDirty;
    void calculateTransformMatrix();
public:
    Transform();
    vec3 getPosition();
    vec3 getRotation();
    void rotate(float x, float y, float z);
    void setRotation(float x, float y, float z);
    void move(float x, float y, float z);
    void setPosition(float x, float y, float z);
    void grow(float x, float y, float z);
    void setScale(float x, float y, float z);
    mat4x4 getTransformMatrix();
};


#endif //MINECRAFTPROJECT_TRANSFORM_H
