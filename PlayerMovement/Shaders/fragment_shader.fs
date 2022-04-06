#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;

uniform bool useTex = true;

struct Light {  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform float shininess = 12.0;
uniform float reflectivity = 0.5;
uniform vec3 objectColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;
void main()
{    


    vec3 unitNormal = normalize(surfaceNormal);
    vec3 unitLightVector = normalize(toLightVector);

    float nDotl = dot(unitNormal,unitLightVector);
    float brightness = max(nDotl,0.2);

    vec3 unitVectorToCamera = normalize(toCameraVector);
    vec3 lightDirection = -unitLightVector;
    vec3 reflectedLightDirection = reflect(lightDirection,unitNormal);

    float specularFactor = dot(reflectedLightDirection , unitVectorToCamera);
    specularFactor = max(specularFactor,0.0);
    float spec = pow(specularFactor,shininess);


	if(useTex){

		vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
		vec3 finalSpecular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
		vec3 diffuse = light.diffuse * brightness * vec3(texture(texture_diffuse1, TexCoords));  
		FragColor = vec4(ambient + diffuse + finalSpecular, 1.0);   

	}

	else{

		vec3 ambient = light.ambient * objectColor;
	    vec3 diffuse = light.diffuse * (brightness *objectColor) ;
	    vec3 finalSpecular = light.specular * spec * reflectivity;
	    vec3 result =  diffuse + ambient + finalSpecular;
	    FragColor = vec4( result, 1.0);

		}
}