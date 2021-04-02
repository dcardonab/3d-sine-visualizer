// ====================================================
#version 330 core

// ======================= uniform ====================
// If we have texture coordinates, they are stored in this sampler.
uniform sampler2D u_DiffuseMap; 

// ======================= IN =========================
in vec2 v_texCoord; // Import our texture coordinates from vertex shader

// ======================= out ========================
// The final output color of each 'fragment' from our fragment shader.
out vec4 FragColor;


const float offset = 1.0 / 300.0;

void main()
{
    // Define offsets to retrieve the correct pixels
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset),	// top-left
        vec2(   0.0f,  offset),	// top-center
        vec2( offset,  offset),	// top-right
        vec2(-offset,    0.0f),	// center-left
        vec2(   0.0f,    0.0f),	// center-center
        vec2( offset,    0.0f),	// center-right
        vec2(-offset, -offset),	// bottom-left
        vec2(   0.0f, -offset),	// bottom-center
        vec2( offset, -offset)	// bottom-right
    );

    // Define kernel to process image
    // Edge detection kernel
    float kernel[9] = float[](
        1.0,  1.0, 1.0,
        1.0, -8.0, 1.0,
        1.0,  1.0, 1.0
    );

    // Get textures at offsets positions
    vec3 sampleTex[9];
    for (int i = 0; i < 9; i++) {
        sampleTex[i] = vec3(texture(u_DiffuseMap, v_texCoord.st + offsets[i]));
    }

    // Apply kernel to the pixel color
    vec3 outputColor = vec3(0.0);
    for (int i = 0; i < 9; i++) {
        outputColor += sampleTex[i] * kernel[i];
    }

    // Apply computed color to rendered fragment
    FragColor = vec4(outputColor, 1.0);
}
// ==================================================================
