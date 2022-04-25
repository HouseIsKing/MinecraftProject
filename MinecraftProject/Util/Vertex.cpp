//
// Created by amit on 4/21/2022.
//

#include "Vertex.h"

vec4 Vertex::getColor() const {
    return color;
}

vec2 Vertex::getTexCoords() const {
    return texCoords;
}

vec3 Vertex::getPosition() const {
    return position;
}

Vertex::Vertex(float x, float y, float z, float u, float v, float r, float g, float b, float a) : usage(VertexUsage::POSITION_AND_TEXTURE_COORDS_AND_COLOR), position(x,y,z), texCoords(u,v), color(r,g,b,a) {}

Vertex::Vertex(float x, float y, float z, float r, float g, float b, float a) : usage(VertexUsage::POSITION_AND_COLOR) ,position(x,y,z), color(r,g,b,a), texCoords(0,0) {}

VertexUsage Vertex::getVertexUsage() const {
    return usage;
}
