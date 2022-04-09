#version 330 core
out vec4 FragColor;



struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {

    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define NR_POINT_LIGHTS 1


in vec2 TexCoords;
in vec3 surfaceNormal;
in vec3 fragPos;
//in vec3 toCameraVector;

uniform bool useTex = true;
uniform int numPointLights;

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

uniform vec3 viewPos;
uniform float shininess = 32.0;
uniform float reflectivity = 0.5;
uniform vec3 objectColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir , bool useTex);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir,bool useTex);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir,bool useTex);


void main()
{    

	vec3 unitNormal = normalize(surfaceNormal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 result = CalcDirLight(dirLight, unitNormal, viewDir,useTex);
	
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], unitNormal, fragPos, viewDir ,useTex);    

    result += CalcSpotLight(spotLight, unitNormal, fragPos, viewDir,useTex);    

	FragColor = vec4( result, 1.0);


}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir , bool useTex)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float brightness = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // combine results
    
    vec3  ambient ,diffuse,specular;

	if(useTex){

		 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
		 diffuse = light.diffuse * brightness * vec3(texture(texture_diffuse1, TexCoords));  
		 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));

	}

	else{    

		 ambient = light.ambient * objectColor;
	     diffuse = light.diffuse * (brightness *objectColor) ;
	     specular = light.specular * spec * reflectivity;

		}


    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir , bool useTex)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float brightness = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3  ambient ,diffuse,specular;

	if(useTex){

		 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
		 diffuse = light.diffuse * brightness * vec3(texture(texture_diffuse1, TexCoords));  
		 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));

	}

	else{    

		 ambient = light.ambient * objectColor;
	     diffuse = light.diffuse * (brightness *objectColor) ;
	     specular = light.specular * spec * reflectivity;

		}

    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir , bool useTex)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float brightness = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3  ambient ,diffuse,specular;

	if(useTex){
		 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
		 diffuse = light.diffuse * brightness * vec3(texture(texture_diffuse1, TexCoords));  
		 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));

	}
	else{    
		 ambient = light.ambient * objectColor;
	     diffuse = light.diffuse * (brightness *objectColor) ;
	     specular = light.specular * spec * reflectivity;
		}

   
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}