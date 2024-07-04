/**
 * MyGaussianblur.hpp skeleton
 * Contributors:
 *      * CS
 * Licence:
 *      * MIT
 */

#pragma once

#include "Application.hpp"
#include "Framebuffer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>

#include "asset.hpp"
#include "glError.hpp"

#define ENSURE_EVEN(amount) ((amount) % 2 ? (amount) + 1 : (amount))

class MyGaussianblur : public Application {
   public:
    MyGaussianblur();
    ~MyGaussianblur();

   protected:
    std::string title = "Gaussianblur";
    void loop() override;
    void anotherImGui() override;
    void processInput(GLFWwindow* window);

   private:
    const int MAX_AMOUNT = 50;
    int amount = MAX_AMOUNT;  // amount of blur
    std::vector<std::shared_ptr<Fbo>> pingpongFBO{std::make_shared<Fbo>(WINDOW_WIDTH, WINDOW_HEIGHT), std::make_shared<Fbo>(WINDOW_WIDTH, WINDOW_HEIGHT)};
    const std::vector<std::string> imgPathItems = {"/picture/Lakewater_trees.jpg", "/picture/face5.jpg", "/picture/Sunset.jpg"};
    std::shared_ptr<Texture> texture = std::make_unique<Texture>(RES_DIR + imgPathItems[0]);
    std::shared_ptr<Shader> shaderProgram_Gaussianblur = std::make_unique<Shader>(SHADER_DIR "/Gaussianblur/Gaussianblur.vert", SHADER_DIR "/Gaussianblur/Gaussianblur.frag");
    std::shared_ptr<Shader> shaderProgram = std::make_unique<Shader>(SHADER_DIR "/Basic.vert", SHADER_DIR "/Basic.frag");
    void render();
};
