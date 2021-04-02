/** @file SDLGraphicsProgram.h
 *  @brief SDL Class used to setup input and setup of OpenGL.
 *  
 *  This class is used for the initialization of SDL.
 *
 *  @author Mike
 *  @bug No known bugs.
 */

#ifndef SDLGRAPHICSPROGRAM
#define SDLGRAPHICSPROGRAM

// ==================== Libraries ==================
// Depending on the operating system we use
// The paths to SDL are actually different.
// The #define statement should be passed in
// when compiling using the -D argument.
// This gives an example of how a programmer
// may support multiple platforms with different
// dependencies.
#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
#else // This works for Mac
    #include <SDL.h>
#endif

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "Renderer.h"
#include "Terrain.h"

// Purpose:
// This class sets up a full graphics program using SDL
class SDLGraphicsProgram{
public:

    // Constructors
    SDLGraphicsProgram(int w, int h);   // Default constructor
    SDLGraphicsProgram(int w, int h, std::string terrain_filepath);
    SDLGraphicsProgram(int w, int h, std::string terrain_filepath, std::string texture_filepath);
    // Desctructor
    ~SDLGraphicsProgram();

    void init();
    // loop that runs forever
    void loop();
    // Get Pointer to Window
    SDL_Window* getSDLWindow();
    // Helper Function to Query OpenGL information.
    void getOpenGLVersionInfo();

private:
    // The window we'll be rendering to
    SDL_Window* gWindow ;
    // OpenGL context
    SDL_GLContext gContext;
	// The Renderer responsible for drawing objects
	// in OpenGL (Or whatever Renderer you choose!)
	Renderer* renderer;
    // String to store the currently loaded FBO fragment
    // shader when manually selecting it.
    std::string fboFragShader;

    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;

    std::string m_terrainPath;
    std::string m_texturePath;
};

#endif
