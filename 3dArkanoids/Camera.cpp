#include "Camera.h"
#include <iostream>
#include <fstream>
Camera::Camera(std::string positionFile)
//    :ISerializable()
{
    LoadFromFile(positionFile);
    updateCameraVectors();
}
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
//    :ISerializable(), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
//    :ISerializable(), Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::SaveToFile(std::string file) const
{
    std::ofstream camFile(file, std::ios::out | std::ios::binary);
    camFile.write(reinterpret_cast<const char*>(this), sizeof(Camera));
}

void Camera::LoadFromFile(std::string file)
{
    std::ifstream camFile(file, std::ios::in | std::ios::binary);
    camFile.read(reinterpret_cast<char*>(this), sizeof(Camera));
}

//
//#define NUM_CAMERA_SERIALIZABLE_PROPERTIES 10
//const std::vector<SerializableProperty>& Camera::GetSerializableProperties()
//{
//    std::vector<SerializableProperty> props(NUM_CAMERA_SERIALIZABLE_PROPERTIES);
//
//    props[0].name = "Position";
//    props[0].type = SerializablePropertyType::Vec3;
//    props[0].data.dataUnion.Vec3 = Position;
//
//    props[1].name = "Front";
//    props[1].type = SerializablePropertyType::Vec3;
//    props[1].data.dataUnion.Vec3 = Front;
//
//    props[2].name = "Up";
//    props[2].type = SerializablePropertyType::Vec3;
//    props[2].data.dataUnion.Vec3 = Up;
//
//    props[3].name = "Right";
//    props[3].type = SerializablePropertyType::Vec3;
//    props[3].data.dataUnion.Vec3 = Right;
//
//    props[4].name = "WorldUp";
//    props[4].type = SerializablePropertyType::Vec3;
//    props[4].data.dataUnion.Vec3 = WorldUp;
//
//    props[5].name = "Yaw";
//    props[5].type = SerializablePropertyType::Float;
//    props[5].data.dataUnion.Float = Yaw;
//
//    props[6].name = "Pitch";
//    props[6].type = SerializablePropertyType::Float;
//    props[6].data.dataUnion.Float = Pitch;
//
//    props[7].name = "MovementSpeed";
//    props[7].type = SerializablePropertyType::Float;
//    props[7].data.dataUnion.Float = MovementSpeed;
//
//    props[8].name = "MouseSensitivity";
//    props[8].type = SerializablePropertyType::Float;
//    props[8].data.dataUnion.Float = MouseSensitivity;
//
//    props[9].name = "Zoom";
//    props[9].type = SerializablePropertyType::Float;
//    props[9].data.dataUnion.Float = Zoom;
//
//    return props;
//}
//
//bool Camera::SetSerializableProperty(const SerializableProperty& p)
//{
//    return false;
//}
//
//int Camera::GetNumSerializableProperties() const
//{
//    return NUM_CAMERA_SERIALIZABLE_PROPERTIES;
//}
