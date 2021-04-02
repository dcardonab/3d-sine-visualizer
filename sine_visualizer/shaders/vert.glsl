// ==================================================================
#version 330 core

#define NR_POINT_LIGHTS 13
// ============== VBO LAYOUTS ==============
layout(location=0)in vec3 position;

// ============== STRUCTS ==============

// Struct to store a directional light.
struct DirLight {
    vec3 direction;

    vec3 color;
    float ambientIntensity;
    float specularStrength;
};

// Struct to store a point light.
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
uniform mat4 model;         // Model to World.
uniform mat4 view;          // World to View.
uniform mat4 projection;    // View to Projection.

uniform vec3 viewPos;   // Used for computing the tangent value
                        // of the view position.

// Lights included in the vertex shader to calculate the tangent
// value of the lights' direction and positions, respectively.
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

// Values to control the rendered wave.
uniform float amplitude;
uniform float waveNumber;
uniform float wavePeriod;

uniform float time; // Offset to automate height.
uniform int planeMode;  // Type of process on the y pos coords.

// ============== OUT ==============
// Export our normal data, and read it into our frag shader.
out VS_OUT {
    vec3 Normal;
    vec3 tanFragPos;
    vec3 tanViewPos;
    vec3 tanDirLightPos;
    vec3 tanPointLightsPos[NR_POINT_LIGHTS];
} vs_out;

// The values for Phase and Vertical Offset were ignored
// in the calculations of sine and cosine, since the change
// in phase did not heavily affect the result to great extents,
// and vertical offset would only shift the figure up as a whole.
float calculateSine(float coord1, float coord2);
mat3 Rotate(float angle, vec3 axis);

// The angular frequency ensures that the number of oscillations
// does not keep increasing with every iteration, as time increases.
float angularFreq = 0.05f;

void main() {
    // Create variables
    // Since the initial mesh is a plane, all normals will be facing
    // out on the Z direction.
    vec3 normal = vec3(0.0f, 0.0f, 1.0f);
    vec3 newPos, newNorm = vec3(0.0f);  // Containers for sine wave
                                        // calculation results.
    
    // Plane modes:
    // (0) Render a plane.
    // (1) Apply a sine wave on the Y axis only using the X coordinate.
    // (2) Apply a sine wave on the Y axis by multiplying both the X and
    //     Z coordinates.
    // Note that the Sine function checks whether to apply mode 1 or 2.
    if (planeMode != 0) {
        float newYpos = calculateSine(position.x, position.z);
        float newYnorm = calculateSine(normal.x, normal.z);

        newPos  = vec3(position.x, newYpos, position.z);
        newNorm = vec3(normal.x, newYnorm, normal.z);
    } else {
        newPos  = vec3(position.x, 0.0f, position.z);
        newNorm = normal;
    }

    // Compute TBN matrix.
    // Calculate normal matrix and normal component.
    mat3 normal_matrix = transpose(inverse(mat3(model)));
    vec3 normal_component = normalize(normal_matrix * newNorm);
    // Since the basic shape is a plane with all of its normals pointing
    // on the positive Z direction, the up vector will be perpendicular,
    // and thus represent the tangent for the calculation of the TBN matrix.
    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    // Compute tangent space by using the normal and cross products of the up
    // vector (tangent) and the normal component. The cross product of the up
    // vector and the normal component is the bitangent.
    mat3 TBN = Rotate(
        acos(dot(up, normal_component)),
        cross(up, normal_component)
    );

    // Apply a circular motion to the directional light, and compute the tangent
    // light direction to increase light precision in the fragment shader.
    vs_out.tanDirLightPos = TBN * vec3(
        dirLight.direction.x + sin(time / 10.0f),
        dirLight.direction.y,
        dirLight.direction.z + cos(time / 10.0f));

    // Compute the position of each point light in tangent space for use in the
    // fragment shader.
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        vs_out.tanPointLightsPos[i] = TBN * pointLights[i].position;
    
    // Compute and pass the tangent view position.
    vs_out.tanViewPos = TBN * viewPos;

    // Compute and pass the tangent fragment position.
    vec3 FragPos = vec3(model * vec4(newPos, 1.0f));
    vs_out.tanFragPos = TBN * FragPos;

    // Pass the normal as is.
    vs_out.Normal = newNorm;

    // Apply the Projection, View, and Model matrices to the vertex position.
    gl_Position = projection * view * model * vec4(newPos, 1.0f);
}

// sineWave = Asin(k(xpos) - w(time) + p) + D
// where:
// A = amplitude
// k = wavenumber = angular freq. / linear speed
   // Higher wavenumber values bring the peaks closer together, meaning there are
   // more waves in a unit of length.
// w = angular freq. (rate of change radians per second) = 2.0 * PI / lambda
   // Smaller w values slow down the movement of the wave.
// lambda = linear speed / frequency
   // A greater lambda results in a slower sine.
// p = phase (in radians)
// D = a non-zero center amplitude
float calculateSine(float coord1, float coord2) {
    if (planeMode == 1) {
        return amplitude * sin((waveNumber / 50 * coord1) - radians(angularFreq) * wavePeriod * time);
    } else {
        return amplitude * sin((waveNumber / 100 * coord1) * ((waveNumber / 100) * coord2) - radians(angularFreq) * wavePeriod * time);
    }
}

// Code adapted from Mike Molisani's video at:
// https://www.youtube.com/watch?v=V4UakVeat_4&feature=youtu.be
mat3 Rotate(float angle, vec3 axis) {
    float c = cos(angle);
    float s = sin(angle);
    float c_diff = 1.0f - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    return mat3(
        c + x * x * c_diff, y * x * c_diff + z * s, z * x * c_diff - y * s,
        x * y * c_diff - z * s, c + y * y * c_diff, z * y * c_diff + x * s,
        x * z * c_diff + y * s, y * z * c_diff - x * s, c + z * z * c_diff
    );
}
// ==================================================================
