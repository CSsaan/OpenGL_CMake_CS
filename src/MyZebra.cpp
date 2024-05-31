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
  texture->load(RES_DIR "/picture/Lakewater_trees.jpg");
  texture->bind(0);
  glCheckError(__FILE__, __LINE__);

  // create VAO & VBO
  float vertices[] = {
    -1.0, -1.0, 0.0,  0.0, 1.0,
     1.0, -1.0, 0.0,  1.0, 1.0,
     1.0,  1.0, 0.0,  1.0, 0.0,
    -1.0, -1.0, 0.0,  0.0, 1.0,
     1.0,  1.0, 0.0,  1.0, 0.0,
    -1.0,  1.0, 0.0,  0.0, 0.0	
  };
  unsigned int VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);
  glCheckError(__FILE__, __LINE__);
}

void MyZebra::loop() {
  processInput(getWindow());
  if (glfwWindowShouldClose(getWindow()))
  {
    exit();
    return;
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.6784f, 0.8f, 1.0f, 1.0f);

  render();
}

void MyZebra::render() {
  shaderProgram.use();
  glBindVertexArray(VAO);
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
  model = glm::scale(model, glm::vec3(1.0f));
  shaderProgram.setUniform("model", model);
  shaderProgram.setUniform("IGlobalTime", static_cast<float>(glfwGetTime()));
  shaderProgram.setUniform("iResolution", glm::vec2(1.0f*WINDOW_WIDTH, 1.0f*WINDOW_HEIGHT));
  glCheckError(__FILE__, __LINE__);
  texture->bind(0);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glCheckError(__FILE__, __LINE__);
  shaderProgram.unuse();
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
