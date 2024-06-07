/**
 * MyWaveform.hpp skeleton
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

class MyWaveform : public Application {
public:
    MyWaveform();
    ~MyWaveform();

protected:
    void loop() override;
    void processInput(GLFWwindow *window);

private:
    unsigned int VAO{};
    std::unique_ptr<Shader> shaderProgram_original = std::make_unique<Shader>(SHADER_DIR "/Basic.vert", SHADER_DIR "/Basic.frag");
    std::unique_ptr<Shader> shaderProgram_waveform = std::make_unique<Shader>(SHADER_DIR "/Waveform.vert", SHADER_DIR "/Waveform.frag");
    std::unique_ptr<Texture> texture = std::make_unique<Texture>();
    void render();
};