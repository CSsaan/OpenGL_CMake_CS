/**
 * MyWaveform.cpp
 * Contributors:
 *      * CS (author)
 * Licence:
 *      * MIT
 */
#include "MyWaveform.hpp"

#include <iostream>
#include <vector>

MyWaveform::MyWaveform() {
    funcIndex = CS_WAVEFORM;
    shaderProgram_waveform = std::make_unique<Shader>((SHADER_DIR + vertPath).c_str(), (SHADER_DIR + fragPath).c_str());

    // setting the window size and aspect ratio
    setWindowAspectRatio(texture->getWidth(), texture->getHeight());

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

MyWaveform::~MyWaveform() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDisable(GL_BLEND);
}

void MyWaveform::anotherImGui() {
    // 3. Show another MyPseudocolor window.
    if (show_another_window) {
        ImGui::Begin("Another Window", &show_another_window);  // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text(("Hello from " + title + " window!").c_str());
        ImGui::Spacing();

        // Select Function
        selectFunction();

        ImGui::SeparatorText("Select Mode");
        static int e = 0;
        ImGui::RadioButton("Luma", &e, 0);
        ImGui::SameLine();
        ImGui::RadioButton("RGB", &e, 1);
        ImGui::SameLine();
        ImGui::RadioButton("RGB split", &e, 2);
        if (e == 0) {
            vertPath = "/Waveform/Waveform.vert";
            fragPath = "/Waveform/Waveform.frag";
        } else if (e == 1) {
            vertPath = "/Waveform/Waveform.vert";
            fragPath = "/Waveform/Waveform_RGB.frag";
        } else if (e == 2) {
            vertPath = "/Waveform/Waveform.vert";
            fragPath = "/Waveform/Waveform_RGBsplit.frag";
        }
        shaderProgram_waveform.reset();  // 释放资源
        shaderProgram_waveform = std::make_unique<Shader>((SHADER_DIR + vertPath).c_str(), (SHADER_DIR + fragPath).c_str());

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

        // Close
        ImGui::Spacing();
        if (ImGui::Button("Close"))
            show_another_window = false;

        ImGui::End();
    }
}

void MyWaveform::loop() {
    processInput(getWindow());
    if (glfwWindowShouldClose(getWindow())) {
        exit();
        return;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0f, 0.0f, 1.0f);

    render();
}

void MyWaveform::render() {
    // original
    shaderProgram_original->use();
    glBindVertexArray(VAO);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
    model = glm::scale(model, glm::vec3(1.0f));
    shaderProgram_original->setUniformMat4("model", model);
    glCheckError(__FILE__, __LINE__);
    texture->bind(0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glCheckError(__FILE__, __LINE__);
    shaderProgram_original->unuse();
    // waveform
    shaderProgram_waveform->use();
    glBindVertexArray(VAO);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.5f, -0.5f, 0.0f));
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
    model = glm::scale(model, glm::vec3(0.25f, 0.25f * 0.75f * texture->getWidth() / texture->getHeight(), 0.25f));
    shaderProgram_waveform->setUniformMat4("model", model);
    shaderProgram_waveform->setUniform2f("iResolution", glm::vec2(1.0f * WINDOW_WIDTH, 1.0f * WINDOW_HEIGHT));
    glCheckError(__FILE__, __LINE__);
    texture->bind(0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glCheckError(__FILE__, __LINE__);
    shaderProgram_waveform->unuse();
}

void MyWaveform::processInput(GLFWwindow* window) {
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
