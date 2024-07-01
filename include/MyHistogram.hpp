/**
 * MyHistogram.hpp skeleton
 * Contributors:
 *      * CS
 * Licence:
 *      * MIT
 */

#pragma once

#include <memory>
#include "Application.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "ComputeShader.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>

#include "asset.hpp"
#include "glError.hpp"

#define HISTOGRAM_SIZE 256

class MyHistogram : public Application {
public:
    MyHistogram();
    ~MyHistogram();

protected:
    std::string title = "Histogram";
    void loop() override;
    void anotherImGui() override;
    void processInput(GLFWwindow *window);

private:
    int fCounter = 0;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    void InitComputeShader();
    GLuint histogramBuffer;
    unsigned char* pixels = nullptr;
    int img_width, img_height, img_components;
    unsigned int inputTexture;
    const std::vector<std::string> imgPathItems = { "/picture/Lakewater_trees.jpg", "/picture/face5.jpg", "/picture/Sunset.jpg" };
    std::vector<int> useRGB_choseColor = {1, 0}; // 1:RGB_colors  other:One_color | 1: R, 2: G, 3: B, 4: Y
    float* GetTextureData(GLuint width, GLuint height, GLuint channels, GLuint texID);
    std::unique_ptr<ComputeShader> computeShader = std::make_unique<ComputeShader>(SHADER_DIR "/histogram/computeShaderHistogram.comp");
    std::unique_ptr<Shader> shaderProgram_image = std::make_unique<Shader>(SHADER_DIR "/Basic.vert", SHADER_DIR "/Basic.frag");
    std::unique_ptr<Shader> shaderProgram_histogram = std::make_unique<Shader>(SHADER_DIR "/histogram/Histogram.vert", SHADER_DIR "/histogram/Histogram.frag");
    void render();
};
