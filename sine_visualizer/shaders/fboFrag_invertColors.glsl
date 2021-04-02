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
    // Compute inverted color for each pixel
    FragColor = vec4(vec3(1.0 - texture(u_DiffuseMap, v_texCoord)), 1.0);
}
// ==================================================================
