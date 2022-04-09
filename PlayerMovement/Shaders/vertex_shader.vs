#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;


out vec2 TexCoords;
out vec3 surfaceNormal;
out vec3 fragPos;
//out vec3 toCameraVector;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{

    vec4 worldPosition = model * vec4(aPos,1.0);
    fragPos = vec3(worldPosition);   
    gl_Position = projection * view * worldPosition;

    TexCoords = aTexCoords; 

    surfaceNormal = mat3(transpose(inverse(model))) * aNormal;  
    //toCameraVector = (inverse(view) * vec4(0.0,0.0,0.0,1.0)).xyz - worldPosition.xyz;

   
	
}

