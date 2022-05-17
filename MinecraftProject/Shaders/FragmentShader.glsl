#version 460 core
#extension GL_ARB_bindless_texture : enable

layout(bindless_sampler) uniform;

uniform samplerBlock { uvec2 samplers[32]; };
//uniform vec3 directionalLightDirection;
//uniform float ambientLightIntensity;
uniform vec3 fogColor;
in float fogFactor;
in vec2 outTexture;
in vec4 color;
//in vec3 normal;
in float samplerHandle;
//in vec3 FragPos;

out vec4 FragColor;

void main()
{
    //vec3 norm = normalize(normal);
    //vec3 lightDirection = normalize(-directionalLightDirection);
    //float diff = max(dot(norm, lightDirection), 0.0);
    //float finalEffect = diff+ambientLightIntensity;
    int helper = int(round(samplerHandle));
	FragColor = color*texture(sampler2D(samplers[helper]) , outTexture);
	FragColor = mix(FragColor, vec4(fogColor, 1.0), fogFactor);
}
