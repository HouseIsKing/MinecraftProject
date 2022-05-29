#version 460 core
#extension GL_ARB_bindless_texture : enable

layout(bindless_sampler) uniform;

uniform samplerBlock { uvec2 samplers[32]; };
//uniform vec3 directionalLightDirection;
in float fogFactor;
in vec4 fogColor;
in vec2 outTexture;
in vec4 color;
//in vec3 normal;
in flat int samplerHandle;
//in vec3 FragPos;

out vec4 FragColor;

void main()
{
    //vec3 norm = normalize(normal);
    //vec3 lightDirection = normalize(-directionalLightDirection);
    //float diff = max(dot(norm, lightDirection), 0.0);
	FragColor = color*texture(sampler2D(samplers[samplerHandle]) , outTexture);
	FragColor = mix(FragColor, fogColor, fogFactor);
}
