#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)  {
    this->Front = glm::vec3(0.0f, 0.0f, -1.0f);
    this->MovementSpeed = SPEED;
    this->Position = position;
    this->WorldUp = up;
    this->MouseSensitivity = SENSITIVITY;
    this->Zoom = ZOOM;
    this->Yaw = yaw;
    this->Pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)  {
    this->Front = glm::vec3(0.0f, 0.0f, -1.0f);
    this->MovementSpeed = SPEED;
    this->Position = glm::vec3(posX, posY, posZ);
    this->WorldUp = glm::vec3(upX, upY, upZ);
    this->MouseSensitivity = SENSITIVITY;
    this->Zoom = ZOOM;
    this->Yaw = yaw;
    this->Pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    // Calculate the velocity based on movement speed and time since the last frame
    float velocity = MovementSpeed * deltaTime;
    // Move the camera position based on the specified direction
    if (direction == FORWARD)
        Position += Front * velocity;  // Move forward along the camera's front vector
    if (direction == BACKWARD)
        Position -= Front * velocity;  // Move backward along the camera's front vector
    if (direction == LEFT)
        Position -= Right * velocity;  // Move left along the camera's right vector
    if (direction == RIGHT)
        Position += Right * velocity;  // Move right along the camera's right vector
}


void Camera::MouseButton(int button, int action) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        MouseDragging = true;
    }
    else{
        MouseDragging = false;
    }
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch){
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
    Zoom -= static_cast<float>(yoffset);
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}
