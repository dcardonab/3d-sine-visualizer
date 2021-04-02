#include "Renderer.h"

// Sets the height and width of our renderer
Renderer::Renderer(unsigned int w, unsigned int h) : m_screenWidth(w), m_screenHeight(h){
    std::cout << "(Renderer.cpp) Constructor Called\n";

    camera = new Camera();
    root = nullptr;

    // Setup our Framebuffer within the renderer
    myFramebuffer = new Framebuffer();
    myFramebuffer->Create(w,h);
}

// Sets the height and width of our renderer
Renderer::~Renderer(){
    delete camera;
    delete myFramebuffer;
}

void Renderer::Update(){
    // Here we apply the projection matrix which creates perspective.
    // The first argument is 'field of view'
    // Then perspective
    // Then the near and far clipping plane.
    // Note I cannot see anything closer than 0.1f units from the screen.
    projectionMatrix = glm::perspective(45.0f,((float)m_screenWidth)/((float)m_screenHeight),0.1f, 1000.0f);

    // Perform the update
    if(root!=nullptr){
        // TODO: See if I can pass these by reference
        root->Update(projectionMatrix, camera);
    }
}

// Initialize clear color
// Setup our OpenGL State machine
// Then render the scene
void Renderer::Render(){
    // Setup our uniforms
    // In reality, only need to do this once for this
    // particular fbo because the texture data is 
    // not going to change.
    myFramebuffer->Update();
    // Bind to our farmebuffer
    myFramebuffer->Bind();

    // What we are doing, is telling opengl to create a depth(or Z-buffer) 
    // for us that is stored every frame.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D); 
    // This is the background of the screen.
    glViewport(0, 0, m_screenWidth, m_screenHeight);
    glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
    // Clear color buffer and Depth Buffer
    // Remember that the 'depth buffer' is our
    // z-buffer that figures out how far away items are every frame
    // and we have to do this every frame!
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // Use glPolygonMode() to enable and disable wireframe mode.
    // GL_LINE will be used when wireframe is true, otherwise,
    // GL_FILL will be used. This value is toggled with the
    // 'w' key.
    glPolygonMode(GL_FRONT_AND_BACK, (m_geometryWireframe) ? GL_LINE : GL_FILL);
    
    // Now we render our objects from our scenegraph
    if(root!=nullptr){
        root->Draw();
    }

    // Finish with our framebuffer
    myFramebuffer->Unbind();

    // The framebuffer wireframe mode can be toggled using the
    // 'e' key.
    glPolygonMode(GL_FRONT_AND_BACK, (m_framebufferWireframe) ? GL_LINE : GL_FILL);

    // Now draw a new scene
    // We do not need depth since we are drawing a '2D'
    // image over our screen.
    glDisable(GL_DEPTH_TEST);
    // Clear everything away
    // Clear the screen color, and typically I do this
    // to something 'different' than our original as an
    // indication that I am in a FBO. But you may choose
    // to match the glClearColor
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    // We only have 'color' in our buffer that is stored
    glClear(GL_COLOR_BUFFER_BIT); 
    // Use our new 'simple screen shader'
    myFramebuffer->fboShader->Bind();
    // Overlay our 'quad' over the screen
    myFramebuffer->drawFBO();    
    // Unselect our shader and continue
    myFramebuffer->fboShader->Unbind();
}

// Determines what the root is of the renderer, so the
// scene can be drawn.
void Renderer::setRoot(SceneNode* n){
    root = n;
}

void Renderer::setFBOShader(std::string fboFragShader) {
    // Delete and create a new framebuffer
    delete myFramebuffer;
    // Pass in fboFragShader to apply it to the new shader
    myFramebuffer = new Framebuffer(fboFragShader);
    myFramebuffer->Create(m_screenWidth, m_screenHeight);
}
