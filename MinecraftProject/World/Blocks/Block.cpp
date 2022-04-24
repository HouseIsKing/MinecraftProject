//
// Created by amit on 4/22/2022.
//

#include "Block.h"

#include <utility>

Block* Block::getAirBlock() {
    return new Block(BlockTypeList::getBlockType(EBlockType::AIR),0,0,0);
}

Block::Block(const BlockType* type, float x, float y, float z) : Block(type,x,y,z,x+1,y+1,z+1) {
}

Block::Block(const BlockType* type, float minX, float minY, float minZ, float maxX, float maxY, float maxZ) : UVsOverride(false),
                                                                                                       UVs(array<float,24>()),
                                                                                                       boundingBox(BoundingBox(minX, minY, minZ, maxX, maxY, maxZ)), type(type) {

}

Block::Block(const BlockType* type, float minX, float minY, float minZ, float maxX, float maxY, float maxZ,
             array<float, 24> UVs) : UVsOverride(true), UVs(UVs), boundingBox(BoundingBox(minX, minY, minZ, maxX, maxY, maxZ)), type(type) {

}

bool Block::isAirBlock() const {
    return type->getType() == EBlockType::AIR;
}

BoundingBox &Block::getBoundingBox() {
    return boundingBox;
}

void Block::generateTessellationData(TessellationHelper &tessellationHelper, BlockFaces face) {
    uint16_t index1 = 0, index2 = 0, index3 = 0, index4 = 0;
    float uMin, uMax, vMin, vMax;
    switch(face)
    {
    case BlockFaces::TOP:
            if(UVsOverride)
            {
                uMin = UVs[0];
                uMax = UVs[1];
                vMin = UVs[2];
                vMax = UVs[3];
            }
            else
            {
                uMin = boundingBox.getMinX()- floor(boundingBox.getMinX());
                uMax = boundingBox.getMaxX()-boundingBox.getMinX();
                vMin = boundingBox.getMinZ()- floor(boundingBox.getMinZ());
                vMax = boundingBox.getMaxZ()-boundingBox.getMinZ();
            }
            index1 = tessellationHelper.addVertex(Vertex(boundingBox.getMinX(), boundingBox.getMaxY(), boundingBox.getMinZ(), uMin, vMin));
            index2 = tessellationHelper.addVertex(Vertex(boundingBox.getMinX(), boundingBox.getMaxY(), boundingBox.getMaxZ(), uMin, vMax));
            index3 = tessellationHelper.addVertex(Vertex(boundingBox.getMaxX(), boundingBox.getMaxY(), boundingBox.getMinZ(), uMax, vMin));
            index4 = tessellationHelper.addVertex(Vertex(boundingBox.getMaxX(), boundingBox.getMaxY(), boundingBox.getMaxZ(), uMax, vMax));
            break;
    case BlockFaces::BOTTOM:
            if(UVsOverride)
            {
                uMin = UVs[4];
                uMax = UVs[5];
                vMin = UVs[6];
                vMax = UVs[7];
            }
            else
            {
                uMin = boundingBox.getMinX()- floor(boundingBox.getMinX());
                uMax = boundingBox.getMaxX()-boundingBox.getMinX();
                vMin = boundingBox.getMinZ()- floor(boundingBox.getMinZ());
                vMax = boundingBox.getMaxZ()-boundingBox.getMinZ();
            }
            index1 = tessellationHelper.addVertex(Vertex(boundingBox.getMinX(), boundingBox.getMinY(), boundingBox.getMinZ(), uMin, vMin));
            index2 = tessellationHelper.addVertex(Vertex(boundingBox.getMaxX(), boundingBox.getMinY(), boundingBox.getMinZ(), uMax, vMin));
            index3 = tessellationHelper.addVertex(Vertex(boundingBox.getMinX(), boundingBox.getMinY(), boundingBox.getMaxZ(), uMin, vMax));
            index4 = tessellationHelper.addVertex(Vertex(boundingBox.getMaxX(), boundingBox.getMinY(), boundingBox.getMaxZ(), uMax, vMax));
            break;
    case BlockFaces::SOUTH:
            if(UVsOverride)
            {
                uMin = UVs[8];
                uMax = UVs[9];
                vMin = UVs[10];
                vMax = UVs[11];
            }
            else
            {
                uMin = boundingBox.getMinX()- floor(boundingBox.getMinX());
                uMax = boundingBox.getMaxX()-boundingBox.getMinX();
                vMin = boundingBox.getMinY()- floor(boundingBox.getMinY());
                vMax = boundingBox.getMaxY()-boundingBox.getMinY();
            }
            index1 = tessellationHelper.addVertex(Vertex(boundingBox.getMinX(), boundingBox.getMinY(), boundingBox.getMinZ(), uMin, vMin));
            index2 = tessellationHelper.addVertex(Vertex(boundingBox.getMinX(), boundingBox.getMaxY(), boundingBox.getMinZ(), uMin, vMax));
            index3 = tessellationHelper.addVertex(Vertex(boundingBox.getMaxX(), boundingBox.getMinY(), boundingBox.getMinZ(), uMax, vMin));
            index4 = tessellationHelper.addVertex(Vertex(boundingBox.getMaxX(), boundingBox.getMaxY(), boundingBox.getMinZ(), uMax, vMax));
            break;
    case BlockFaces::NORTH:
            if(UVsOverride)
            {
                uMin = UVs[12];
                uMax = UVs[13];
                vMin = UVs[14];
                vMax = UVs[15];
            }
            else
            {
                uMin = boundingBox.getMinX()- floor(boundingBox.getMinX());
                uMax = boundingBox.getMaxX()-boundingBox.getMinX();
                vMin = boundingBox.getMinY()- floor(boundingBox.getMinY());
                vMax = boundingBox.getMaxY()-boundingBox.getMinY();
            }
            index1 = tessellationHelper.addVertex(Vertex(boundingBox.getMinX(), boundingBox.getMinY(), boundingBox.getMaxZ(), uMin, vMin));
            index2 = tessellationHelper.addVertex(Vertex(boundingBox.getMaxX(), boundingBox.getMinY(), boundingBox.getMaxZ(), uMax, vMin));
            index3 = tessellationHelper.addVertex(Vertex(boundingBox.getMinX(), boundingBox.getMaxY(), boundingBox.getMaxZ(), uMin, vMax));
            index4 = tessellationHelper.addVertex(Vertex(boundingBox.getMaxX(), boundingBox.getMaxY(), boundingBox.getMaxZ(), uMax, vMax));
            break;
    case BlockFaces::EAST:
            if(UVsOverride)
            {
                uMin = UVs[16];
                uMax = UVs[17];
                vMin = UVs[18];
                vMax = UVs[19];
            }
            else
            {
                uMin = boundingBox.getMinZ()- floor(boundingBox.getMinZ());
                uMax = boundingBox.getMaxZ()-boundingBox.getMinZ();
                vMin = boundingBox.getMinY()- floor(boundingBox.getMinY());
                vMax = boundingBox.getMaxY()-boundingBox.getMinY();
            }
            index1 = tessellationHelper.addVertex(Vertex(boundingBox.getMaxX(), boundingBox.getMinY(), boundingBox.getMinZ(), uMin, vMin));
            index2 = tessellationHelper.addVertex(Vertex(boundingBox.getMaxX(), boundingBox.getMaxY(), boundingBox.getMinZ(), uMin, vMax));
            index3 = tessellationHelper.addVertex(Vertex(boundingBox.getMaxX(), boundingBox.getMinY(), boundingBox.getMaxZ(), uMax, vMin));
            index4 = tessellationHelper.addVertex(Vertex(boundingBox.getMaxX(), boundingBox.getMaxY(), boundingBox.getMaxZ(), uMax, vMax));
            break;
    case BlockFaces::WEST:
            if(UVsOverride)
            {
                uMin = UVs[20];
                uMax = UVs[21];
                vMin = UVs[22];
                vMax = UVs[23];
            }
            else
            {
                uMin = boundingBox.getMinZ()- floor(boundingBox.getMinZ());
                uMax = boundingBox.getMaxZ()-boundingBox.getMinZ();
                vMin = boundingBox.getMinY()- floor(boundingBox.getMinY());
                vMax = boundingBox.getMaxY()-boundingBox.getMinY();
            }
            index1 = tessellationHelper.addVertex(Vertex(boundingBox.getMinX(), boundingBox.getMinY(), boundingBox.getMinZ(), uMin, vMin));
            index2 = tessellationHelper.addVertex(Vertex(boundingBox.getMinX(), boundingBox.getMinY(), boundingBox.getMaxZ(), uMax, vMin));
            index3 = tessellationHelper.addVertex(Vertex(boundingBox.getMinX(), boundingBox.getMaxY(), boundingBox.getMinZ(), uMin, vMax));
            index4 = tessellationHelper.addVertex(Vertex(boundingBox.getMinX(), boundingBox.getMaxY(), boundingBox.getMaxZ(), uMax, vMax));
            break;
    }
    Texture faceMaterial = getFaceTexture(face);
    tessellationHelper.addTriangle(faceMaterial, index1);
    tessellationHelper.addTriangle(faceMaterial, index2);
    tessellationHelper.addTriangle(faceMaterial, index3);
    tessellationHelper.addTriangle(faceMaterial, index4);
    tessellationHelper.addTriangle(faceMaterial, index3);
    tessellationHelper.addTriangle(faceMaterial, index2);

}

Texture Block::getFaceTexture(BlockFaces face) const {
    return type->getTextures()[0];
}
