#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;

uniform bool useLight;

uniform vec3 lightColour;
uniform float shineDamper;
uniform float reflectivity;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;
void main()
{    

	if(useLight){

	  vec3 unitNormal = normalize(surfaceNormal);
	  vec3 unitLightVector = normalize(toLightVector);
	
	  float nDotl = dot(unitNormal,unitLightVector);
	  float brightness = max(nDotl,0.2);
	  vec3 diffuse = brightness * lightColour;

	  vec3 unitVectorToCamera = normalize(toCameraVector);
	  vec3 lightDirection = -unitLightVector;
	  vec3 reflectedLightDirection = reflect(lightDirection,unitNormal);
	
	  float specularFactor = dot(reflectedLightDirection , unitVectorToCamera);
	  specularFactor = max(specularFactor,0.0);
	  float dampedFactor = pow(specularFactor,shineDamper);
	  vec3 finalSpecular = dampedFactor * reflectivity * lightColour;
	
	  FragColor =  vec4(diffuse,1.0) * texture(texture_diffuse1, TexCoords) + vec4(finalSpecular,1.0);


	}
	else
	  FragColor = texture(texture_diffuse1, TexCoords);
}