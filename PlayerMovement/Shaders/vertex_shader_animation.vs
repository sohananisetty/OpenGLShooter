#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout(location = 5) in ivec4 boneIds; 
layout(location = 6) in vec4 weights;


out vec2 TexCoords;
out vec3 surfaceNormal;
out vec3 toLightVector;
out vec3 toCameraVector;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPosition;

uniform bool useAnim;


const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

void main()
{

    vec4 totalPosition = vec4(0.0f);
    vec3 localNormal = vec3(0.0f);

    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(boneIds[i] == -1) 
            continue;
        if(boneIds[i] >=MAX_BONES) 
        {
            totalPosition = vec4(aPos,1.0f);
            break;
        }
        vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(aPos,1.0f);
        totalPosition += localPosition * weights[i];
        vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * aNormal;
    }
	

    vec4 worldPosition = model * totalPosition;   
    gl_Position = projection * view * worldPosition;

    TexCoords = aTexCoords; 

    surfaceNormal = (model* vec4(localNormal,0.0)).xyz;
    toLightVector = lightPosition - worldPosition.xyz;
    toCameraVector = (inverse(view) * vec4(0.0,0.0,0.0,1.0)).xyz - worldPosition.xyz;
	
}