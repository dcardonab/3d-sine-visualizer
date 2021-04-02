#include "SDLGraphicsProgram.h"

// 
SDLGraphicsProgram::SDLGraphicsProgram(int w, int h) : WINDOW_WIDTH(w), WINDOW_HEIGHT(h) {
    init();
}


SDLGraphicsProgram::SDLGraphicsProgram(int w, int h, std::string texture_filepath) : WINDOW_WIDTH(w), WINDOW_HEIGHT(h), m_texturePath(texture_filepath){
    init();
}

SDLGraphicsProgram::SDLGraphicsProgram(int w, int h, std::string texture_filepath, std::string terrain_filepath) : WINDOW_WIDTH(w), WINDOW_HEIGHT(h), m_texturePath(texture_filepath), m_terrainPath(terrain_filepath){
    init();
}


// Proper shutdown of SDL and destroy initialized objects
SDLGraphicsProgram::~SDLGraphicsProgram(){
    if(renderer!=nullptr){
        delete renderer;
    }

    //Destroy window
    SDL_DestroyWindow( gWindow );
    // Point gWindow to NULL to ensure it points to nothing.
    gWindow = NULL;
    //Quit SDL subsystems
    SDL_Quit();
}

void SDLGraphicsProgram::init() {
    std::cout << "(SDLGraphicsProgram.cpp) Constructor Called\n";
    // Initialization flag
    bool success = true;
    // String to hold any errors that occur.
    std::stringstream errorStream;
    // The window we'll be rendering to
    gWindow = NULL;
    // Render flag

    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO)< 0){
        errorStream << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
        success = false;
    }
    else{
    //Use OpenGL 3.3 core
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    // We want to request a double buffer for smooth updating.
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    //Create window
    gWindow = SDL_CreateWindow( "3D Graphics Final Project - David Cardona",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                WINDOW_WIDTH,
                                WINDOW_HEIGHT,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

        // Check if Window did not create.
        if( gWindow == NULL ){
            errorStream << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
            success = false;
        }

        //Create an OpenGL Graphics Context
        gContext = SDL_GL_CreateContext( gWindow );
        if( gContext == NULL){
            errorStream << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
            success = false;
        }

        // Initialize GLAD Library
        if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
            errorStream << "Failed to iniitalize GLAD and OpenGL\n";
            success = false;
        }
    }

    // If initialization did not work, then print out a list of errors in the constructor.
    if(!success){
        errorStream << "SDLGraphicsProgram::SDLGraphicsProgram - Failed to initialize!\n";
        std::string errors=errorStream.str();
        SDL_Log("%s\n",errors.c_str());
    }else{
        SDL_Log("SDLGraphicsProgram::SDLGraphicsProgram - No SDL, GLAD, or OpenGL, errors detected during initialization\n\n");
    }

    // SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN); // Uncomment to enable extra debug support!
    getOpenGLVersionInfo();


    // Setup our Renderer
    renderer = new Renderer(WINDOW_WIDTH, WINDOW_HEIGHT);
}

