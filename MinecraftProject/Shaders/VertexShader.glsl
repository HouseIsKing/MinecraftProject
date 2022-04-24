#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec4 aColor;

uniform mat4 transformationMatrix;
uniform mat4 view;
uniform mat4 projection;
out vec2 outTexture;

void main()
{
	gl_Position = projection*view*transformationMatrix*vec4(aPos,1.0);
	outTexture = aUV;
}