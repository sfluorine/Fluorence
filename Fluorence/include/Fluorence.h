#pragma once

#include "Common.h"

#include "3D/Camera.h"
#include "3D/Model.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"

namespace FL
{

bool Init(const char* appName, i32 windowWidth, i32 windowHeight);
void Shutdown();

bool WindowShouldClose();
void WindowPollEvents();
void WindowSwapBuffers();
void WindowClear(const glm::vec3& color);

void SetCursor(i32 value);

bool IsKeyPressed(i32 key);

glm::vec2 GetWindowDimension();
glm::vec2 GetMousePosition();
glm::mat4 GetProjectionMatrix();

} // namespace FL