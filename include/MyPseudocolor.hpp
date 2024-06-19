/**
 * MyPseudocolor.hpp skeleton
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

class MyPseudocolor : public Application {
public:
    MyPseudocolor();
    ~MyPseudocolor() = default;

protected:
    void loop() override;
    void processInput(GLFWwindow *window);

private:
    unsigned int VAO{0};
    std::unique_ptr<Shader> shaderProgram = std::make_unique<Shader>(SHADER_DIR "/Pseudocolor/Pseudocolor.vert", SHADER_DIR "/Pseudocolor/Pseudocolor.frag");
    std::unique_ptr<Texture> texture = std::make_unique<Texture>(RES_DIR "/picture/Lakewater_trees.jpg");
    std::unique_ptr<Texture> texture_lut = std::make_unique<Texture>(RES_DIR "/picture/Pseudocolor3DLUT.png");
    void render();
};

