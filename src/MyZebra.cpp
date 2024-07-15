/**
 * MyZebra.cpp
 * Contributors:
 *      * CS (author)
 * Licence:
 *      * MIT
 */
#include "MyZebra.hpp"

#include <iostream>
#include <vector>

MyZebra::MyZebra() {
    funcIndex = CS_ZEBRA;
    // setting the window size and aspect ratio
    setWindowAspectRatio(texture->getWidth(), texture->getHeight());
}

void MyZebra::anotherImGui() {
    // 3. Show another MyPseudocolor window.
    if (show_another_window) {
        ImGui::Begin("Another Window", &show_another_window);  // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text(("Hello from " + title + " window!").c_str());
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
        ImGui::SeparatorText("Low-High Threads");
        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
        ImGui::SliderFloat("Low", &lowHigtThreads[0], 0.0f, 1.0f);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
        ImGui::SliderFloat("High", &lowHigtThreads[1], lowHigtThreads[0], 1.0f);

        // Close
        ImGui::Spacing();
        if (ImGui::Button("Close window"))
            show_another_window = false;

        ImGui::End();
    }
}

void MyZebra::loop() {
    processInput(getWindow());
    if (glfwWindowShouldClose(getWindow())) {
        exit();
        return;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.6784f, 0.8f, 1.0f, 1.0f);

    render();
}

void MyZebra::render() {
    shaderProgram->use();
    glBindVertexArray(VAO);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
    model = glm::scale(model, glm::vec3(1.0f));
    shaderProgram->setUniformMat4("model", model);
    shaderProgram->setUniform1f("IGlobalTime", static_cast<float>(glfwGetTime()));
    shaderProgram->setUniform2f("iResolution", glm::vec2(1.0f * WINDOW_WIDTH, 1.0f * WINDOW_HEIGHT));
    shaderProgram->setUniform2f("iLowHighThreads", lowHigtThreads);
    glCheckError(__FILE__, __LINE__);
    texture->bind(0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glCheckError(__FILE__, __LINE__);
    shaderProgram->unuse();
}

void MyZebra::processInput(GLFWwindow* window) {
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
