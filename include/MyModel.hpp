/**
 * MyModel.hpp skeleton
 * Contributors:
 *      * CS
 * Licence:
 *      * MIT
 */

#pragma once

 #include "Application.hpp"
 #include "Model.hpp"
 #include "Shader.hpp"
 #include "Texture.hpp"
 #include "camera.hpp"
 
#define GLM_ENABLE_EXPERIMENTAL
 #include <GLFW/glfw3.h>

 #include <glm/glm.hpp>
 #include <glm/gtc/matrix_transform.hpp>
 #include <glm/gtx/matrix_operation.hpp>
 
 #include "asset.hpp"
 #include "glError.hpp"

typedef struct DirLight
{
    glm::vec3 direction;
    float ambient;
    float diffuse;
    float specular;
} DirLight;

typedef struct PointLight
{
    glm::vec3 position;
    float ambient;
    float diffuse;
    float specular;
    float constant;
    float linear;
    float quadratic;
} PointLight;

class MyModel : public Application
{
public:
    MyModel();
    ~MyModel();

protected:
    std::string title = "3DModel";
    void loop() override;
    void anotherImGui() override;
    static void mouse_callback(GLFWwindow *window, double xpos, double ypos) noexcept;
    static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) noexcept;
    static void mymouse_callback_null(GLFWwindow *window, double xpos, double ypos) noexcept;
    static void myscroll_callback_null(GLFWwindow *window, double xoffset, double yoffset) noexcept;
    void processInput(GLFWwindow *window);

private:
    DirLight dirLight;
    PointLight pointLight;
    glm::vec3 dirLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 pointLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    std::unique_ptr<Shader> shaderProgram = std::make_unique<Shader>(SHADER_DIR "/3dmodel/3dmodel.vert", SHADER_DIR "/3dmodel/3dmodel.frag");
    std::unique_ptr<Model> ourModel = std::make_unique<Model>(RES_DIR "/models/nanosuit/nanosuit.obj");
    void render();
};
