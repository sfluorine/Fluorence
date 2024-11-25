#include "Fluorence.h"

struct Application {
    GLFWwindow* window { nullptr };
    const char* appName { nullptr };

    i32 windowWidth {};
    i32 windowHeight {};

    glm::mat4 projection;
};

static Application s_application {};

namespace FL
{

bool Init(const char* appName, i32 windowWidth, i32 windowHeight)
{
    if (!glfwInit()) {
        spdlog::critical("failed to initialize glfw!");
        return false;
    }

    s_application.appName = appName;
    s_application.windowWidth = windowWidth;
    s_application.windowHeight = windowHeight;

    auto aspectRatio
        = static_cast<f32>(windowWidth) / static_cast<f32>(windowHeight);

    s_application.projection
        = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    s_application.window = glfwCreateWindow(
        windowWidth, windowHeight, appName, nullptr, nullptr);

    if (!s_application.window) {
        glfwTerminate();

        spdlog::critical("failed to create glfw window!");
        return false;
    }

    glfwMakeContextCurrent(s_application.window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        glfwDestroyWindow(s_application.window);
        glfwTerminate();

        spdlog::critical("failed to initialize glad!");
        return false;
    }

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(
        [](GLenum source,
           GLenum type,
           GLuint id,
           GLenum severity,
           GLsizei length,
           const GLchar* message,
           const void* userParam) { spdlog::info("OpenGL: {}", message); },
        nullptr);

    glfwSetFramebufferSizeCallback(
        s_application.window, [](GLFWwindow* window, i32 width, i32 height) {
            s_application.windowWidth = width;
            s_application.windowHeight = height;

            auto aspectRatio
                = static_cast<f32>(width) / static_cast<f32>(height);

            s_application.projection = glm::perspective(
                glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

            glViewport(0, 0, width, height);
        });

    spdlog::info("GPU: {} {}",
                 reinterpret_cast<const char*>(glGetString(GL_VENDOR)),
                 reinterpret_cast<const char*>(glGetString(GL_RENDERER)));

    spdlog::info("OpenGL Version: {}",
                 reinterpret_cast<const char*>(glGetString(GL_VERSION)));

    spdlog::info("Shading Language Version: {}",
                 reinterpret_cast<const char*>(
                     glGetString(GL_SHADING_LANGUAGE_VERSION)));

    return true;
}

void Shutdown()
{
    if (s_application.window) {
        glfwDestroyWindow(s_application.window);
        glfwTerminate();
    }
}

bool WindowShouldClose()
{
    return glfwWindowShouldClose(s_application.window);
}

void WindowPollEvents()
{
    glfwPollEvents();
}

void WindowSwapBuffers()
{
    glfwSwapBuffers(s_application.window);
}

void WindowClear(const glm::vec3& color)
{
    glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SetCursor(i32 value)
{
    glfwSetInputMode(s_application.window, GLFW_CURSOR, value);
}

bool IsKeyPressed(i32 key)
{
    return glfwGetKey(s_application.window, key) == GLFW_PRESS;
}

glm::vec2 GetWindowDimension()
{
    return glm::vec2(s_application.windowWidth, s_application.windowHeight);
}

glm::vec2 GetMousePosition()
{
    double x, y;
    glfwGetCursorPos(s_application.window, &x, &y);

    return glm::vec2(static_cast<f32>(x), static_cast<f32>(y));
}

glm::mat4 GetProjectionMatrix()
{
    return s_application.projection;
}

} // namespace FL