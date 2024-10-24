#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

// Material properties struct
struct Material {
    vec3 ambient;   // Ambient light
    vec3 diffuse;   // Diffuse reflection
    vec3 specular;  // Specular reflection
    float shininess; // Shininess factor
};

uniform Material material;
uniform sampler2D ourTexture;

// Light properties struct
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main()
{
    // Fetch the color from the texture
    vec4 currentColor = texture(ourTexture, TexCoord);
    
    // Light position
    vec3 lightPos = light.position;

    // Ambient light contribution
    vec3 ambient = lightColor * material.ambient * light.ambient;

    // Diffuse reflection calculation
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * light.diffuse * (diff * material.diffuse);

    // Specular reflection calculation
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = material.specular * spec * light.specular;

    // Final color calculation
    vec3 result = (ambient + diffuse + specular) * currentColor.xyz;
    
    // Set the output color
    FragColor = vec4(result, 1.0);
}
