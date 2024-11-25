#pragma once

#include "Common.h"

namespace FL
{

class Camera3D {
public:
    Camera3D(glm::vec3 position);

    glm::vec3 GetPosition() const;
    glm::mat4 GetViewMatrix() const;

    void ProcessKeyboard(float deltaTime);
    void ProcessMouse();

private:
    void UpdateCameraVectors();

private:
    glm::vec3 m_position {};
    glm::vec3 m_front {};
    glm::vec3 m_up {};
    glm::vec3 m_worldUp { 0.0f, 1.0f, 0.0f };
    glm::vec3 m_right {};
    glm::vec2 m_lastMousePos {};

    f32 m_yaw {};
    f32 m_pitch {};

    f32 m_movementSpeed {};
    f32 m_mouseSensitivity {};
};

}