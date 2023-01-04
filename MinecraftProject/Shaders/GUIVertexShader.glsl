#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec4 aColor;
layout (location = 3) in int aIndex;

out vec2 outTexture;
out vec4 color;
out int samplerHandle;

void main()
{
	gl_Position = vec4(aPos,1.0F);
	outTexture = aUV;
	color = aColor;
	samplerHandle = aIndex;
}