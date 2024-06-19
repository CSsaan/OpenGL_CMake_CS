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
    std::unique_ptr<Texture> texture = std::make_unique<Texture>(RES_DIR "/picture/face5.jpg");
    std::unique_ptr<Texture> texture_aiMask = std::make_unique<Texture>(RES_DIR "/picture/face5_aiMask.jpg");
    // YCbCrSkinMask
    std::unique_ptr<Fbo> fbo_YCbCrSkinMask = std::make_unique<Fbo>(WINDOW_WIDTH/DOWN_SAMPLE_RATIO, WINDOW_HEIGHT/DOWN_SAMPLE_RATIO);
    std::unique_ptr<Shader> shaderProgram_YCbCrSkinMask = std::make_unique<Shader>(SHADER_DIR "/Skinsmoothing/YCbCrSkinMask.vert", SHADER_DIR "/Skinsmoothing/YCbCrSkinMask.frag");
    // dilate
    std::unique_ptr<Fbo> fbo_dilate = std::make_unique<Fbo>(WINDOW_WIDTH/DOWN_SAMPLE_RATIO, WINDOW_HEIGHT/DOWN_SAMPLE_RATIO);
    std::unique_ptr<Shader> shaderProgram_dilate = std::make_unique<Shader>(SHADER_DIR "/Skinsmoothing/Dilate.vert", SHADER_DIR "/Skinsmoothing/Dilate.frag");
    // merge
    std::unique_ptr<Fbo> fbo_merge = std::make_unique<Fbo>(WINDOW_WIDTH/DOWN_SAMPLE_RATIO, WINDOW_HEIGHT/DOWN_SAMPLE_RATIO);
    std::unique_ptr<Shader> shaderProgram_merge = std::make_unique<Shader>(SHADER_DIR "/Skinsmoothing/Merge.vert", SHADER_DIR "/Skinsmoothing/Merge.frag");
    // gaussian
    std::unique_ptr<Fbo> fbo_gaussian = std::make_unique<Fbo>(WINDOW_WIDTH/DOWN_SAMPLE_RATIO, WINDOW_HEIGHT/DOWN_SAMPLE_RATIO);
    std::unique_ptr<Shader> shaderProgram_gaussian = std::make_unique<Shader>(SHADER_DIR "/Skinsmoothing/GaussianBlur.vert", SHADER_DIR "/Skinsmoothing/GaussianBlur.frag");
    // DownSample1 & DownSample2
    std::unique_ptr<Fbo> fbo_downSample1 = std::make_unique<Fbo>(WINDOW_WIDTH/DOWN_SAMPLE_RATIO, WINDOW_HEIGHT/DOWN_SAMPLE_RATIO);
    std::unique_ptr<Fbo> fbo_downSample2 = std::make_unique<Fbo>(WINDOW_WIDTH/(DOWN_SAMPLE_RATIO*2), WINDOW_HEIGHT/(DOWN_SAMPLE_RATIO*2));
    std::unique_ptr<Shader> shaderProgram_downSample = std::make_unique<Shader>(SHADER_DIR "/Skinsmoothing/DownSample.vert", SHADER_DIR "/Skinsmoothing/DownSample.frag");
    // UpSample1 & UpSample2
    std::unique_ptr<Fbo> fbo_upSample1 = std::make_unique<Fbo>(WINDOW_WIDTH/DOWN_SAMPLE_RATIO, WINDOW_HEIGHT/DOWN_SAMPLE_RATIO);
    std::unique_ptr<Fbo> fbo_upSample2 = std::make_unique<Fbo>(WINDOW_WIDTH, WINDOW_HEIGHT);
    std::unique_ptr<Shader> shaderProgram_upSample = std::make_unique<Shader>(SHADER_DIR "/Skinsmoothing/UpSample.vert", SHADER_DIR "/Skinsmoothing/UpSample.frag");
    // smooth
    std::unique_ptr<Fbo> fbo_smooth = std::make_unique<Fbo>(WINDOW_WIDTH, WINDOW_HEIGHT);
    std::unique_ptr<Shader> shaderProgram_smooth = std::make_unique<Shader>(SHADER_DIR "/Skinsmoothing/Smooth.vert", SHADER_DIR "/Skinsmoothing/Smooth.frag");

    // final normal 2d 
    std::unique_ptr<Shader> shaderProgram = std::make_unique<Shader>(SHADER_DIR "/Basic.vert", SHADER_DIR "/Basic.frag");

    void render();
    float calGaussianBlurKernel(float x, float y, float sigma);
};

