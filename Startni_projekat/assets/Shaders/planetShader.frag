#version 330 core

struct Material {
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	float shininess;
};

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;
in vec3 objectPos;

uniform Material material;
uniform DirLight dirLight;
uniform vec3 viewPos;

uniform vec2 elevationMinMax;
uniform sampler1D colorGradient;

vec3 GetBaseColor();
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 baseColor); 

void main()
{
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 baseColor = GetBaseColor();
    vec3 result = CalcDirLight(dirLight, norm, viewDir, baseColor);
    FragColor = vec4(result, 1.0f);
}

vec3 GetBaseColor()
{
    float range = max(elevationMinMax.y - elevationMinMax.x, 1e-6);
    float t = clamp((length(objectPos) - elevationMinMax.x) / range, 0.0, 1.0);
    return texture(colorGradient, t).rgb;
}

vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir, vec3 baseColor)
{
    vec3 lightDir = normalize(-light.direction);
    
    // diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);
    
    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // combine results
    vec3 ambient  = light.ambient  * baseColor;
    vec3 diffuse  = light.diffuse  * (diff * baseColor);
    vec3 specular = light.specular * (spec * material.specular);
    
    return (ambient + diffuse + specular);
} 