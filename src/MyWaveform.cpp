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

MyWaveform::MyWaveform(int selectShader) {
    if(selectShader == 0) {
        vertPath = "/Waveform/Waveform.vert";
        fragPath = "/Waveform/Waveform.frag";
    }
    else if(selectShader == 1) { // TODO: RGB
        vertPath = "/Waveform/Waveform_2.vert";
        fragPath = "/Waveform/Waveform_2.frag";
    }
    else if(selectShader == 2) { // TODO: 3RGB
        vertPath = "/Waveform/Waveform_3.vert";
        fragPath = "/Waveform/Waveform_3.frag";
    }
    shaderProgram_waveform = std::make_unique<Shader>((SHADER_DIR + vertPath).c_str(), (SHADER_DIR + fragPath).c_str());

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    texture->load(RES_DIR "/picture/Sunset.jpg");
    texture->bind(0);
    glCheckError(__FILE__, __LINE__);

    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    glCheckError(__FILE__, __LINE__);
}

MyWaveform::~MyWaveform() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDisable(GL_BLEND);
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
    model = glm::scale(model, glm::vec3(0.25f));
    shaderProgram_waveform->setUniformMat4("model", model);
    shaderProgram_waveform->setUniform2f("iResolution", glm::vec2(1.0f*WINDOW_WIDTH, 1.0f*WINDOW_HEIGHT));
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
