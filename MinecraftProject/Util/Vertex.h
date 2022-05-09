//
// Created by amit on 4/21/2022.
//

#pragma once
#include <glm/glm.hpp>

#include "../World/Blocks/BlockTypeList.h"
using glm::vec3;
using glm::vec2;
using glm::vec4;
using glm::uvec2;

struct Vertex
{
	vec3 Position;
	vec2 TexCoords;
	vec4 Color;
	float IndexTexture;
	Vertex(float x, float y, float z, float u, float v, float r, float g, float b, float a, uint16_t indexTexture);

	[[nodiscard]] vec3 GetPosition() const;
	[[nodiscard]] vec2 GetTexCoords() const;
	[[nodiscard]] vec4 GetColor() const;
};
