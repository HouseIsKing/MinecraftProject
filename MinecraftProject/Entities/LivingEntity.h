//
// Created by amit on 4/22/2022.
//

#ifndef MINECRAFTPROJECT_LIVINGENTITY_H
#define MINECRAFTPROJECT_LIVINGENTITY_H
#include "Entity.h"

using glm::sin;
using glm::cos;
using glm::radians;

class LivingEntity : public Entity {
private:
    void calculateVelocity();
protected:
    bool jumpRequested;
    float horizontalInput;
    float verticalInput;
    LivingEntity(uint16_t entityID, vec3 entitySize, float x, float y, float z);
    void tick() override;
};


#endif //MINECRAFTPROJECT_LIVINGENTITY_H
