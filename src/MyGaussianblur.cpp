/**
 * MyGaussianblur.cpp
 * Contributors:
 *      * CS (author)
 * Licence:
 *      * MIT
 */
#include "MyGaussianblur.hpp"

#include <iostream>
#include <vector>

MyGaussianblur::MyGaussianblur() {
    funcIndex = CS_GAUSSIANBLUR;
    // setting the window size and aspect ratio
    setWindowAspectRatio(texture->getWidth(), texture->getHeight());

    glDisable(GL_DEPTH_TEST);
}

MyGaussianblur::~MyGaussianblur() {
    glEnable(GL_DEPTH_TEST);
}

void MyGaussianblur::anotherImGui() {
    if (show_another_window) {
        ImGui::SetNextWindowSize(ImVec2(430, 200), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Another Window", &show_another_window)) {
            ImGui::Text("Hello from %s window!", title.c_str());
            ImGui::Spacing();

            // Select Function
            selectFunction();

            ImGui::SeparatorText("");
            // Select Image
            if (ImGui::CollapsingHeader("Image")) {
                std::string selectedImagePath = imgPathItems[0];
                ImGui::SetNextItemWidth(ImGui::GetFontSize() * 10);
                if (ImGui::BeginCombo("select picture", selectedImagePath.c_str())) {
                    for (const auto& imgPath : imgPathItems) {
                        if (ImGui::Selectable(imgPath.c_str())) {
                            selectedImagePath = imgPath;
                            texture->update(RES_DIR + selectedImagePath);
                            setWindowAspectRatio(texture->getWidth(), texture->getHeight());
                        }
                    }
                    ImGui::EndCombo();
                }
                ImGui::SameLine();
                static bool useLocalImgPath = false;
                ImGui::Checkbox("Use Local Image", &useLocalImgPath);
                if (useLocalImgPath) {
                    static char inputBuffer[1024] = "";
                    ImGui::InputTextWithHint(" ", "Image path", inputBuffer, IM_ARRAYSIZE(inputBuffer), ImGuiInputTextFlags_EnterReturnsTrue);
                    if (ImGui::IsItemDeactivatedAfterEdit()) {
                        texture->update(inputBuffer);
                        setWindowAspectRatio(texture->getWidth(), texture->getHeight());
                    }
                }
            }

            // Options
            ImGui::Separator();
            ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
            ImGui::SliderInt("strength", &amount, 0, MAX_AMOUNT, "%d", ImGuiSliderFlags_AlwaysClamp);
            amount = ENSURE_EVEN(amount);

            // Close
            ImGui::Spacing();
            if (ImGui::Button("Close"))
                show_another_window = false;
        }
        ImGui::End();
    }
}

void MyGaussianblur::loop() {
    processInput(getWindow());
    if (glfwWindowShouldClose(getWindow())) {
        exit();
        return;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0f, 0.0f, 1.0f);

    render();
}

void MyGaussianblur::render() {
    // Gaussian blur(FBO)
    static GLboolean horizontal = true;
    GLboolean first_iteration = true;

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    amount = ENSURE_EVEN(amount);
    for (int i = 0; i < amount; i++) {
        // fbo_Gaussianblur->bind();
        pingpongFBO[horizontal]->bind();
        shaderProgram_Gaussianblur->use();
        glBindVertexArray(VAO);
        shaderProgram_Gaussianblur->setUniformMat4("model", glm::mat4(1.0f));
        shaderProgram_Gaussianblur->setUniform2f("iResolution", static_cast<float>(texture->getWidth()), static_cast<float>(texture->getHeight()));
        glCheckError(__FILE__, __LINE__);
        if (first_iteration) {
            texture->bind(0);
        } else {
            pingpongFBO[!horizontal]->bindTexture(0);
        }
        glCheckError(__FILE__, __LINE__);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glCheckError(__FILE__, __LINE__);
        glBindTexture(GL_TEXTURE_2D, GL_NONE);
        glBindVertexArray(0);

        horizontal = !horizontal;
        if (first_iteration) {
            first_iteration = false;
        }

        shaderProgram_Gaussianblur->unuse();
        pingpongFBO[horizontal]->unbind();
    }

    // final render on screen
    glViewport(0, 0, getWidth(), getHeight());
    shaderProgram->use();
    glBindVertexArray(VAO);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f));
    shaderProgram->setUniformMat4("model", model);
    glCheckError(__FILE__, __LINE__);
    if (amount == 0) {
        texture->bind(0);
    } else {
        pingpongFBO[!horizontal]->bindTexture(0);
    }
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glCheckError(__FILE__, __LINE__);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindVertexArray(0);
    shaderProgram->unuse();
}

void MyGaussianblur::processInput(GLFWwindow* window) {
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
