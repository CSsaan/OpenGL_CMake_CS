/**
 * MyVectorscope.hpp skeleton
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

// Number of divisions in histogram.
// 65536 + min and max range to speed up the tabulation
#define TOTAL_BINS 0x13333
#define HIST_SECTIONS 4
#define FLOAT_RANGE 1.2
// Minimum value in percentage
#define FLOAT_MIN -0.000001
// Maximum value in percentage
#define FLOAT_MAX 1.000001
#define VECTORSCOPE_DIVISIONS 11

const int vector_h = 512;
const int vector_w = 512;

enum clorModel {
    CS_RGB888RGB = 0,
    CS_RGB888GRAY = 1,
    CS_RGB8888RGB = 2,
    CS_RGB8888GRAY = 3,
    CS_RGB565 = 4,
    CS_BGR565 = 5,
    CS_RGB8 = 6
};

class MyVectorscope : public Application {
public:
    MyVectorscope();
    ~MyVectorscope();

protected:
    std::string title = "Vectorscope";
    void loop() override;
    void anotherImGui() override;
    void processInput(GLFWwindow *window);

private:
    int colorModel = CS_RGB888RGB;
    GLuint texture_id = 0;
    int fCounter = 0;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    int img_width, img_height, img_components;
    const std::vector<std::string> imgPathItems = { "/picture/Lakewater_trees.jpg", "/picture/face5.jpg", "/picture/Sunset.jpg" };
    unsigned char* pixels = nullptr;
    unsigned char* outPixels = new unsigned char[vector_w * vector_h * 3];
    std::unique_ptr<Shader> shaderProgram_vectorscope = std::make_unique<Shader>(SHADER_DIR "/Basic.vert", SHADER_DIR "/Basic.frag");
    std::unique_ptr<Shader> shaderProgram_picture = std::make_unique<Shader>(SHADER_DIR "/Basic.vert", SHADER_DIR "/Basic.frag");
    std::unique_ptr<Texture> texture = std::make_unique<Texture>(RES_DIR+imgPathItems[0]);
    glm::vec2 lowHigtThreads{0.3f, 0.6f};
    void render();
    void draw_point(unsigned char *outPixels, int color_model, int x, int y, int r, int g, int b);
    float getUpdatedColor(float pixel_cur, float pixel_new, float divider, float constant);
};
