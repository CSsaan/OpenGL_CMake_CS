/**
 * MySkinsmooth.cpp
 * Contributors:
 *      * CS (author)
 * Licence:
 *      * MIT
 */
#include "MySkinsmooth.hpp"

#include <iostream>
#include <vector>

MySkinsmooth::MySkinsmooth() {
    funcIndex = CS_SKINSMOOTH;
    // setting the window size and aspect ratio
    setWindowAspectRatio(texture->getWidth(), texture->getHeight());

    glDisable(GL_DEPTH_TEST);
}

MySkinsmooth::~MySkinsmooth() {
    glEnable(GL_DEPTH_TEST);
}

void MySkinsmooth::loop() {
    processInput(getWindow());
    if (glfwWindowShouldClose(getWindow())) {
        exit();
        return;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0f, 0.0f, 1.0f);

    render();
}

void MySkinsmooth::anotherImGui() {
    // 3. Show another MyPseudocolor window.
    if (show_another_window) {
        ImGui::Begin("Another Window", &show_another_window);  // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text(("Hello from " + title + " window!").c_str());
        ImGui::Spacing();

        // Select Function
        selectFunction();

        // checkbox for using local image path
        ImGui::SeparatorText("smooth Intensity");
        ImGui::SliderFloat("Intensity", &intensity, 0.0f, 1.0f);
        ImGui::SameLine();
        HelpMarker("Smooth intensity.\n It use to control the smoothness of the skin mask.\n Default: 0.5.");

        // close another MyPseudocolor window
        ImGui::Spacing();
        if (ImGui::Button("Close window"))
            show_another_window = false;

        ImGui::End();
    }
}

