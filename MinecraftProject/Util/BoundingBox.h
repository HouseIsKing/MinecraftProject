//
// Created by amit on 4/22/2022.
//

#ifndef MINECRAFTPROJECT_BOUNDINGBOX_H
#define MINECRAFTPROJECT_BOUNDINGBOX_H
#include <limits>

using std::numeric_limits;

struct BoundingBox {
private:
    float minX, minY, minZ;
    float maxX, maxY, maxZ;
public:
    BoundingBox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
    BoundingBox(BoundingBox const &other);
    [[nodiscard]] float getMinX() const;
    [[nodiscard]] float getMinY() const;
    [[nodiscard]] float getMinZ() const;
    [[nodiscard]] float getMaxX() const;
    [[nodiscard]] float getMaxY() const;
    [[nodiscard]] float getMaxZ() const;
    void expand(float x, float y, float z);
    void grow(float x, float y, float z);
    void move(float x, float y, float z);
    [[nodiscard]] bool isPointInside(float x, float y, float z) const;
    [[nodiscard]] bool isIntersecting(const BoundingBox& other) const;
    [[nodiscard]] bool isIntersectingX(const BoundingBox& other) const;
    [[nodiscard]] bool isIntersectingY(const BoundingBox& other) const;
    [[nodiscard]] bool isIntersectingZ(const BoundingBox& other) const;
    [[nodiscard]] float clipCollisionX(const BoundingBox& other, float x) const;
    [[nodiscard]] float clipCollisionY(const BoundingBox& other, float y) const;
    [[nodiscard]] float clipCollisionZ(const BoundingBox& other, float z) const;
};


#endif //MINECRAFTPROJECT_BOUNDINGBOX_H
