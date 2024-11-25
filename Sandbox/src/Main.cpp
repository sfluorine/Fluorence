#include <Fluorence.h>

int main()
{
    FL::Init("Sandbox", 800, 600);

    FL::Camera3D camera(glm::vec3(0.0f, 0.0f, 6.0f));

    auto shader = FL::Shader::LoadFromFile("res/shaders/vertex.glsl",
                                           "res/shaders/fragment.glsl");

    auto texture = FL::Texture::LoadFromFile("res/textures/skin_man.png");

    auto model = FL::Model::LoadFromFile("res/models/basicCharacter.obj");

    shader->Bind();
    shader->SetUniform1i("u_sampler", 0);
    shader->SetUniformMat4("u_projection",
                           glm::value_ptr(FL::GetProjectionMatrix()));
    shader->SetUniformMat4(
        "u_model",
        glm::value_ptr(glm::scale(glm::mat4(1.0f), glm::vec3(0.05f))));

    f32 lastTime = 0;

    while (!FL::WindowShouldClose()) {
        f32 currentTime = static_cast<f32>(glfwGetTime());
        f32 deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        FL::WindowPollEvents();

        camera.ProcessMouse();
        camera.ProcessKeyboard(deltaTime);

        FL::WindowClear(glm::vec3(0.0f));

        texture->Bind(0);
        shader->Bind();
        shader->SetUniformMat4("u_view",
                               glm::value_ptr(camera.GetViewMatrix()));

        model->Draw(shader);

        FL::WindowSwapBuffers();
    }

    FL::Shutdown();
}