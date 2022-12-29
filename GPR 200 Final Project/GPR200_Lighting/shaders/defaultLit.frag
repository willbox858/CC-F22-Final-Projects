#version 330
out vec4 FragColor;

in vec3 Color;

in vec3 WorldPos;
in vec3 WorldNormal;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct SpotLight
{
    vec3  position;
    vec3  direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
};

struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Light
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

vec3 CalculateDirectionalLighting(DirLight light, vec3 normal, vec3 cameraDirection);

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 cameraDirection);

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 cameraDirection, vec3 cameraPosition);

uniform vec3 uEyePos;

uniform Material material;

uniform DirLight dirLight;

uniform SpotLight sptLight;

#define NR_POINT_LIGHTS 2
uniform PointLight pointLights[NR_POINT_LIGHTS];

void main()
{
    vec3 normal = normalize(WorldNormal);
    vec3 viewDirection = normalize(uEyePos - WorldPos);

    vec3 totalLight = CalculateDirectionalLighting(dirLight,normal,viewDirection);

    totalLight += CalculateSpotLight(sptLight,normal,WorldPos,viewDirection,uEyePos);

    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        totalLight += CalculatePointLight(pointLights[i], normal, WorldPos,viewDirection);

    FragColor = vec4(totalLight,1.0f);
};

vec3 CalculateDirectionalLighting(DirLight light, vec3 normal, vec3 cameraDirection)
{
    vec3 toLight = normalize(-light.direction);
    
    float d = max(dot(normal,toLight),0.0);

    vec3 reflectDir = reflect(-toLight,normal);
    float s = pow(max(dot(cameraDirection,reflectDir),0.0),material.shininess);

    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * d * material.diffuse;
    vec3 specular = light.specular * s * material.specular;

    return (ambient + diffuse + specular);
};

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 cameraDirection)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float d = max(dot(normal,lightDir),0.0);

    vec3 reflectDir = reflect(-lightDir,normal);
    float s = pow(max(dot(cameraDirection,reflectDir),0.0),material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * d * material.diffuse;
    vec3 specular = light.specular * s * material.specular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
};

 vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 cameraDirection, vec3 cameraPosition)
 {
    vec3 lightDir = normalize(light.position - fragPos);

    float theta = dot(lightDir, normalize(-light.direction));

    if(theta > light.cutOff)
    {
        //ambient
        vec3 ambient = light.ambient * material.ambient;

        //diffuse
        vec3 norm = normalize(normal);
        float d = max(dot(norm,lightDir),0.0);
        vec3 diffuse = light.diffuse * d * material.diffuse;

        //specular
        vec3 viewDirection = normalize(cameraPosition - fragPos);
        vec3 reflectDir = reflect(-lightDir,normal);
        float s = pow(max(dot(viewDirection,reflectDir),0.0),material.shininess);
        vec3 specular = light.specular * s * material.specular;

        //attenuation
        float distance    = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
        
        ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;

        return (ambient + diffuse + specular);
    }

    return vec3(0.0f);
 };
 
 