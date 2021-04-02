/** @file Camera.h
 *  @brief Sets up an OpenGL camera.
 *  
 *  Sets up an OpenGL Camera. The camera is what
 *  sets up our 'view' matrix.
 *
 *  @author Mike
 *  @bug No known bugs.
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

class Camera{
public:
	// Constructor
    Camera();

    void setView(glm::vec3 eyePos, glm::vec3 viewDir, glm::vec3 upVec);
    // Return a 'view' matrix with our
    // camera transformation applied.
    glm::mat4 getWorldToViewmatrix() const;
    // Move the camera around
    void mouseLook(int mouseX, int mouseY);
    void moveForward(float speed);
    void moveBackward(float speed);
    void moveLeft(float speed);
    void moveRight(float speed);
    void moveUp(float speed);
    void moveDown(float speed);

    inline void printEyePosition() {
            std::cout << "Cam pos\tX: " << eyePosition.x << "\tY: " << eyePosition.y << "\tZ: " << eyePosition.z << std::endl;
            return;
    };

    // Returns the 'eye' position which
    // is where the camera is.
    float getEyeXPosition();
    float getEyeYPosition();
    float getEyeZPosition();

    float getViewXDirection();
    float getViewYDirection();
    float getViewZDirection();
private:
    // Track the old mouse position
    glm::vec2 oldMousePosition;
    // Where is our camera positioned
    glm::vec3 eyePosition;
    // What direction is the camera looking
    glm::vec3 viewDirection;
    // Which direction is 'up' in our world
    // Generally this is constant, but if you wanted
    // to 'rock' or 'rattle' the camera you might play
    // with modifying this value.
    glm::vec3 upVector;
};




#endif
