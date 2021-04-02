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


void main()
{
    FragColor = texture(u_DiffuseMap, v_texCoord);

    // Assign average value to the output fragment
    FragColor = FragColor;
}
// ==================================================================
