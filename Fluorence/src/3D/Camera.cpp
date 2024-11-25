#include "3D/Camera.h"
#include "Fluorence.h"

namespace FL
{

Camera3D::Camera3D(glm::vec3 position)
    : m_position(position)
{
    m_yaw = -90.0f;
    m_pitch = 0.0f;

    m_movementSpeed = 2.5f;
    m_mouseSensitivity = 0.1f;

    m_lastMousePos = GetWindowDimension();
    m_lastMousePos /= 2.0;

    UpdateCameraVectors();
}

glm::vec3 Camera3D::GetPosition() const
{
    return m_position;
}

glm::mat4 Camera3D::GetViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera3D::ProcessKeyboard(f32 deltaTime)
{
    f32 velocity = 2.5f * deltaTime;
    glm::vec3 direction(0.0f);

    if (IsKeyPressed(GLFW_KEY_W)) {
        direction += m_front;
    }

    if (IsKeyPressed(GLFW_KEY_S)) {
        direction -= m_front;
    }

    if (IsKeyPressed(GLFW_KEY_A)) {
        direction -= m_right;
    }

    if (IsKeyPressed(GLFW_KEY_D)) {
        direction += m_right;
    }

    if (glm::length(direction) > 0.0f) {
        direction = glm::normalize(direction);
    }

    m_position += direction * velocity;
}

void Camera3D::ProcessMouse()
{
    auto mousePosition = GetMousePosition();
    auto offset = glm::vec2(mousePosition.x - m_lastMousePos.x,
                            m_lastMousePos.y - mousePosition.y);

    m_lastMousePos = mousePosition;

    offset *= m_mouseSensitivity;

    m_yaw += offset.x;
    m_pitch += offset.y;

    if (m_pitch > 89.0f) {
        m_pitch = 89.0f;
    }

    if (m_pitch < -89.0f) {
        m_pitch = -89.0f;
    }

    UpdateCameraVectors();
}

void Camera3D::UpdateCameraVectors()
{
    glm::vec3 front;
    front.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
    front.y = glm::sin(glm::radians(m_pitch));
    front.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));

    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

}