//
// Created by amit on 4/22/2022.
//

#include "BoundingBox.h"

float BoundingBox::getMinX() const {
    return minX;
}

float BoundingBox::getMinY() const {
    return minY;
}

float BoundingBox::getMinZ() const {
    return minZ;
}

float BoundingBox::getMaxX() const {
    return maxX;
}

float BoundingBox::getMaxY() const {
    return maxY;
}

float BoundingBox::getMaxZ() const {
    return maxZ;
}

BoundingBox::BoundingBox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) {
    this->minX = minX;
    this->minY = minY;
    this->minZ = minZ;
    this->maxX = maxX;
    this->maxY = maxY;
    this->maxZ = maxZ;
}

void BoundingBox::expand(float x, float y, float z) {
    if(x< 0) {
        minX += x;
    } else {
        maxX += x;
    }
    if(y < 0) {
        minY += y;
    } else {
        maxY += y;
    }
    if(z < 0) {
        minZ += z;
    } else {
        maxZ += z;
    }
}

void BoundingBox::grow(float x, float y, float z) {
    minX -= x;
    minY -= y;
    minZ -= z;
    maxX += x;
    maxY += y;
    maxZ += z;
}

void BoundingBox::move(float x, float y, float z) {
    minX += x;
    minY += y;
    minZ += z;
    maxX += x;
    maxY += y;
    maxZ += z;
}

bool BoundingBox::isPointInside(float x, float y, float z) const {
    return x >= minX && x <= maxX && y >= minY && y <= maxY && z >= minZ && z <= maxZ;
}

bool BoundingBox::isIntersecting(const BoundingBox& other) const {
    return isIntersectingX(other) && isIntersectingY(other) && isIntersectingZ(other);
}

bool BoundingBox::isIntersectingX(const BoundingBox& other) const {
    return minX <= other.getMaxX() && maxX >= other.getMinX();
}

bool BoundingBox::isIntersectingY(const BoundingBox& other) const {
    return minY <= other.getMaxY() && maxY >= other.getMinY();
}

bool BoundingBox::isIntersectingZ(const BoundingBox& other) const {
    return minZ <= other.getMaxZ() && maxZ >= other.getMinZ();
}

float BoundingBox::clipCollisionX(const BoundingBox& other, float x) const {
    if(isIntersectingY(other) && isIntersectingZ(other)){
        if(x<0 && minX >= other.getMaxX()) {
            float max = other.getMaxX() - minX + numeric_limits<float>::epsilon();
            if(max > x)
                x = max;
        }
        else if(x>0 && maxX <= other.getMinX()) {
            float min = other.getMinX() - maxX - numeric_limits<float>::epsilon();
            if(min < x)
                x = min;
        }
    }
    return x;
}

float BoundingBox::clipCollisionY(const BoundingBox& other, float y) const {
    if(isIntersectingX(other) && isIntersectingZ(other)){
        if(y<0 && minY >= other.getMaxY()) {
            float max = other.getMaxY() - minY + numeric_limits<float>::epsilon();
            if(max > y)
                y = max;
        }
        else if(y>0 && maxY <= other.getMinY()) {
            float min = other.getMinY() - maxY - numeric_limits<float>::epsilon();
            if(min < y)
                y = min;
        }
    }
    return y;
}

float BoundingBox::clipCollisionZ(const BoundingBox& other, float z) const {
    if(isIntersectingX(other) && isIntersectingY(other)){
        if(z<0 && minZ >= other.getMaxZ()) {
            float max = other.getMaxZ() - minZ + numeric_limits<float>::epsilon();
            if(max > z)
                z = max;
        }
        else if(z>0 && maxZ <= other.getMinZ()) {
            float min = other.getMinZ() - maxZ - numeric_limits<float>::epsilon();
            if(min < z)
                z = min;
        }
    }
    return z;
}

BoundingBox::BoundingBox(const BoundingBox &other) : minX(other.getMinX()), minY(other.getMinY()), minZ(other.getMinZ()), maxX(other.getMaxX()), maxY(other.getMaxY()), maxZ(other.getMaxZ()) {
}
