//
// Created by amit on 4/22/2022.
//

#include "Transform.h"

Transform::Transform() : position(vec3(0.0f)), rotation(vec3(0.0f)), scale(vec3(1.0f)), transformMatrix(1.0f), isDirty(true) {
}

void Transform::calculateTransformMatrix() {
    transformMatrix = translate(position);
    transformMatrix = glm::rotate(transformMatrix, glm::radians(rotation.x), vec3(1.0f, 0.0f, 0.0f));
    transformMatrix = glm::rotate(transformMatrix, glm::radians(rotation.y), vec3(0.0f, 1.0f, 0.0f));
    transformMatrix = glm::rotate(transformMatrix, glm::radians(rotation.z), vec3(0.0f, 0.0f, 1.0f));
    transformMatrix = glm::scale(transformMatrix, scale);
    isDirty = false;
}

vec3 Transform::getPosition() {
    return position;
}

vec3 Transform::getRotation() {
    return rotation;
}

void Transform::rotate(float x, float y, float z) {
    rotation.x += x;
    rotation.y += y;
    rotation.z += z;
    isDirty = true;
}

void Transform::setRotation(float x, float y, float z) {
    rotation.x = x;
    rotation.y = y;
    rotation.z = z;
    isDirty = true;
}

void Transform::move(float x, float y, float z) {
    position.x += x;
    position.y += y;
    position.z += z;
    isDirty = true;
}

void Transform::setPosition(float x, float y, float z) {
    position.x = x;
    position.y = y;
    position.z = z;
    isDirty = true;
}

void Transform::grow(float x, float y, float z) {
    scale.x += x;
    scale.y += y;
    scale.z += z;
    isDirty = true;
}

void Transform::setScale(float x, float y, float z) {
    scale.x = x;
    scale.y = y;
    scale.z = z;
    isDirty = true;
}

mat4x4 Transform::getTransformMatrix() {
    if(isDirty)
        calculateTransformMatrix();
    return transformMatrix;
}
