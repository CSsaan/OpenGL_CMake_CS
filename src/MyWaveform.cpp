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

        // IMGUI_DEMO_MARKER("Image");
        if (ImGui::CollapsingHeader("Image")) {
            static ImGuiComboFlags flags = 0;
            static int item_current_idx = 0;  // Here we store our selection data as an index
            std::string combo_preview_value = imgPathItems[item_current_idx];

            // checkbox for using local image path
            ImGui::SeparatorText("Select Image");
            static bool useLocalImgPath = false;
            ImGui::Checkbox("Use Local Image", &useLocalImgPath);
            ImGui::SameLine();
            HelpMarker("Input image path, or Select image from directory.");
            if (useLocalImgPath) {
                static char buf1[1024] = "";
                static bool inputComplete = false;
                ImGui::InputText(" ", buf1, IM_ARRAYSIZE(buf1));
                ImGui::SameLine();
                ImGui::Checkbox("input image path", &inputComplete);
                if (inputComplete) {
                    texture->update(buf1);  // update the texture
                    setWindowAspectRatio(texture->getWidth(), texture->getHeight());
                }
            } else if (ImGui::BeginCombo("select picture", combo_preview_value.c_str(), flags)) {
                for (int n = 0; n < (int)imgPathItems.size(); n++) {
                    const bool is_selected = (item_current_idx == n);
                    if (ImGui::Selectable((RES_DIR + imgPathItems[n]).c_str(), is_selected)) {
                        item_current_idx = n;
                        texture->update(RES_DIR + imgPathItems[item_current_idx]);  // update the texture
                        setWindowAspectRatio(texture->getWidth(), texture->getHeight());
                    }
                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
        }

        // close another MyPseudocolor window
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
