/**
 * MyModel.cpp
 * Contributors:
 *      * CS (author)
 * Licence:
 *      * MIT
 */
#include <iostream>
#include <vector>

#include "MyModel.hpp"

namespace {
bool captureMouse = false;
float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
// timing
float mDeltaTime = 0.0f;  // 上下帧时间间隔初始化
float lastFrame = 0.0f;
}  // namespace

MyModel::MyModel() {
    funcIndex = CS_3DMODEL;
    dirLight.direction = glm::vec3(-1.2f, -1.0f, -2.0f);
    dirLight.ambient = 0.7f;
    dirLight.diffuse = 0.7f;
    dirLight.specular = 1.0f;

    pointLight.position = glm::vec3(0.7f, 0.2f, 2.0f);
    pointLight.ambient = 0.7f;
    pointLight.diffuse = 0.7f;
    pointLight.specular = 1.0f;
    pointLight.constant = 1.0f;
    pointLight.linear = 0.35f;
    pointLight.quadratic = 0.44f;

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glCheckError(__FILE__, __LINE__);
}

MyModel::~MyModel() {
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
}

void MyModel::anotherImGui() {
    // 3. Show another MyPseudocolor window.
    if (show_another_window) {
        ImGui::Begin("Another Window", &show_another_window);
        ImGui::Text(("Hello from " + title + " window!").c_str());
        ImGui::Spacing();

        // Select Function
        selectFunction();

        ImGui::SeparatorText("Options");

        ImGui::Spacing();
        if (ImGui::CollapsingHeader("Direction Light")) {
            ImGui::ColorEdit3("light color", (float*)&dirLightColor);
            ImGui::SliderFloat3("direction", (float*)&dirLight.direction, -2.0f, 2.0f);
            ImGui::SliderFloat("ambient", &dirLight.ambient, 0.0f, 1.0f);
            ImGui::SliderFloat("diffuse", &dirLight.diffuse, 0.0f, 1.0f);
            ImGui::SliderFloat("specular", &dirLight.specular, 0.0f, 1.0f);
        }

        ImGui::Spacing();
        if (ImGui::CollapsingHeader("Point Light")) {
            ImGui::ColorEdit3("light color", (float*)&pointLightColor);
            ImGui::SliderFloat3("position", (float*)&pointLight.position, -20.0f, 20.0f);
            ImGui::SliderFloat("specular", (float*)&pointLight.specular, 0.0f, 1.0f);
            static int Attenuation = 0;
            std::vector<std::vector<float>> AttenuationList = {{0.7, 1.8}, {0.35, 0.44}, {0.22, 0.20}, {0.14, 0.07}, {0.09, 0.032}, {0.07, 0.017}, {0.045, 0.0075}, {0.027, 0.0028}, {0.022, 0.0019}, {0.014, 0.0007}, {0.007, 0.0002}, {0.0014, 0.000007}};
            ImGui::DragInt("Attenuation", &Attenuation, 0.1f, 0, 11);
            pointLight.linear = AttenuationList[Attenuation][0];
            pointLight.quadratic = AttenuationList[Attenuation][1];
            ImGui::Text("linear: %f, quadratic: %f", pointLight.linear, pointLight.quadratic);
        }

        ImGui::Spacing();
        ImGui::SliderFloat("scale", &scale, 0.0f, 5.0f);

        ImGui::Spacing();
        ImGui::SeparatorText("Translate");
        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
        ImGui::SliderFloat("x", (float*)&translate[0], -10.0f, 10.0f);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
        ImGui::SliderFloat("y", (float*)&translate[1], -10.0f, 10.0f);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
        ImGui::SliderFloat("z", (float*)&translate[2], -10.0f, 10.0f);

        ImGui::Spacing();
        ImGui::SliderFloat3("rotate", (float*)&rotateAngle, 0.0f, 360.0f);

        if (ImGui::Button("Save")) {
            if (show_another_window) {
                captureMouse = true;
                show_another_window = false;
                show_demo_window = false;
            }
        }

        // close another MyPseudocolor window
        ImGui::Spacing();
        if (ImGui::Button("Close window"))
            show_another_window = false;

        ImGui::End();
    }
}

