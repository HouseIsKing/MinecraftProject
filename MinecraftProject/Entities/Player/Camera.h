//
// Created by amit on 4/22/2022.
//

#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

using glm::vec3;
using glm::mat4x4;
using glm::cos;
using glm::sin;
using glm::radians;
using glm::normalize;
using glm::lookAt;
using glm::perspective;

class Camera
{
    vec3 Front;
    vec3 Up;
    vec3 Right;
    float Fov;
    float AspectRatio;
    vec3 Position;
    void UpdateVectors();
    mat4x4 ViewMatrix;
    mat4x4 ProjectionMatrix;
    bool IsDirtyViewMatrix;
    bool IsDirtyProjectionMatrix;
    float PrevYaw;
    float ZNear;
    float ZFar;
    float PrevPitch;
public:
    void SetCameraPosition(vec3 newPosition);
    Camera(vec3 position, float aspectRatio);
    float Pitch;
    float Yaw;
    void SetFov(float newFov);
    void SetAspectRatio(float newAspectRatio);
    void RecalculateProjectionMatrix();
    void RecalculateViewMatrix();
    mat4x4 GetViewMatrix();
    mat4x4 GetProjectionMatrix();
};

