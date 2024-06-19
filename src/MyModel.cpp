/**
 * MyModel.cpp
 * Contributors:
 *      * CS (author)
 * Licence:
 *      * MIT
 */
#include "MyModel.hpp"

#include <iostream>
#include <vector>

namespace {
    float lastX = WINDOW_WIDTH / 2.0f;
    float lastY = WINDOW_HEIGHT / 2.0f;
    bool firstMouse = true;
    Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
    // timing
    float mDeltaTime = 0.0f; // 上下帧时间间隔初始化
    float lastFrame = 0.0f;
}

MyModel::MyModel() {
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glCheckError(__FILE__, __LINE__);
}

MyModel::~MyModel() {
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
}

void MyModel::loop() {
    float currentFrame = static_cast<float>(glfwGetTime());
    mDeltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    processInput(getWindow());
    glfwSetCursorPosCallback(getWindow(), mouse_callback);
    glfwSetScrollCallback(getWindow(), scroll_callback);
    glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwWindowShouldClose(getWindow())) {
        exit();
        return;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.6784f, 0.8f, 1.0f, 1.0f);

    render();
}

void MyModel::render() {
    shaderProgram->use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
    model = glm::scale(model, glm::vec3(1.0f));
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT), 0.1f, 1000.0f);
    shaderProgram->setUniformMat4("model", model);
    shaderProgram->setUniformMat4("view", camera.GetViewMatrix());
    shaderProgram->setUniformMat4("projection", projection);
    shaderProgram->setUniform3f("viewPos", camera.Position);// 相机观察位置
    shaderProgram->setUniform1f("material.shininess", 128.0f); // 镜面反光度
    // render light
    glm::vec3 lightColor(1.0f);
    // lightColor.x = abs(static_cast<float>(sin(glfwGetTime() * 1.0f))); // R
    // lightColor.y = abs(static_cast<float>(sin(glfwGetTime() * 2.7f))); // G
    // lightColor.z = abs(static_cast<float>(sin(glfwGetTime() * 4.3f))); // B
    glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);             // decrease the influence
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.5f);           // low influence
    // direction light
    shaderProgram->setUniform3f("dirLight.direction", -1.2f, -1.0f, -2.0f);
    shaderProgram->setUniform3f("dirLight.ambient", 0.5f, 0.5f, 0.5f);
    shaderProgram->setUniform3f("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
    shaderProgram->setUniform3f("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // point light
    shaderProgram->setUniform3f("pointLights.position", 0.7f, 0.2f, 2.0f);
    shaderProgram->setUniform3f("pointLights.ambient", ambientColor);
    shaderProgram->setUniform3f("pointLights.diffuse", diffuseColor);
    shaderProgram->setUniform3f("pointLights.specular", 1.0f, 1.0f, 1.0f);
    shaderProgram->setUniform1f("pointLights.constant", 1.0f); // 常数项系数（参照表格）
    shaderProgram->setUniform1f("pointLights.linear", 0.35f);
    shaderProgram->setUniform1f("pointLights.quadratic", 0.44f);
    glCheckError(__FILE__, __LINE__);
    ourModel->Draw(*shaderProgram);
    shaderProgram->unuse();
}

void MyModel::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
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

void MyModel::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
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
}
