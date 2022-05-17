//
// Created by amit on 4/22/2022.
//

#include "Camera.h"

#include <glad/glad.h>

void Camera::UpdateVectors()
{
    Front.x = cos(radians(Pitch)) * cos(radians(Yaw));
    Front.y = sin(radians(Pitch));
    Front.z = cos(radians(Pitch)) * sin(radians(Yaw));
    Front = normalize(Front);
    Right = normalize(cross(Front, vec3(0, 1, 0)));
    Up = normalize(cross(Right, Front));
    IsDirtyViewMatrix = true;
}

void Camera::SetCameraPosition(const vec3 newPosition)
{
    Position = newPosition;
    IsDirtyViewMatrix = true;
}

Camera::Camera(vec3 position, float aspectRatio) : Front(0, 0, -1), Up(0, 1, 0), Right(1, 0, 0),
                                                   Fov(70), AspectRatio(aspectRatio), Position(position),
                                                   ViewMatrix(), ProjectionMatrix(),
                                                   IsDirtyViewMatrix(true), IsDirtyProjectionMatrix(true),
                                                   PrevYaw(0),
                                                   ZNear(0.05F), ZFar(1000.0F), PrevPitch(0), Pitch(0), Yaw(0)
{
    UpdateVectors();
}


void Camera::SetFov(const float newFov)
{
    Fov = newFov;
    IsDirtyProjectionMatrix = true;
}

mat4x4 Camera::GetViewMatrix()
{
    IsDirtyViewMatrix = true;
    if (IsDirtyViewMatrix)
    {
        RecalculateViewMatrix();
    }
    return ViewMatrix;
}

mat4x4 Camera::GetProjectionMatrix()
{
    if (IsDirtyProjectionMatrix)
    {
        RecalculateProjectionMatrix();
    }
    return ProjectionMatrix;
}

void Camera::RecalculateProjectionMatrix()
{
    ProjectionMatrix = perspective(radians(Fov), AspectRatio, ZNear, ZFar);
    IsDirtyProjectionMatrix = false;
}

void Camera::SetAspectRatio(const float newAspectRatio)
{
    AspectRatio = newAspectRatio;
    IsDirtyProjectionMatrix = true;
    RecalculateProjectionMatrix();
}

void Camera::RecalculateViewMatrix()
{
    PrevYaw = Yaw;
    PrevPitch = Pitch;
    UpdateVectors();
    ViewMatrix = lookAt(Position, Position + Front, Up);
    IsDirtyViewMatrix = false;
}
