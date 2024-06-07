/**
 * MySkinsmooth.hpp skeleton
 * Contributors:
 *      * CS
 * Licence:
 *      * MIT
 */

#pragma once

#include "Application.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Framebuffer.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>

#include "asset.hpp"
#include "glError.hpp"

#define DOWN_SAMPLE_RATIO 2

class MySkinsmooth : public Application {
public:
    MySkinsmooth();
    ~MySkinsmooth();

protected:
    void loop() override;
    void processInput(GLFWwindow *window);

private:
    unsigned int VAO{0};
    std::unique_ptr<Texture> texture = std::make_unique<Texture>();
    std::unique_ptr<Texture> texture_aiMask = std::make_unique<Texture>();
    // YCbCrSkinMask
    std::unique_ptr<Fbo> fbo_YCbCrSkinMask = std::make_unique<Fbo>(WINDOW_WIDTH/DOWN_SAMPLE_RATIO, WINDOW_HEIGHT/DOWN_SAMPLE_RATIO);
    std::unique_ptr<Shader> shaderProgram_YCbCrSkinMask = std::make_unique<Shader>(SHADER_DIR "/YCbCrSkinMask.vert", SHADER_DIR "/YCbCrSkinMask.frag");
    // dilate
    std::unique_ptr<Fbo> fbo_dilate = std::make_unique<Fbo>(WINDOW_WIDTH/DOWN_SAMPLE_RATIO, WINDOW_HEIGHT/DOWN_SAMPLE_RATIO);
    std::unique_ptr<Shader> shaderProgram_dilate = std::make_unique<Shader>(SHADER_DIR "/Dilate.vert", SHADER_DIR "/Dilate.frag");
    // merge
    std::unique_ptr<Fbo> fbo_merge = std::make_unique<Fbo>(WINDOW_WIDTH/DOWN_SAMPLE_RATIO, WINDOW_HEIGHT/DOWN_SAMPLE_RATIO);
    std::unique_ptr<Shader> shaderProgram_merge = std::make_unique<Shader>(SHADER_DIR "/Merge.vert", SHADER_DIR "/Merge.frag");
    // gaussian
    std::unique_ptr<Fbo> fbo_gaussian = std::make_unique<Fbo>(WINDOW_WIDTH/DOWN_SAMPLE_RATIO, WINDOW_HEIGHT/DOWN_SAMPLE_RATIO);
    std::unique_ptr<Shader> shaderProgram_gaussian = std::make_unique<Shader>(SHADER_DIR "/GaussianBlur.vert", SHADER_DIR "/GaussianBlur.frag");
    // DownSample1 & DownSample2
    std::unique_ptr<Fbo> fbo_downSample1 = std::make_unique<Fbo>(WINDOW_WIDTH/DOWN_SAMPLE_RATIO, WINDOW_HEIGHT/DOWN_SAMPLE_RATIO);
    std::unique_ptr<Fbo> fbo_downSample2 = std::make_unique<Fbo>(WINDOW_WIDTH/(DOWN_SAMPLE_RATIO*2), WINDOW_HEIGHT/(DOWN_SAMPLE_RATIO*2));
    std::unique_ptr<Shader> shaderProgram_downSample = std::make_unique<Shader>(SHADER_DIR "/DownSample.vert", SHADER_DIR "/DownSample.frag");
    // UpSample1 & UpSample2
    std::unique_ptr<Fbo> fbo_upSample1 = std::make_unique<Fbo>(WINDOW_WIDTH/DOWN_SAMPLE_RATIO, WINDOW_HEIGHT/DOWN_SAMPLE_RATIO);
    std::unique_ptr<Fbo> fbo_upSample2 = std::make_unique<Fbo>(WINDOW_WIDTH, WINDOW_HEIGHT);
    std::unique_ptr<Shader> shaderProgram_upSample = std::make_unique<Shader>(SHADER_DIR "/UpSample.vert", SHADER_DIR "/UpSample.frag");
    // smooth
    std::unique_ptr<Fbo> fbo_smooth = std::make_unique<Fbo>(WINDOW_WIDTH, WINDOW_HEIGHT);
    std::unique_ptr<Shader> shaderProgram_smooth = std::make_unique<Shader>(SHADER_DIR "/Smooth.vert", SHADER_DIR "/Smooth.frag");

    // final normal 2d 
    std::unique_ptr<Shader> shaderProgram = std::make_unique<Shader>(SHADER_DIR "/Basic.vert", SHADER_DIR "/Basic.frag");

    void render();
    float calGaussianBlurKernel(float x, float y, float sigma);
};

