#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

// Material properties struct
struct Material {
    vec3 ambient;   // Ambient light
    vec3 diffuse;   // Diffuse reflection
    vec3 specular;  // Specular reflection
    float shininess; // Shininess factor
};

uniform Material material;
// Light properties struct
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform vec3 lightColor;
uniform vec3 viewPos;


float ShadowCalculation(vec4 fragPosLightSpace,float bias)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

void main()
{           
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;

    // Light position
    vec3 lightPos = light.position;

    // Ambient light contribution
    vec3 ambient = lightColor * material.ambient * light.ambient;

    // Diffuse reflection calculation
    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * light.diffuse * (diff * material.diffuse);

    // Specular reflection calculation
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = material.specular * spec * light.specular;

    float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);

    float shadow = ShadowCalculation(fs_in.FragPosLightSpace, bias);
    // Final color calculation
    vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
    
    // Set the output color
    FragColor = vec4(result, 1.0);
}