//Loops forever!
void SDLGraphicsProgram::loop() {
    // Get terrain data and build terrain
    int terrainX;
    int terrainZ;

    std::cout << "Please enter the X and Z dimensions for your plane:\n\tX: ";
    std::cin >> terrainX;
    std::cout << "\tZ: ";
    std::cin >> terrainZ;
    std::cout << "\n\n";

    // Create terrain and assign texture if there is one
    // The parser currently requires the X coordinate to
    // be larger than the Z coordinate to properly add
    // indices onto the geometry class.
    Terrain* myTerrain = new Terrain(
        (terrainX > terrainZ) ? terrainX : terrainZ, (terrainZ < terrainX) ? terrainZ : terrainX
    );

    // Logic for adding a texture has not yet been fully implemented,
    // as it requires implementing a different vertex shader (more layouts).
    // It's a way to further expand the project.
    if (!m_texturePath.empty())
        myTerrain->LoadTexture(m_texturePath);
    // Create a node for the terrain 
    SceneNode* terrainNode;
    // The dimensions are also passed in to create light
    // offsets. This should be further abstracted in the
    // future, possibly by creating a light's class.
    terrainNode = new SceneNode(
        myTerrain,
        (terrainX > terrainZ) ? terrainX : terrainZ, (terrainZ < terrainX) ? terrainZ : terrainX
    );
    // Set up as root of SceneTree
    renderer->setRoot(terrainNode);

    float amplitude = 100.0f;
    float amplitudeSpeed = 1.0f;
    terrainNode->setAmplitude(amplitude);

    float waveNumber = 0.5f;
    float waveNumberSpeed = 0.1;
    terrainNode->setWaveNumber(waveNumber);

    float wavePeriod = 50.0f;
    float wavePeriodSpeed = 5.0f;
    terrainNode->setWavePeriod(wavePeriod);

    // Flag to keep track of whether wireframe mode is enabled
    bool geometryWireframe = false;
    bool framebufferWireframe = false;
    // Set initial wireframe modes
    renderer->setGeometryWireframe(geometryWireframe);
    renderer->setFramebufferWireframe(framebufferWireframe);

    // Flag to enable/disable the mouse movement of the camera
    bool camRotationEnabled = false;
    // Set a default speed for the camera
    float cameraSpeed = 5.0f;

    // Automatically place height of the camera
    float cameraHeight = (terrainX > terrainZ) ? terrainX * 1.5f : terrainZ * 1.5f;

    renderer->camera->setView(
        // Eye position
        glm::vec3(0.0f, cameraHeight, 0.0f),
        // View direction
        glm::vec3(0.0f,  -1.0f, 0.0f),
        // Up vector
        glm::vec3(0.0f,   0.0f, -1.0f));

    myTerrain->setRenderMode("triStrip");
    std::cout << "Rendering in GL_TRIANGLE_STRIP mode.\n";
    terrainNode->setPlaneMode("yAxis");

    // Event handler that handles various events in SDL
    // that are related to input and output
    SDL_Event e;
    // Enable text 
    SDL_StartTextInput();

    // Main loop flag
    // If this is quit = 'true' then the program terminates.
    bool quit = false;

    // While application is running
    while(!quit){
        terrainNode->getLocalTransform().loadIdentity();

        //Handle events on queue
        while(SDL_PollEvent(&e) != 0){
            // User posts an event to quit
            // An example is hitting the "x" in the corner of the window.
            if(e.type == SDL_QUIT){
                quit = true;
            }
            // Handle keyboad input for the camera class
            if(e.type==SDL_MOUSEMOTION && camRotationEnabled){
                // Handle mouse movements
                renderer->camera->mouseLook(e.motion.x, e.motion.y);
            }
            switch(e.type){
                // Handle keyboard presses
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym){
//===================== INTERFACE CONTROLS
                        case SDLK_ESCAPE:
                        case SDLK_q:
                            quit = true;
                            break;

//===================== CAMERA CONTROLS
                        // setView() arguments are eye position,
                        // view direction, and up vector.

                        // Top view
                        case SDLK_a:
                            renderer->camera->setView(
                                glm::vec3(0.0f, cameraHeight, 0.0f),
                                glm::vec3(0.0f, -1.0f, 0.0f),
                                glm::vec3(0.0f, 0.0f, -1.0f));
                            break;

                        // Bottom view
                        case SDLK_s:
                            renderer->camera->setView(
                                glm::vec3(0.0f, -cameraHeight, 0.0f),
                                glm::vec3(0.0f, 1.0f, 0.0f),
                                glm::vec3(0.0f, 0.0f, -1.0f));
                            break;

                        // Left side
                        case SDLK_d:
                            renderer->camera->setView(
                                glm::vec3(-cameraHeight, 0.0f, 0.0f),
                                glm::vec3(1.0f, 0.0f, 0.0f),
                                glm::vec3(0.0f, 1.0f, 0.0f));
                            break;

                        // Right side
                        case SDLK_f:
                            renderer->camera->setView(
                                glm::vec3(cameraHeight, 0.0f, 0.0f),
                                glm::vec3(-1.0f, 0.0f, 0.0f),
                                glm::vec3(0.0f, 1.0f, 0.0f));
                            break;

                        // Front side
                        case SDLK_g:
                            renderer->camera->setView(
                                glm::vec3(0.0f, 0.0f, cameraHeight),
                                glm::vec3(0.0f, 0.0f, -1.0f),
                                glm::vec3(0.0f, 1.0f,  0.0f));
                            break;

                        // Back side
                        case SDLK_h:
                            renderer->camera->setView(
                                glm::vec3(0.0f, 0.0f, -cameraHeight),
                                glm::vec3(0.0f, 0.0f, 1.0f),
                                glm::vec3(0.0f, 1.0f, 0.0f));
                            break;

                        // Use arrows to move the camera
                        case SDLK_LEFT:
                            renderer->camera->moveLeft(cameraSpeed);
                            break;
                        case SDLK_RIGHT:
                            renderer->camera->moveRight(cameraSpeed);
                            break;
                        case SDLK_UP:
                            renderer->camera->moveForward(cameraSpeed);
                            break;
                        case SDLK_DOWN:
                            renderer->camera->moveBackward(cameraSpeed);
                            break;
                        case SDLK_RSHIFT:
                            renderer->camera->moveUp(cameraSpeed);
                            break;
                        case SDLK_RCTRL:
                        case SDLK_RALT:
                            renderer->camera->moveDown(cameraSpeed);
                            break;

                        // Use 'r' to allow the mouse to move the camera sideways
                        case SDLK_r:
                            camRotationEnabled = !camRotationEnabled;
                            break;

//===================== WAVE CONTROLS
                        // [ decreases the amplitude
                        case SDLK_LEFTBRACKET:
                            amplitude -= amplitudeSpeed;
                            std::cout << "Amplitude: " << amplitude << '\n';
                            terrainNode->setAmplitude(amplitude);
                            break;
                        // ] increases the amplitude
                        case SDLK_RIGHTBRACKET:
                            amplitude += amplitudeSpeed;
                            std::cout << "Amplitude: " << amplitude << '\n';
                            terrainNode->setAmplitude(amplitude);
                            break;
                        // ; decreases the wavenumber
                        case SDLK_SEMICOLON:
                            waveNumber -= waveNumberSpeed;
                            std::cout << "Wave Number: " << waveNumber << '\n';
                            terrainNode->setWaveNumber(waveNumber);
                            break;
                        // ' increases the wavenumber
                        case SDLK_QUOTE:
                            waveNumber += waveNumberSpeed;
                            std::cout << "Wave Number: " << waveNumber << '\n';
                            terrainNode->setWaveNumber(waveNumber);
                            break;
                        // . decreases the wave period
                        case SDLK_PERIOD:
                            wavePeriod -= wavePeriodSpeed;
                            std::cout << "Wave Period: " << wavePeriod << '\n';
                            terrainNode->setWavePeriod(wavePeriod);
                            break;
                        // / increases the wave period
                        case SDLK_SLASH:
                            wavePeriod += wavePeriodSpeed;
                            std::cout << "Wave Period: " << wavePeriod << '\n';
                            terrainNode->setWavePeriod(wavePeriod);
                            break;

//===================== PLANE MODES
                        case SDLK_z:
                            terrainNode->setPlaneMode("yAxis");
                            break;
                        case SDLK_x:
                            terrainNode->setPlaneMode("xyAxis");
                            break;
                        case SDLK_c:
                            terrainNode->setPlaneMode("flat");
                            break;

//===================== RENDER MODES
                        // Use the w key to toggle wireframe mode
                        case SDLK_w:
                            geometryWireframe = !geometryWireframe;
                            renderer->setGeometryWireframe(geometryWireframe);
                            if (geometryWireframe)
                                std::cout << "\nWireframe mode ENABLED.\n";
                            else
                                std::cout << "\nWireframe mode DISABLED.\n\n";
                            break;

                        case SDLK_e:
                            framebufferWireframe = !framebufferWireframe;
                            renderer->setFramebufferWireframe(framebufferWireframe);
                            if (framebufferWireframe)
                                std::cout << "\nFramebuffer wireframe mode ENABLED.\n";
                            else
                                std::cout << "\nFramebuffer wireframe mode DISABLED.\n\n";
                            break;
                        
                        case SDLK_o:
                            myTerrain->setRenderMode("triStrip");
                            std::cout << "Rendering in GL_TRIANGLE_STRIP mode.\n";
                            break;

                        case SDLK_p:
                            myTerrain->setRenderMode("points");
                            std::cout << "Rendering in GL_POINTS mode.\n";
                            break;

//===================== POST-PROCESSING EFFECTS
                        case SDLK_1:
                            fboFragShader = "./shaders/fboFrag_standard.glsl";
                            renderer->setFBOShader(fboFragShader);
                            break;
                        case SDLK_2:
                            fboFragShader = "./shaders/fboFrag_grayscale.glsl";
                            renderer->setFBOShader(fboFragShader);
                            break;
                        case SDLK_3:
                            fboFragShader = "./shaders/fboFrag_blur.glsl";
                            renderer->setFBOShader(fboFragShader);
                            break;
                        case SDLK_4:
                            fboFragShader = "./shaders/fboFrag_invertColors.glsl";
                            renderer->setFBOShader(fboFragShader);
                            break;
                        case SDLK_5:
                            fboFragShader = "./shaders/fboFrag_sharpen.glsl";
                            renderer->setFBOShader(fboFragShader);
                            break;
                        case SDLK_6:
                            fboFragShader = "./shaders/fboFrag_edgeDetection.glsl";
                            renderer->setFBOShader(fboFragShader);
                            break;

                        default:
                            break;
                    }
                break;
            }
        } // End SDL_PollEvent loop.

        // Update our scene through our renderer
        renderer->Update();
        // Render our scene using our selected renderer
        renderer->Render();
        // Delay to slow things down just a bit!
        SDL_Delay(25); 
        //Update screen of our specified window
        SDL_GL_SwapWindow(getSDLWindow());
    }

    //Disable text input
    SDL_StopTextInput();
}


// Get Pointer to Window
SDL_Window* SDLGraphicsProgram::getSDLWindow(){
  return gWindow;
}

// Helper Function to get OpenGL Version Information
void SDLGraphicsProgram::getOpenGLVersionInfo(){
    SDL_Log("(Note: If you have two GPU's, make sure the correct one is selected)");
    SDL_Log("Vendor: %s",(const char*)glGetString(GL_VENDOR));
    SDL_Log("Renderer: %s",(const char*)glGetString(GL_RENDERER));
    SDL_Log("Version: %s",(const char*)glGetString(GL_VERSION));
    SDL_Log("Shading language: %s",(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}
