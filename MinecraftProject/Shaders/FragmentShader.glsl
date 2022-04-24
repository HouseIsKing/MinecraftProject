#version 460 core
in vec2 outTexture;

out vec4 FragColor;

uniform sampler2D sampler;

void main()
{
	FragColor = texture(sampler, outTexture);
}