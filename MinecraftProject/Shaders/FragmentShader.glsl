#version 460 core
in vec2 outTexture;
in vec4 color;

out vec4 FragColor;

uniform sampler2D sampler;

void main()
{
	FragColor = color*texture(sampler, outTexture);
}