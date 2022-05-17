#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec4 aColor;
layout (location = 3) in float aIndex;
layout (location = 4) in vec3 aNormal;
layout (location = 5) in float brightness;

uniform mat4 transformationMatrix;
uniform mat4 view;
uniform mat4 projection;
uniform float fogStart;
uniform float fogEnd;
out vec2 outTexture;
out vec4 color;
out vec3 normal;
out float fogFactor;
out float samplerHandle;
//out vec3 FragPos;

void main()
{
    //FragPos = vec3(transformationMatrix * vec4(aPos, 1.0));
    vec4 posRelativeToCamera = view * transformationMatrix * vec4(aPos, 1.0);
	gl_Position = projection*posRelativeToCamera;
	outTexture = aUV;
	color = aColor;
	color = vec4(color.rgb * brightness, color.a);
    //normal = aNormal;
	samplerHandle = aIndex;
	float distance = length(posRelativeToCamera.xyz);
    fogFactor = clamp((distance-fogStart)/(fogEnd-fogStart), 0.0, 1.0);
    if(brightness > 0.999F)
    {
        fogFactor = 0.0F;
    }
}