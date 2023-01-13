#pragma once
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 Position;
	glm::vec2 TexCoords;
	glm::vec4 Color;
	int IndexTexture;
	glm::vec3 Normal;
	int Brightness;
	unsigned int SpecialFactors;
	Vertex(float x, float y, float z, float u, float v, float r, float g, float b, float a, uint16_t indexTexture, float normalX, float normalY, float normalZ, int brightness);
	Vertex(const glm::vec3& pos, float u, float v, float r, float g, float b, float a, uint16_t indexTexture, float normalX, float normalY, float normalZ, int brightness);
	Vertex(float x, float y, float z, float u, float v, float r, float g, float b, float a, uint16_t indexTexture, float normalX, float normalY, float normalZ, int brightness, unsigned int specialFactors);
};
