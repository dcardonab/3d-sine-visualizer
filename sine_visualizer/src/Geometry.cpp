#include "Geometry.h"
#include <assert.h>
#include <iostream>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/glm.hpp"

// Constructor for the geometry routine
Geometry::Geometry(){
    std::cout << "(Geometry.cpp) Constructor Called\n";
}

// Destructor for geometry
Geometry::~Geometry(){

}


// Automatically adds a vertex and a normal
void Geometry::addVertex(float x, float y, float z){
    vertexPositions.push_back(x);
    vertexPositions.push_back(y);
    vertexPositions.push_back(z);
    // Push back placeholders for normals
    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(1.0f);
    // Push back placeholders for tangents
    Tangents.push_back(0.0f);
    Tangents.push_back(0.0f);
    Tangents.push_back(1.0f);
    // push back placeholders for bi-tangents
    BiTangents.push_back(0.0f);
    BiTangents.push_back(0.0f);
    BiTangents.push_back(1.0f);
}

// Allows for adding one index at a time manually if 
// you know which vertices are needed to make a triangle.
void Geometry::addIndex(unsigned int i) {
    // Simple bounds check to make sure a valid index is added.
    if (i >= 0 && i <= vertexPositions.size()/3) {
        indices.push_back(i);
    } else {
        std::cout << "(Geometry.cpp) ERROR, invalid index\n";
    }
}

// Add texture coordinates
void Geometry::addTexture(float s, float t) {
    textureCoords.push_back(s);
    textureCoords.push_back(t);
}

// Return all of the raw data put into a vertex buffer object
float* Geometry::getData(){
    return allData.data();
}

// Return the total number of data in a vertex buffer object.
// Typically this is useful for calculating the size in bytes
// needed in a vertex buffer object.
unsigned int Geometry::getSize(){
    return allData.size();
}

// Return the size in bytes
unsigned int Geometry::getSizeInBytes(){
    return allData.size()*sizeof(float);
}

// Create all data
void Geometry::gen(){
    // Code will execute when a texture map was provided
    if (textureCoords.size() != 0) {
        assert((vertexPositions.size() / 3) == (textureCoords.size() / 2));

        int coordsPos = 0;
        for(int i =0; i < vertexPositions.size() / 3; i++){
            // vertices
            allData.push_back(vertexPositions[i * 3 + 0]);
            allData.push_back(vertexPositions[i * 3 + 1]);
            allData.push_back(vertexPositions[i * 3 + 2]);
            // normals
            allData.push_back(normals[i * 3 + 0]);
            allData.push_back(normals[i * 3 + 1]);
            allData.push_back(normals[i * 3 + 2]);
            // texture information
            allData.push_back(textureCoords[coordsPos * 2 + 0]); 
            allData.push_back(textureCoords[coordsPos * 2 + 1]); 
            ++coordsPos;
            // tangents
            allData.push_back(Tangents[i * 3 + 0]);
            allData.push_back(Tangents[i * 3 + 1]);
            allData.push_back(Tangents[i * 3 + 2]);
            // bi-tangents
            allData.push_back(BiTangents[i * 3 + 0]);
            allData.push_back(BiTangents[i * 3 + 1]);
            allData.push_back(BiTangents[i * 3 + 2]);
        }
    }
    // Only push vertex positions if no texture was provided
    else {
        for (int i = 0; i < vertexPositions.size() / 3; i++) {
            allData.push_back(vertexPositions[i * 3 + 0]);
            allData.push_back(vertexPositions[i * 3 + 1]);
            allData.push_back(vertexPositions[i * 3 + 2]);
        }
    }
}

