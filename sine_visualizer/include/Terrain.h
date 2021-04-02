/** @file Terrain.h
 *  @brief Create a terrain
 *  
 *  More...
 *
 *  @author Mike
 *  @bug No known bugs.
 */
#ifndef TERRAIN_H
#define TERRAIN_H

#include "Object.h"
#include "Texture.h"
#include "Shader.h"
#include "Image.h"
#include <vector>
#include <string>

class Terrain : public Object {
public:
    // Default constructor
    Terrain(int xSegs, int zSegs);
    // Alternate constructor
    Terrain(int xSegs, int zSegs, std::string fileName);
    ~Terrain();
    // override the initilization routine.
    void init();
    inline int getXSegments() { return xSegments; }
    inline int getZSegments() { return zSegments; }

private:
    // data
    int xSegments;
    int zSegments;

    // Store the height in a multidimensional array
    int* heightData;

    // Textures for the terrain
    // Terrains are often 'multitextured' and have multiple textures.
    std::vector<Texture> textures;

    std::string m_texturePath;
    std::string m_terrainPath;

};

#endif
