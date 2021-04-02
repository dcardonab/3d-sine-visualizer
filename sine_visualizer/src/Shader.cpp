#include "Shader.h"

// Constructor
Shader::Shader(){
	std::cout << "(Shader.cpp) Constructor Called\n";
}

// Destructor
Shader::~Shader(){
	// Deallocate Program
	glDeleteProgram(shaderID);
}

// Use our shader
void Shader::Bind() const{
	glUseProgram(shaderID);
}


// Turns off our shader
void Shader::Unbind() const{
	glUseProgram(0);
}

void Shader::Log(const char* system, const char* message){
    std::cout << "[" << system << "]" << message << "\n";
}

// Loads a shader and returns a string
std::string Shader::LoadShader(const std::string& fname){
		std::string result;
		// 1.) Get every line of data
		std::string line;
		std::ifstream myFile(fname.c_str());

		if(myFile.is_open()){
			while(getline(myFile,line)){
				result += line + '\n';
				// Uncomment this line to see the
				// shader code get printed out.
                // SDL_Log(line);
			}
		}
		else{
			Log("LoadShader","file not found. Try an absolute file path to see if the file exists");
		}
		// Close file
		myFile.close();
		return result;
}

void Shader::CreateShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
    std::cout << "Creating shader with a vertex and fragment shader elements.\n";

    // Create a new program
    unsigned int program = glCreateProgram();
    // Compile our shaders
    unsigned int myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    // Link our program
    // These have been compiled already.
    glAttachShader(program,myVertexShader);
    glAttachShader(program,myFragmentShader);
    // Link our programs that have been 'attached'
    glLinkProgram(program);
    glValidateProgram(program);

    // Once the shaders have been linked in, we can delete them.
    glDetachShader(program,myVertexShader);
    glDetachShader(program,myFragmentShader);

    glDeleteShader(myVertexShader);
    glDeleteShader(myFragmentShader);

    if(!CheckLinkStatus(program)){
        Log("CreateShader","ERROR, shader did not link! Were there compile errors in the shader?");
    }

    shaderID = program;
}

// Alternate method initializer in case there is a geometry shader included.
void Shader::CreateShader(const std::string& vertexShaderSource, const std::string& geometryShaderSource, const std::string& fragmentShaderSource) {
    std::cout << "Creating shader with a vertex, geometry, and fragment shader elements.\n";
    // Create a new program
    unsigned int program = glCreateProgram();
    // Compile our shaders
    unsigned int myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int myGeometryShader = CompileShader(GL_GEOMETRY_SHADER, geometryShaderSource);
    unsigned int myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    // Link our program
    // These have been compiled already.
    glAttachShader(program, myVertexShader);
    glAttachShader(program, myGeometryShader);
    glAttachShader(program, myFragmentShader);
    std::cout << "Shaders successfully attached.\n";
    
    // Link our programs that have been 'attached'
    glLinkProgram(program);
    std::cout << "Shader program linked.\n";

    glValidateProgram(program);

    std::cout << "Shader program validated.\n";

    // Once the shaders have been linked in, we can delete them.
    glDetachShader(program, myVertexShader);
    glDetachShader(program, myGeometryShader);
    glDetachShader(program, myFragmentShader);

    std::cout << "Shaders successfully detached.\n";

    glDeleteShader(myVertexShader);
    glDeleteShader(myGeometryShader);
    glDeleteShader(myFragmentShader);

    std::cout << "Shaders successfully deleted.\n";

    if(!CheckLinkStatus(program)){
        Log("CreateShader","ERROR, shader did not link! Were there compile errors in the shader?");
    }

    shaderID = program;
}


unsigned int Shader::CompileShader(unsigned int type, const std::string& source){
    // Compile our shaders
    // id is the type of shader (Vertex, fragment, etc.)
    unsigned int id;

    if (type == GL_VERTEX_SHADER) {
        id = glCreateShader(GL_VERTEX_SHADER);
        std::cout << "Compiling vertex shader.\n";
    } else if (type == GL_GEOMETRY_SHADER) {
        id = glCreateShader(GL_GEOMETRY_SHADER);
        std::cout << "Compiling geometry shader.\n";
    } else if (type == GL_FRAGMENT_SHADER) {
        id = glCreateShader(GL_FRAGMENT_SHADER);
        std::cout << "Compiling fragment shader.\n";
    }
    const char* src = source.c_str();
    // The source of our shader
    glShaderSource(id, 1, &src, nullptr);
    // Now compile our shader
    glCompileShader(id);

    // Retrieve the result of our compilation
    int result;
    // This code is returning any compilation errors that may have occurred!
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* errorMessages = new char[length]; // Could also use alloca here.
        glGetShaderInfoLog(id, length, &length, errorMessages);
        if(type == GL_VERTEX_SHADER) {
        Log("CompileShader ERROR", "GL_VERTEX_SHADER compilation failed!");
        Log("CompileShader ERROR", (const char*)errorMessages);
        } else if (type == GL_GEOMETRY_SHADER) {
        Log("CompileShader ERROR","GL_GEOMETRY_SHADER compilation failed!");
        Log("CompileShader ERROR",(const char*)errorMessages);
        } else if (type == GL_FRAGMENT_SHADER) {
        Log("CompileShader ERROR","GL_FRAGMENT_SHADER compilation failed!");
        Log("CompileShader ERROR",(const char*)errorMessages);
        }
        // Reclaim our memory
        delete[] errorMessages;
        // Delete our broken shader
        glDeleteShader(id);
        return 0;
    }
    std::cout << "\tSuccessfully compiled shader.\n";
    return id;
}

// Check to see if linking was successful
bool Shader::CheckLinkStatus(GLuint programID) {
    // Retrieve the result of our compilation
    int result;
    // This code is returning any Linker errors that may have occurred!
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    if(result == GL_FALSE){
        int length;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);
        char* errorMessages = new char[length]; // Could also use alloca here.
        glGetProgramInfoLog(programID, length, &length, errorMessages);
        // Reclaim our memory
        SDL_Log("ERROR in linking process\n");
        SDL_Log("%s\n", errorMessages);
        delete[] errorMessages;
        return false;
    }

    return true;
}


GLuint Shader::getID() const{
    return shaderID;
}


// Set our uniforms for our shader.
void Shader::setUniformMatrix4fv(const GLchar* name, const GLfloat* value){
    // Note that we are now 'looking' inside the shader for a particular
    // variable. This means the name has to exactly match!
    GLint location = glGetUniformLocation(shaderID,name);

    // Now update this information through our uniforms.
    // glUniformMatrix4v means a 4x4 matrix of floats
    glUniformMatrix4fv(location, 1, GL_FALSE, value);
}


// Set our uniforms for our shader (Useful for a vec3).
void Shader::setUniform3f(const GLchar* name, float v0, float v1, float v2){
    GLint location = glGetUniformLocation(shaderID,name);
    glUniform3f(location, v0, v1, v2);
}

// Sets 1 int value in our uniform (That is why the suffix is 1i).
void Shader::setUniform1i(const GLchar* name, int value){
    GLint location = glGetUniformLocation(shaderID,name);
    glUniform1i(location, value);
}

// Sets 1 float value in our uniform (That is why the suffix is 1f).
void Shader::setUniform1f(const GLchar* name, float value){
    GLint location = glGetUniformLocation(shaderID,name);
    glUniform1f(location, value);
}
