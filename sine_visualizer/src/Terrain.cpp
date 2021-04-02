#include "Terrain.h"
#include "Image.h"

// Constructor for our object
// Calls the initialization method
Terrain::Terrain(int xSegs, int zSegs) : xSegments(xSegs), zSegments(zSegs) {
    std::cout << "(Terrain.cpp) Constructor Called \n";
    init();
}

Terrain::Terrain(int xSegs, int zSegs, std::string fileName) : xSegments(xSegs), zSegments(zSegs) {
    std::cout << "(Terrain.cpp) Constructor Called \n";

    // Load up some image data
    Image heightMap(fileName);
    heightMap.loadPPM(true);
    // Set the height data for the image
    // TODO: Currently there is a 1-1 mapping between a pixel and a segment
    // You might consider interpolating values if there are more segments
    // than pixels. 
    float scale = 2.0f; // Note that this scales down the values
                        // to make the image a bit more flat.
                        
    // Create height data
    heightData = new int[xSegments*zSegments];
    // Set the height data equal to the grayscale value of the heightmap
    // Because the R,G,B will all be equal in a grayscale image, then
    // we just grab one of the color components.
    for(int z = 0 ; z < zSegments; ++z){
        for(int x = 0; x < xSegments; ++x){
            heightData[x+z*xSegments] = (float)heightMap.getPixelR(z,x)/scale;
        }
    }
    init();
}

// Destructor
Terrain::~Terrain(){
    if(heightData!=nullptr){
        delete heightData;
    }
}

// Creates a grid of segments
// This article has a pretty handy illustration here:
// http://www.learnopengles.com/wordpress/wp-content/uploads/2012/05/vbo.png
// of what we are trying to do.
void Terrain::init(){
    // Create a terrain ia texture was provided.
    if (!m_texturePath.empty()) {
        // Create the initial grid of vertices.
        for(int z = 0 ; z < zSegments; ++z){
            for(int x = 0; x < xSegments; ++x){
                // z is negative so we build the heightmap 'away' from the camera
                geometry.addVertex(x - xSegments / 2.0f, heightData[x+z*xSegments], z - zSegments / 2.0f);   // Position
                // Calculate the texture
                float u = 1.0f - ((float)x/(float)xSegments);
                float v = 1.0f - ((float)z/(float)zSegments);
                geometry.addTexture(u, v);    // Texture
            }
        }
        // Figure out which indices make up each triangle
        for(int z = 0 ; z < zSegments - 1; ++z){
            // Degenerate triangles are used to create more than one row,
            // and they are created by repeating the last vertex of the
            // previous row (see below), and the first vertex of the new row.
            // This is the degenerate begin, which repeats the first vertex.
            if (z > 0)
                geometry.addIndex(z * zSegments);
                
            for(int x = 0; x < xSegments - 1; ++x){
                geometry.makeTriangle(
                    x + (z * zSegments),
                    x + (z * zSegments) + xSegments,
                    x + (z * zSegments) + 1
                );

                geometry.makeTriangle(
                    x + (z * zSegments) + 1,
                    x + (z * zSegments) + xSegments,
                    x + (z * zSegments) + xSegments + 1
                );
            }

            // This is the degenerate end, meaning that it repeats the last index
            // in a row to properly render each row. Two vertices are necessary to
            // maintain the winding order of the grid.
            if (z < zSegments - 2)
                geometry.addIndex(((z + 1) * zSegments) + (xSegments - 1));
        }
        // Finally generate a simple 'array of bytes' that contains
        // everything for our buffer to work with.
        geometry.gen();

        // Create a buffer and set the stride of information
        myBuffer.CreateBufferNormalMapLayout(14,
                            geometry.getSize(),
                            geometry.getIndicesSize(),
                            geometry.getData(),
                            geometry.getIndicesData());
    }
    // Create a flat plane when no texture map is provided.
    else {
        for(int z = 0 ; z < zSegments; ++z){
            for(int x = 0; x < xSegments; ++x){
                // create a plane if no texture was provided
                geometry.addVertex(x - xSegments / 2.0f, 0.0f, z - zSegments / 2.0f);
            }
        }
        // No need to use the geometry make triangle method when
        // there is no texture provided. The normals will be
        // computed in the geometry shader. Adding the indices
        // is enough
        for(int z = 0 ; z < zSegments - 1; ++z){
            if (z > 0)
                geometry.addIndex(z * zSegments);
                
            for(int x = 0; x < xSegments - 1; ++x){
                geometry.addIndex(x + (z * zSegments));
                geometry.addIndex(x + (z * zSegments) + xSegments);
                geometry.addIndex(x + (z * zSegments) + 1);

                geometry.addIndex(x + (z * zSegments) + 1);
                geometry.addIndex(x + (z * zSegments) + xSegments);
                geometry.addIndex(x + (z * zSegments) + xSegments + 1);
            }
            if (z < zSegments - 2)
                geometry.addIndex(((z + 1) * zSegments) + (xSegments - 1));
        }
        // Finally generate a simple 'array of bytes' that contains
        // everything for our buffer to work with.
        geometry.gen();


        // Create a buffer and set the stride of information
        myBuffer.CreateBufferLayout(3,
                    geometry.getSize(),
                    geometry.getIndicesSize(),
                    geometry.getData(),
                    geometry.getIndicesData());
    }

}
