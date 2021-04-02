#include "Camera.h"

// Camera constructor
Camera::Camera(){
    std::cout << "(Camera.cpp) Constructor Called\n";
    return;
}

void Camera::setView(glm::vec3 eyePos, glm::vec3 viewDir, glm::vec3 upVec) {
    eyePosition = eyePos;
    viewDirection = viewDir;
    upVector = upVec;
    std::cout << "New Eye Position:\n\tX: " << eyePos.x << "\tY: " << eyePos.y << "\tZ: " << eyePos.z << "\n";
    std::cout << "New View Direction:\n\tX: " << viewDir.x << "\tY: " << viewDir.y << "\tZ: " << viewDir.z << "\n";
    std::cout << "New Up Vector:\n\tX: " << upVec.x << "\tY: " << upVec.y << "\tZ: " << upVec.z << "\n";
    return;
}

void Camera::mouseLook(int mouseX, int mouseY){
    // Record our new position as a vector
    glm::vec2 newMousePosition(mouseX, mouseY);
    // Detect how much the mouse has moved since
    // the last time
    glm::vec2 mouseDelta = 0.005f * (newMousePosition-oldMousePosition);
    viewDirection = glm::mat3(glm::rotate(-mouseDelta.x, upVector)) * viewDirection;
    
    // Update our old position after we have made changes 
    oldMousePosition = newMousePosition;
}

void Camera::moveForward(float speed){
    eyePosition += viewDirection * speed;
    printEyePosition();
}

void Camera::moveBackward(float speed){
    eyePosition -= viewDirection * speed;
    printEyePosition();
}

void Camera::moveLeft(float speed){
    eyePosition -=  glm::normalize(glm::cross(viewDirection, upVector)) * speed;
    printEyePosition();
}

void Camera::moveRight(float speed){
    eyePosition += glm::normalize(glm::cross(viewDirection, upVector)) * speed;
    printEyePosition();
}

void Camera::moveUp(float speed){
    eyePosition += upVector * speed;
    printEyePosition();
}

void Camera::moveDown(float speed){
    eyePosition -= upVector * speed;
    printEyePosition();
}

float Camera::getEyeXPosition(){
    return eyePosition.x;
}

float Camera::getEyeYPosition(){
    return eyePosition.y;
}

float Camera::getEyeZPosition(){
    return eyePosition.z;
}

float Camera::getViewXDirection(){
    return viewDirection.x;
}

float Camera::getViewYDirection(){
    return viewDirection.y;
}

float Camera::getViewZDirection(){
    return viewDirection.z;
}

// Get our view matrix from this camera
glm::mat4 Camera::getWorldToViewmatrix() const{
    // Think about the second argument and why that is
    // setup as it is.
    return glm::lookAt( eyePosition,
                        eyePosition + viewDirection,
                        upVector);
}
