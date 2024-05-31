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

namespace
{
  float lastX = WINDOW_WIDTH / 2.0f;
  float lastY = WINDOW_HEIGHT / 2.0f;
  bool firstMouse = true;
  Camera camera(glm::vec3(0.0f, 0.0f, 2.0f));
  // timing
  float mDeltaTime = 0.0f; // 上下帧时间间隔初始化
  float lastFrame = 0.0f;
}

MyModel::MyModel() {
    texture->load(RES_DIR "/models/alliance.png");
    texture->bind(0);
    glCheckError(__FILE__, __LINE__);
}

void MyModel::loop()
{
    float currentFrame = static_cast<float>(glfwGetTime());
    mDeltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    processInput(getWindow());
    glfwSetCursorPosCallback(getWindow(), mouse_callback);
    glfwSetScrollCallback(getWindow(), scroll_callback);
    glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwWindowShouldClose(getWindow()))
    {
      exit();
      return;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.6784f, 0.8f, 1.0f, 1.0f);

    render();
}

void MyModel::render()
{
    shaderProgram.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
    model = glm::scale(model, glm::vec3(1.0f));
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT), 0.1f, 1000.0f);
    shaderProgram.setUniform("model", model);
    shaderProgram.setUniform("viewProj", camera.GetViewMatrix());
    shaderProgram.setUniform("projection", projection);
    shaderProgram.setUniform("cam_pos", camera.Position);
    glCheckError(__FILE__, __LINE__);
    texture->bind(0);
    mesh->Draw();
    glCheckError(__FILE__, __LINE__);
    shaderProgram.unuse();
}

void MyModel::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
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

void MyModel::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void MyModel::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      camera.ProcessKeyboard(FORWARD, mDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      camera.ProcessKeyboard(BACKWARD, mDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      camera.ProcessKeyboard(LEFT, mDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      camera.ProcessKeyboard(RIGHT, mDeltaTime);
}
