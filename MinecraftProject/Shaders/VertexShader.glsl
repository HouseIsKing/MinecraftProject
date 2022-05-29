#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec4 aColor;
layout (location = 3) in int aIndex;
layout (location = 4) in vec3 aNormal;
layout (location = 5) in int brightness;

struct Fog
{
    vec4 fogColor;
    float fogDensity;
};

uniform fogBlock { Fog fogs[2]; };
uniform mat4 transformationMatrix;
uniform mat4 view;
uniform mat4 projection;
out vec2 outTexture;
out vec4 color;
out vec3 normal;
out int samplerHandle;
out float fogFactor;
out vec4 fogColor;
//out vec3 FragPos;

void main()
{
    //FragPos = vec3(transformationMatrix * vec4(aPos, 1.0));
    vec4 posRelativeToCamera = view * transformationMatrix * vec4(aPos, 1.0);
	gl_Position = projection*posRelativeToCamera;
	outTexture = aUV;
	color = aColor;
    //normal = aNormal;
	samplerHandle = aIndex;
	float distance = length(posRelativeToCamera.xyz);
	fogColor = fogs[brightness].fogColor;
    fogFactor = 1.0F - exp(-fogs[brightness].fogDensity * distance);
}