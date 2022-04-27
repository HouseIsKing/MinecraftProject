//
// Created by amit on 4/22/2022.
//

#ifndef MINECRAFTPROJECT_ENTITY_H
#define MINECRAFTPROJECT_ENTITY_H
//#include "../World/SinglePlayerWorld.h"
#include <glm/glm.hpp>
#include "../Util/TessellationHelper.h"
#include "../Util/EngineDefaults.h"
#include "../Util/BoundingBox.h"

class SinglePlayerWorld;

using glm::vec3;

class Entity {
private:
    static SinglePlayerWorld* world;
    bool isGrounded;
    vec3 entitySize;
    const uint16_t entityID;
protected:
    float velocityX;
    float velocityY;
    float velocityZ;
    TessellationHelper tessellationHelper;
    Entity(uint16_t entityID, vec3 entitySize,float x, float y, float z);
    [[nodiscard]] bool IsGrounded() const;
    void virtual Render();
    void checkCollisionAndMove();
    virtual void tick();
public:
    virtual ~Entity();
    static void setWorld(SinglePlayerWorld* newWorld);
    static SinglePlayerWorld* getWorld();
    void doTick();
    Transform& getTransform();
};
#endif //MINECRAFTPROJECT_ENTITY_H