void MySkinsmooth::render() {
    glViewport(0, 0, WINDOW_WIDTH / DOWN_SAMPLE_RATIO, WINDOW_HEIGHT / DOWN_SAMPLE_RATIO);
    // YCbCrSkinMask
    fbo_YCbCrSkinMask->bind();
    shaderProgram_YCbCrSkinMask->use();
    glBindVertexArray(VAO);
    shaderProgram_YCbCrSkinMask->setUniformMat4("model", glm::mat4(1.0f));
    glCheckError(__FILE__, __LINE__);
    texture->bind(0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glCheckError(__FILE__, __LINE__);
    shaderProgram_YCbCrSkinMask->unuse();
    fbo_YCbCrSkinMask->unbind();

    // dilate
    fbo_dilate->bind();
    shaderProgram_dilate->use();
    glBindVertexArray(VAO);
    shaderProgram_dilate->setUniformMat4("model", glm::mat4(1.0f));
    int dilate_size = 9, nd = 0;
    std::vector<float> L_dilate_offset(dilate_size * dilate_size * 2);
    for (int y = -dilate_size / 2; y <= dilate_size / 2; y++) {
        for (int x = -dilate_size / 2; x <= dilate_size / 2; x++) {
            L_dilate_offset.at(2 * nd + 0) = 1.0f * x / 1920.0f;
            L_dilate_offset.at(2 * nd + 1) = 1.0f * y / 1080.0f;
            nd++;
        }
    }
    shaderProgram_dilate->setUniform1i("dilate_size", dilate_size);
    shaderProgram_dilate->setUniform2fv("offset", L_dilate_offset);
    glCheckError(__FILE__, __LINE__);
    fbo_YCbCrSkinMask->bindTexture(0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glCheckError(__FILE__, __LINE__);
    shaderProgram_dilate->unuse();
    fbo_dilate->unbind();

    // merge
    fbo_merge->bind();
    shaderProgram_merge->use();
    glBindVertexArray(VAO);
    shaderProgram_merge->setUniformMat4("model", glm::mat4(1.0f));
    shaderProgram_merge->setUniform2f("resolu", glm::vec2(1.0f / WINDOW_WIDTH, 1.0f / WINDOW_HEIGHT));
    glCheckError(__FILE__, __LINE__);
    fbo_dilate->bindTexture(0);
    texture_aiMask->bind(1);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glCheckError(__FILE__, __LINE__);
    shaderProgram_merge->unuse();
    fbo_merge->unbind();

    // gaussian
    fbo_gaussian->bind();
    shaderProgram_gaussian->use();
    glBindVertexArray(VAO);
    shaderProgram_gaussian->setUniformMat4("model", glm::mat4(1.0f));
    const float sigma = 5.0f;
    std::vector<float> blur_offset(255 * 2);
    std::vector<float> blur_kernel(255);
    const int blur_count = 15;
    nd = 0;
    float total_wgt = 0.0f;
    for (int y = -blur_count / 2; y <= blur_count / 2; y++) {
        for (int x = -blur_count / 2; x <= blur_count / 2; x++) {
            blur_offset.at(2 * nd + 0) = 1.0f * x / WINDOW_WIDTH;
            blur_offset.at(2 * nd + 1) = 1.0f * y / WINDOW_HEIGHT;
            blur_kernel.at(nd++) = calGaussianBlurKernel(x, y, sigma);
            total_wgt += blur_kernel[nd - 1];
        }
    }
    assert(total_wgt > 0.970f);
    for (int z = 0; z < blur_count * blur_count; z++) {
        blur_kernel.at(z) /= total_wgt;
    }
    shaderProgram_gaussian->setUniform2fv("blur_offset", blur_offset);
    shaderProgram_gaussian->setUniform1fv("blur_kernel", blur_kernel);
    shaderProgram_gaussian->setUniform1i("blur_count", blur_count);
    glCheckError(__FILE__, __LINE__);
    fbo_merge->bindTexture(0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glCheckError(__FILE__, __LINE__);
    shaderProgram_gaussian->unuse();
    fbo_gaussian->unbind();

    // downSample1
    glViewport(0, 0, WINDOW_WIDTH / DOWN_SAMPLE_RATIO, WINDOW_HEIGHT / DOWN_SAMPLE_RATIO);
    fbo_downSample1->bind();
    shaderProgram_downSample->use();
    glBindVertexArray(VAO);
    shaderProgram_downSample->setUniformMat4("model", glm::mat4(1.0f));
    shaderProgram_downSample->setUniform2f("offset", glm::vec2(2.0f, 2.0f));
    shaderProgram_downSample->setUniform2f("halfpixel", glm::vec2(0.5f * 2 / WINDOW_WIDTH, 0.5f * 2 / WINDOW_HEIGHT));
    glCheckError(__FILE__, __LINE__);
    texture->bind(0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glCheckError(__FILE__, __LINE__);
    shaderProgram_downSample->unuse();
    fbo_downSample1->unbind();

    // downSample2
    glViewport(0, 0, WINDOW_WIDTH / (DOWN_SAMPLE_RATIO * 2), WINDOW_HEIGHT / (DOWN_SAMPLE_RATIO * 2));
    fbo_downSample2->bind();
    shaderProgram_downSample->use();
    glBindVertexArray(VAO);
    shaderProgram_downSample->setUniformMat4("model", glm::mat4(1.0f));
    shaderProgram_downSample->setUniform2f("offset", glm::vec2(2.0f, 2.0f));
    shaderProgram_downSample->setUniform2f("halfpixel", glm::vec2(0.5f * 4 / WINDOW_WIDTH, 0.5f * 4 / WINDOW_HEIGHT));
    glCheckError(__FILE__, __LINE__);
    fbo_downSample1->bindTexture(0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glCheckError(__FILE__, __LINE__);
    shaderProgram_downSample->unuse();
    fbo_downSample2->unbind();

    // upSample1
    glViewport(0, 0, WINDOW_WIDTH / DOWN_SAMPLE_RATIO, WINDOW_HEIGHT / DOWN_SAMPLE_RATIO);
    fbo_upSample1->bind();
    shaderProgram_upSample->use();
    glBindVertexArray(VAO);
    shaderProgram_upSample->setUniformMat4("model", glm::mat4(1.0f));
    shaderProgram_upSample->setUniform2f("offset", glm::vec2(2.0f, 2.0f));
    shaderProgram_upSample->setUniform2f("halfpixel", glm::vec2(0.5f * 2 / WINDOW_WIDTH, 0.5f * 2 / WINDOW_HEIGHT));
    glCheckError(__FILE__, __LINE__);
    fbo_downSample2->bindTexture(0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glCheckError(__FILE__, __LINE__);
    shaderProgram_upSample->unuse();
    fbo_upSample1->unbind();

    // upSample2
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    fbo_upSample2->bind();
    shaderProgram_upSample->use();
    glBindVertexArray(VAO);
    shaderProgram_upSample->setUniformMat4("model", glm::mat4(1.0f));
    shaderProgram_upSample->setUniform2f("offset", glm::vec2(2.0f, 2.0f));
    shaderProgram_upSample->setUniform2f("halfpixel", glm::vec2(0.5f / WINDOW_WIDTH, 0.5f / WINDOW_HEIGHT));
    glCheckError(__FILE__, __LINE__);
    fbo_upSample1->bindTexture(0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glCheckError(__FILE__, __LINE__);
    shaderProgram_upSample->unuse();
    fbo_upSample2->unbind();

    // smooth
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    fbo_smooth->bind();
    shaderProgram_smooth->use();
    glBindVertexArray(VAO);
    shaderProgram_smooth->setUniformMat4("model", glm::mat4(1.0f));
    shaderProgram_smooth->setUniform1f("intensity", intensity);
    texture->bind(0);
    glCheckError(__FILE__, __LINE__);
    fbo_upSample2->bindTexture(1);
    glCheckError(__FILE__, __LINE__);
    fbo_gaussian->bindTexture(2);
    glCheckError(__FILE__, __LINE__);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glCheckError(__FILE__, __LINE__);
    shaderProgram_smooth->unuse();
    fbo_smooth->unbind();

    // final
    glViewport(0, 0, getWidth(), getHeight());
    shaderProgram->use();
    glBindVertexArray(VAO);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
    model = glm::scale(model, glm::vec3(1.0f));
    shaderProgram->setUniformMat4("model", model);
    glCheckError(__FILE__, __LINE__);
    fbo_smooth->bindTexture(0);  // [x]: use final fbo's texture
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glCheckError(__FILE__, __LINE__);
    shaderProgram->unuse();
}

float MySkinsmooth::calGaussianBlurKernel(float x, float y, float sigma) {
    float r = sqrtf(x * x + y * y);
    return pow(2.718281828459, -r * r / 2.0 / pow(sigma, 2.0)) / pow(2 * 3.141592653589793, 0.50) / sigma;
}

void MySkinsmooth::processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        std::cout << " GLFW_KEY_W " << std::endl;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        std::cout << " GLFW_KEY_S " << std::endl;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        std::cout << " GLFW_KEY_A " << std::endl;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        std::cout << " GLFW_KEY_D " << std::endl;
}
