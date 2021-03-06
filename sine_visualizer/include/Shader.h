/** @file Shader.h
 *  @brief Manages the loading, compiling, and linking of vertex and fragment shaders.
 *  
 *  Additionally has functions for setting various uniforms.
 *
 *  @author Mike
 *  @bug No known bugs.
 */

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <iostream>
#include <fstream>
#include <glad/glad.h>

#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
#else
    #include <SDL.h>
#endif

class Shader{
public:
    // Shader constructor
    Shader();
    // Shader Destructor
    ~Shader();
    // Use this shader in our pipeline.
    void Bind() const;
    // Remove shader from our pipeline
    void Unbind() const;
    // Load a shader
    std::string LoadShader(const std::string& fname);
    // Create a Shader from a loaded vertex and fragment shaders,
    // or from loaded vertex, geometry, and fragment shaders.
    void CreateShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    void CreateShader(const std::string& vertexShaderSource, const std::string& geometryShaderSource, const std::string& fragmentShaderSource);
    // return the shader id
    GLuint getID() const;
    // Set our uniforms for our shader.
    void setUniformMatrix4fv(const GLchar* name, const GLfloat* value);
	void setUniform3f(const GLchar* name, float v0, float v1, float v2);
    void setUniform1i(const GLchar* name, int value);
    void setUniform1f(const GLchar* name, float value);

private:
    // Compiles loaded shaders
    unsigned int CompileShader(unsigned int type, const std::string& source);
    // Makes sure shaders 'linked' successfully
    bool CheckLinkStatus(GLuint programID);
    // Shader loading utility programs
    void printProgramLog( GLuint program );
    void printShaderLog( GLuint shader );
    // Logs an error message 
    void Log(const char* system, const char* message);
    // The unique shaderID
    GLuint shaderID;
};

#endif

