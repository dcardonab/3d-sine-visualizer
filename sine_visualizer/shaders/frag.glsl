// ===============================================================
#version 330 core

#define NR_POINT_LIGHTS 13

// ============== STRUCTS ==============

// Struct to store a directional light
struct DirLight {
    vec3 direction;

    vec3 color;
    float ambientIntensity;
    float specularStrength;
};

// Struct to store a point light
struct PointLight {
    vec3 position;

    vec3 color;
    float ambientIntensity;
    float specularStrength;

    float constant;
    float linear;
    float quadratic;
};

// ============== UNIFORMS ==============
// Set uniforms from SceneNode::update()
uniform float material_shininess;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform float time;

// ============== IN ==============
// Import data from Vertex Shader
in VS_OUT {
    vec3 Normal;
    vec3 tanFragPos;
    vec3 tanViewPos;
    vec3 tanDirLightPos;
    vec3 tanPointLightsPos[NR_POINT_LIGHTS];
} fs_in;

// ============== OUT ==============
// The final output color of each 'fragment'
out vec4 FragColor;


// Function Prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 tanLightPos, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 HSVtoRGB(vec3 color);


void main() {
    // Properties
    vec3 norm = normalize(fs_in.Normal);
    
    vec3 hsvColor = vec3(tan(time / 1000.0f), (cos(time / 750.0f) + 1) / 2, 1.0f);
    vec3 diffuseColor = HSVtoRGB(hsvColor) * 0.7f;

    vec3 viewDir = normalize(fs_in.tanViewPos - fs_in.tanFragPos);

    // ====== CALCULATE DIRECTIONAL LIGHT
    vec3 Lighting = CalcDirLight(dirLight, norm, viewDir);

    // ====== CALCULATE POINT LIGHTS
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        Lighting += CalcPointLight(pointLights[i], fs_in.tanPointLightsPos[i], norm, fs_in.tanFragPos, viewDir);

    FragColor = vec4(diffuseColor * clamp(Lighting, 0.1f, 1.0f), 1.0f);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    // Find the direction
    vec3 lightDir = normalize(-fs_in.tanDirLightPos);
    // Diffuse shading
    float diffImpact = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess);
    // Combine results
    vec3 ambient  = light.ambientIntensity * light.color;
    vec3 diffuse  = diffImpact * light.color;
    vec3 specular = light.specularStrength * spec * light.color;

    return (ambient + diffuse + specular);
}

// Calculate the color of a point light
vec3 CalcPointLight(PointLight light, vec3 tanLightPos, vec3 normal, vec3 fragPos, vec3 viewDir) {
    // Find the direction
    vec3 lightDir = normalize(tanLightPos - fragPos);
    // Diffuse shading
    float diffImpact = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess);

    // Attenuation
    float distance = length(tanLightPos - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Combine results
    vec3 ambient  = light.ambientIntensity * light.color;
    vec3 diffuse  = diffImpact * light.color;
    vec3 specular = light.specularStrength * spec * light.color;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

// Function to convert colors in HSV format to colors in RGB format.
// REF: https://gamedev.stackexchange.com/questions/59797/glsl-shader-change-hue-saturation-brightness
vec3 HSVtoRGB(vec3 color){
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(color.xxx + K.xyz) * 6.0 - K.www);
    return color.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), color.y);
}
// ==================================================================
