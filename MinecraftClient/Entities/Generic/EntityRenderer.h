#pragma once
#include "Entities/Entity.h"
#include "Util/BoundingBox.h"
#include "Util/TessellationHelper.h"

class EntityRenderer
{
protected:
    const uint16_t EntityId;
    TessellationHelper Tessellation;
    TransformStruct Transform;
    explicit EntityRenderer(uint16_t entityId);

public:
    virtual ~EntityRenderer() = default;
    EntityRenderer(const EntityRenderer&) = delete;
    EntityRenderer& operator=(const EntityRenderer&) = delete;
    EntityRenderer(EntityRenderer&&) noexcept = delete;
    EntityRenderer& operator=(EntityRenderer&&) noexcept = delete;
    virtual void Render(float partialTick);
    virtual const EntityState& GetState() = 0;
    virtual EEntityType GetEntityTypeRenderer() = 0;
};
