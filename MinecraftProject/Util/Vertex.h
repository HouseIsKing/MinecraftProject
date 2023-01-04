//
// Created by amit on 4/21/2022.
//

#pragma once
#include <glm/glm.hpp>
using glm::vec3;
using glm::vec2;
using glm::vec4;
using glm::uvec2;

struct Vertex
{
	vec3 Position;
	vec2 TexCoords;
	vec4 Color;
	int IndexTexture;
	vec3 Normal;
	int Brightness;
	Vertex(float x, float y, float z, float u, float v, float r, float g, float b, float a, uint16_t indexTexture, float normalX, float normalY, float normalZ, int brightness);
	Vertex(const vec3& pos, float u, float v, float r, float g, float b, float a, uint16_t indexTexture, float normalX, float normalY, float normalZ, int brightness);
};