void MyModel::loop() {
    float currentFrame = static_cast<float>(glfwGetTime());
    mDeltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (captureMouse) {
        glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(getWindow(), mouse_callback);
        glfwSetScrollCallback(getWindow(), scroll_callback);
    }

    if (glfwWindowShouldClose(getWindow())) {
        exit();
        return;
    }

    processInput(getWindow());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.6784f, 0.8f, 1.0f, 1.0f);

    render();
}

void MyModel::render() {
    shaderProgram->use();
    int w, h;
    float rx = 1.0f;
    float ry = 1.0f;
    glfwGetWindowSize(getWindow(), &w, &h);
    if (w <= h) {
        rx = static_cast<float>(h) / static_cast<float>(w);
    } else {
        ry = static_cast<float>(w) / static_cast<float>(h);
    }
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, translate);
    model = glm::rotate(model, glm::radians(rotateAngle[0]), glm::vec3(0.0, 1.0, 0.0));
    model = glm::rotate(model, glm::radians(rotateAngle[1]), glm::vec3(1.0, 0.0, 0.0));
    model = glm::rotate(model, glm::radians(rotateAngle[2]), glm::vec3(0.0, 0.0, 1.0));
    model = glm::scale(model, glm::vec3(1.0f * scale * rx, 1.0f * scale * ry, 1.0f * scale));
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT), 0.1f, 1000.0f);
    shaderProgram->setUniformMat4("model", model);
    shaderProgram->setUniformMat4("view", camera.GetViewMatrix());
    shaderProgram->setUniformMat4("projection", projection);
    shaderProgram->setUniform3f("viewPos", camera.Position);    // 相机观察位置
    shaderProgram->setUniform1f("material.shininess", 128.0f);  // 镜面反光度
    // direction light
    glm::vec3 diffuseColor = dirLightColor * glm::vec3(dirLight.diffuse);  // decrease the influence
    glm::vec3 ambientColor = diffuseColor * glm::vec3(dirLight.ambient);   // low influence
    glm::vec3 specularColor = dirLightColor * glm::vec3(dirLight.specular);
    shaderProgram->setUniform3f("dirLight.direction", dirLight.direction);
    shaderProgram->setUniform3f("dirLight.ambient", ambientColor);
    shaderProgram->setUniform3f("dirLight.diffuse", diffuseColor);
    shaderProgram->setUniform3f("dirLight.specular", specularColor);
    // point light
    diffuseColor = pointLightColor * glm::vec3(pointLight.diffuse);  // decrease the influence
    ambientColor = diffuseColor * glm::vec3(pointLight.ambient);     // low influence
    specularColor = pointLightColor * glm::vec3(pointLight.specular);
    shaderProgram->setUniform3f("pointLights.position", pointLight.position);
    shaderProgram->setUniform3f("pointLights.ambient", ambientColor);
    shaderProgram->setUniform3f("pointLights.diffuse", diffuseColor);
    shaderProgram->setUniform3f("pointLights.specular", specularColor);
    shaderProgram->setUniform1f("pointLights.constant", 1.0f);  // 常数项系数（参照表格）
    shaderProgram->setUniform1f("pointLights.linear", pointLight.linear);
    shaderProgram->setUniform1f("pointLights.quadratic", pointLight.quadratic);
    glCheckError(__FILE__, __LINE__);
    ourModel->Draw(*shaderProgram);
    shaderProgram->unuse();
}

void MyModel::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) noexcept {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void MyModel::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) noexcept {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void MyModel::processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::FORWARD, mDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::BACKWARD, mDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::LEFT, mDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::RIGHT, mDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::UP, mDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::DOWN, mDeltaTime);
}
