#pragma once
#include "Util/BoundingBox.h"
#include <array>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct Frustum
{
    enum Plane { PlaneBack, PlaneFront, PlaneLeft, PlaneRight, PlaneTop, PlaneBottom };

    Frustum(const glm::mat4x4& viewMatrix, const glm::mat4x4& projectionMatrix);
    [[nodiscard]] bool CubeInFrustum(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const;
    [[nodiscard]] bool CubeInFrustum(const BoundingBox& box) const;

    std::array<glm::vec4, 6> Planes;
};

class Camera
{
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    float Fov;
    float AspectRatio;
    glm::mat4x4 ViewMatrix;
    glm::mat4x4 ProjectionMatrix;
    bool IsDirtyProjectionMatrix;
    float PrevYaw;
    float ZNear;
    float ZFar;
    float PrevPitch;
    void UpdateVectors();

public:
    glm::vec3 Position;
    float Pitch;
    float Yaw;
    Frustum GetFrustum();
    Camera(glm::vec3 position, float aspectRatio);
    void SetFov(float newFov);
    [[nodiscard]] glm::vec3 GetFrontVector() const;
    void SetAspectRatio(float newAspectRatio);
    void RecalculateProjectionMatrix();
    void RecalculateViewMatrix();
    glm::mat4x4 GetViewMatrix();
    glm::mat4x4 GetProjectionMatrix();
};

