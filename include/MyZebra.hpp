/**
 * MyZebra.hpp skeleton
 * Contributors:
 *      * CS
 * Licence:
 *      * MIT
 */

#pragma once

#include "Application.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>

#include "asset.hpp"
#include "glError.hpp"

class MyZebra : public Application {
public:
    MyZebra();
    ~MyZebra() = default;

protected:
    void loop() override;
    void processInput(GLFWwindow *window);

private:
    unsigned int VAO{0};
    std::unique_ptr<Shader> shaderProgram = std::make_unique<Shader>(SHADER_DIR "/Zebra/Zebra.vert", SHADER_DIR "/Zebra/Zebra.frag");
    std::unique_ptr<Texture> texture = std::make_unique<Texture>();
    glm::vec2 lowHigtThreads{0.3f, 0.6f};
    void render();
};
