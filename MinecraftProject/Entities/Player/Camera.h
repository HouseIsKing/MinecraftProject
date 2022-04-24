//
// Created by amit on 4/22/2022.
//

#ifndef MINECRAFTPROJECT_CAMERA_H
#define MINECRAFTPROJECT_CAMERA_H
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

class Camera {
private:
    vec3 front;
    vec3 up;
    vec3 right;
    float fov;
    float aspectRatio;
    vec3 position;
    void updateVectors();
    mat4x4 viewMatrix;
    mat4x4 projectionMatrix;
    bool isDirtyViewMatrix;
    bool isDirtyProjectionMatrix;
    float prevYaw;
    float prevPitch;
public:
    void setCameraPosition(vec3 position);
    Camera(vec3 position, float aspectRatio);
    float pitch;
    float yaw;
    void setFov(float newFov);
    void setAspectRatio(float newAspectRatio);
    void recalculateProjectionMatrix();
    void recalculateViewMatrix();
    mat4x4 getViewMatrix();
    mat4x4 getProjectionMatrix();
};


#endif //MINECRAFTPROJECT_CAMERA_H
