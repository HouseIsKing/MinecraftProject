//
// Created by amit on 4/22/2022.
//

#include "Camera.h"
#include <iostream>

void Camera::updateVectors() {
    front.x = cos(radians(pitch)) * cos(radians(yaw));
    front.y = sin(radians(pitch));
    front.z = cos(radians(pitch)) * sin(radians(yaw));
    front = normalize(front);
    right = normalize(cross(front, vec3(0, 1, 0)));
    up = normalize(cross(right, front));
    isDirtyViewMatrix = true;
}

void Camera::setCameraPosition(vec3 newPosition) {
    position = newPosition;
    isDirtyViewMatrix = true;
}

Camera::Camera(vec3 startPosition, float aspectRatio) : position(startPosition), aspectRatio(aspectRatio), yaw(0), pitch(0), front(0, 0, -1), right(1, 0, 0), up(0, 1, 0), fov(60), viewMatrix(), projectionMatrix(),isDirtyProjectionMatrix(true), isDirtyViewMatrix(true), prevPitch(0),prevYaw(0) {
    updateVectors();
}

void Camera::setFov(float newFov) {
    fov = newFov;
    isDirtyProjectionMatrix = true;
}

mat4x4 Camera::getViewMatrix() {
    if(prevYaw != yaw || prevPitch != pitch)
        isDirtyViewMatrix = true;
    if(isDirtyViewMatrix)
        recalculateViewMatrix();
    return viewMatrix;
}

mat4x4 Camera::getProjectionMatrix() {
    if(isDirtyProjectionMatrix)
        recalculateProjectionMatrix();
    return projectionMatrix;
}

void Camera::recalculateProjectionMatrix() {
    projectionMatrix = perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);
    isDirtyProjectionMatrix= false;
}

void Camera::setAspectRatio(float newAspectRatio) {
    aspectRatio = newAspectRatio;
    isDirtyProjectionMatrix = true;
}

void Camera::recalculateViewMatrix() {
    prevYaw = yaw;
    prevPitch = pitch;
    updateVectors();
    viewMatrix = lookAt(position, position + front, up);
    isDirtyViewMatrix = false;
}
