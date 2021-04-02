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
    
    // Compute the average
    // These float values are used to compensate for the human eye being
    // more sensitive to green colors, and less sensitive to blue
    float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;

    // Uncomment to replace value for the true average
    // average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;

    // Assign average value to the output fragment
    FragColor = vec4(average, average, average, 1.0);
}
// ==================================================================