// The big trick here, is that when we make a triangle
// We also need to update our normals, tangents, and bi-tangents.
void Geometry::makeTriangle(unsigned int vert0, unsigned int vert1, unsigned int vert2) {
    indices.push_back(vert0);
    indices.push_back(vert1);
    indices.push_back(vert2);

    // Look up the actual vertex positions
    glm::vec3 pos0(vertexPositions[vert0*3 +0], vertexPositions[vert0*3 + 1], vertexPositions[vert0*3 + 2]); 
    glm::vec3 pos1(vertexPositions[vert1*3 +0], vertexPositions[vert1*3 + 1], vertexPositions[vert1*3 + 2]); 
    glm::vec3 pos2(vertexPositions[vert2*3 +0], vertexPositions[vert2*3 + 1], vertexPositions[vert2*3 + 2]); 

    // Look up the texture coordinates
    glm::vec2 tex0(textureCoords[vert0*2 +0], textureCoords[vert0*2 + 1]); 
    glm::vec2 tex1(textureCoords[vert1*2 +0], textureCoords[vert1*2 + 1]); 
    glm::vec2 tex2(textureCoords[vert2*2 +0], textureCoords[vert2*2 + 1]); 

    // Now create an edge
    // With two edges
    // This section is inspired by: https://learnopengl.com/Advanced-Lighting/Normal-Mapping
    glm::vec3 edgeU = pos1 - pos0;
    glm::vec3 edgeV = pos2 - pos0;

    // Compute and assign normals normal
    // REF: https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal
    normals[vert0*3+0] = edgeU.y * edgeV.z - edgeU.z * edgeV.y;
    normals[vert0*3+1] = edgeU.z * edgeV.x - edgeU.x * edgeV.z;
    normals[vert0*3+2] = edgeU.x * edgeV.y - edgeU.y * edgeV.x;

    normals[vert1*3+0] = normals[vert0*3+0];
    normals[vert1*3+1] = normals[vert0*3+1];
    normals[vert1*3+2] = normals[vert0*3+2];

    normals[vert2*3+0] = normals[vert0*3+0];
    normals[vert2*3+1] = normals[vert0*3+1];
    normals[vert2*3+2] = normals[vert0*3+2];


    // Question to ask yourself is what is going on here?
    // The difference of y's and x's? Hmm.
    glm::vec2 deltaUV0 = tex1-tex0;
    glm::vec2 deltaUV1 = tex2-tex0;

    float f = 1.0f / (deltaUV0.x * deltaUV1.y - deltaUV1.x * deltaUV0.y);

    glm::vec3 tangent;
    glm::vec3 bitangent;

    // Compute a tangent
    tangent.x = f * (deltaUV1.y * edgeU.x - deltaUV0.y * edgeV.x);
    tangent.y = f * (deltaUV1.y * edgeU.y - deltaUV0.y * edgeV.y);
    tangent.z = f * (deltaUV1.y * edgeU.z - deltaUV0.y * edgeV.z);
    tangent = glm::normalize(tangent);
    
    // Assign tangent values
    Tangents[vert0*3+0] = tangent.x;
    Tangents[vert0*3+1] = tangent.y;
    Tangents[vert0*3+2] = tangent.z;

    Tangents[vert1*3+0] = tangent.x;
    Tangents[vert1*3+1] = tangent.y;
    Tangents[vert1*3+2] = tangent.z;

    Tangents[vert2*3+0] = tangent.x;
    Tangents[vert2*3+1] = tangent.y;
    Tangents[vert2*3+2] = tangent.z;


    // Compute a bi-tangent
    bitangent.x = f * (-deltaUV1.x * edgeU.x + deltaUV0.x * edgeV.x);
    bitangent.y = f * (-deltaUV1.x * edgeU.y + deltaUV0.x * edgeV.y);
    bitangent.z = f * (-deltaUV1.x * edgeU.z + deltaUV0.x * edgeV.z);
    bitangent = glm::normalize(bitangent);

    // Assign bi-tangent values
    BiTangents[vert0*3+0] = bitangent.x;
    BiTangents[vert0*3+1] = bitangent.y;
    BiTangents[vert0*3+2] = bitangent.z;

    BiTangents[vert1*3+0] = bitangent.x;
    BiTangents[vert1*3+1] = bitangent.y;
    BiTangents[vert1*3+2] = bitangent.z;

    BiTangents[vert2*3+0] = bitangent.x;
    BiTangents[vert2*3+1] = bitangent.y;
    BiTangents[vert2*3+2] = bitangent.z;	
}

// Returns number of indicies
unsigned int Geometry::getIndicesSize(){
    return indices.size();
}

// Returns raw indicies data
unsigned int* Geometry::getIndicesData(){
    return indices.data();
}
