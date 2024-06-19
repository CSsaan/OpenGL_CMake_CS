/**
 * MyHistogram.cpp
 * Contributors:
 *      * CS (author)
 * Licence:
 *      * MIT
 */
#include "MyHistogram.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

MyHistogram::MyHistogram(uint8_t selectColor) {
    if (selectColor == 0) {
        useRGB_choseColor = {1, 0};
    } else if (selectColor == 1) {
        useRGB_choseColor = {0, 1};
    } else if (selectColor == 2) {
        useRGB_choseColor = {0, 2};
    } else if (selectColor == 3) {
        useRGB_choseColor = {0, 3};
    } else if (selectColor == 4) {
        useRGB_choseColor = {0, 4};
    }

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSwapInterval(0); // 禁用垂直同步，即允许无限制地渲染。解除60pfs限制

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
    InitComputeShader();
}

MyHistogram::~MyHistogram() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDisable(GL_BLEND);
    glfwSwapInterval(1); // 将渲染限制在显示器的垂直刷新率
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &histogramBuffer);
    stbi_image_free(pixels);
}

void MyHistogram::InitComputeShader() {
    // query limitations 
	int max_compute_work_group_count[3];
	int max_compute_work_group_size[3];
	int max_compute_work_group_invocations;
	for (int idx = 0; idx < 3; idx++) {
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, idx, &max_compute_work_group_count[idx]);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, idx, &max_compute_work_group_size[idx]);
	}	
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &max_compute_work_group_invocations);
    std::cout << "---------------------------------------------------" << std::endl;
	std::cout << "OpenGL Limitations: " << std::endl;
	std::cout << "maximum number of work groups in X dimension: " << max_compute_work_group_count[0] << std::endl;
	std::cout << "maximum number of work groups in Y dimension: " << max_compute_work_group_count[1] << std::endl;
	std::cout << "maximum number of work groups in Z dimension: " << max_compute_work_group_count[2] << std::endl;
	std::cout << "maximum size of a work group in X dimension: " << max_compute_work_group_size[0] << std::endl;
	std::cout << "maximum size of a work group in Y dimension: " << max_compute_work_group_size[1] << std::endl;
	std::cout << "maximum size of a work group in Z dimension: " << max_compute_work_group_size[2] << std::endl;
	std::cout << "Number of invocations in a single local work group that may be dispatched to a compute shader: " << max_compute_work_group_invocations << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    // 创建输入图像纹理 
    pixels = stbi_load(imgPath.c_str(), &width, &height, &components, 0);
    if(pixels == nullptr) {
        std::cout << "Could not load file " << imgPath << std::endl;
        return;
    } 
    std::cout<< "[" << imgPath << " ImageSize] x:" << width << ", y:" << height << std::endl;
    // 创建输入和输出纹理
    glGenTextures(1, &inputTexture);
	glBindTexture(GL_TEXTURE_2D, inputTexture); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA8, GL_UNSIGNED_BYTE, pixels);
    glBindImageTexture(0, inputTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8UI); // 绑定输入纹理
    // glBindImageTexture(1, outputTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8); // 绑定输出纹理

	// 创建直方图缓冲区
    glGenBuffers(1, &histogramBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, histogramBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(unsigned int) * HISTOGRAM_SIZE * 4, NULL, GL_DYNAMIC_COPY);
    glCheckError(__FILE__, __LINE__);
}

void MyHistogram::loop() {
    processInput(getWindow());
    if (glfwWindowShouldClose(getWindow())) {
      exit();
      return;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    render();
}

void MyHistogram::render() {
    // get FPS
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	if(fCounter > 500) {
		std::cout << "FPS: " << 1 / deltaTime << std::endl;
		fCounter = 0;
	} else {
		fCounter++;
	}

    // render Image
    shaderProgram_image->use();
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, inputTexture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
    model = glm::scale(model, glm::vec3(1.0f));
    shaderProgram_image->setUniformMat4("model", model);
    glCheckError(__FILE__, __LINE__);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindTexture(GL_TEXTURE_2D, 0);
    shaderProgram_image->unuse();

    // render Compute Shaders
    computeShader->use();
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, inputTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glDispatchCompute((unsigned int)(width / 32), (unsigned int)(height / 16), 1); // 启动计算着色器程序
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT); // GL_ALL_BARRIER_BITS设置通用障碍， 确保在读取结果之前已经完成了写入
    glBindTexture(GL_TEXTURE_2D, 0);

    // 读取缓冲区数据
    /* 法一：glMapBuffer从缓冲区中读取直方图数据 */
    std::unique_ptr<unsigned int[]> histogramData(new unsigned int[HISTOGRAM_SIZE * 4]);
    std::vector<int> histogramYVector(histogramData.get(), histogramData.get() + HISTOGRAM_SIZE);
    std::vector<int> histogramRVector(histogramData.get() + HISTOGRAM_SIZE, histogramData.get() + HISTOGRAM_SIZE*2);
    std::vector<int> histogramGVector(histogramData.get() + HISTOGRAM_SIZE*2, histogramData.get() + HISTOGRAM_SIZE*3);
    std::vector<int> histogramBVector(histogramData.get() + HISTOGRAM_SIZE*3, histogramData.get() + HISTOGRAM_SIZE*4);

    auto maxElement = std::max_element(histogramRVector.begin(), histogramRVector.end());
    int maxR = (maxElement != histogramRVector.end()) ? *maxElement : 0; 
    maxElement = std::max_element(histogramGVector.begin(), histogramGVector.end());
    int maxG = (maxElement != histogramGVector.end()) ? *maxElement : 0; 
    maxElement = std::max_element(histogramBVector.begin(), histogramBVector.end());
    int maxB = (maxElement != histogramBVector.end()) ? *maxElement : 0; 
    maxElement = std::max_element(histogramYVector.begin(), histogramYVector.end());
    int maxY = (maxElement != histogramYVector.end()) ? *maxElement : 0; 
    
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, histogramBuffer);
    void* ptr = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
    memcpy(histogramData.get(), ptr, HISTOGRAM_SIZE * 4 * sizeof(unsigned int));
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    
    /* 法二：glGetBufferSubData读取缓冲区数据(需搭配glBufferSubData清空缓冲区) */
    // unsigned int histogramData[HISTOGRAM_SIZE * 4] = {0};
    // glBindBuffer(GL_SHADER_STORAGE_BUFFER, histogramBuffer);
    // glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(unsigned int) * HISTOGRAM_SIZE * 4, histogramData);
    // for (int i = 0; i < HISTOGRAM_SIZE * 4; i++) {
    //     std::cout << histogramData[i] << ",";
    // }
    // std::cout << std::endl;

    // 清空缓冲区
    /* 法一：glBufferData清空缓冲区 */
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, histogramBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(unsigned int) * HISTOGRAM_SIZE * 4, NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    /* 法二：glBufferSubData清空缓冲区 */
    // glBindBuffer(GL_SHADER_STORAGE_BUFFER, histogramBuffer);
    // unsigned int zeroData[HISTOGRAM_SIZE * 4] = {0};
    // glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(zeroData), zeroData);
    // glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    computeShader->unuse();

    // render Histogram to screen
    shaderProgram_histogram->use(); 
    glBindVertexArray(VAO);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.5f, -0.5f, 0.0f));
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
    model = glm::scale(model, glm::vec3(0.25f));
    shaderProgram_histogram->setUniformMat4("model", model);
    shaderProgram_histogram->setUniform2iv("useRGB_choseColor", useRGB_choseColor); // r:1 g:2 b:3 y:4 
    // shaderProgram_histogram->setUniform1iv("pointsY", histogramYVector);
    shaderProgram_histogram->setUniform1iv("pointsR", histogramRVector);
    shaderProgram_histogram->setUniform1iv("pointsG", histogramGVector);
    shaderProgram_histogram->setUniform1iv("pointsB", histogramBVector); 
    shaderProgram_histogram->setUniform4i("maxRGB", maxR, maxG, maxB, maxY);
    glCheckError(__FILE__, __LINE__);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glCheckError(__FILE__, __LINE__);
    shaderProgram_histogram->unuse();
}

float* MyHistogram::GetTextureData(GLuint width, GLuint height, GLuint channels, GLuint texID) {
    float* data = new float[width * height * channels];
    glBindTexture(GL_TEXTURE_2D, texID);
    if(channels==1)
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, data);
    if(channels==3)
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, data);
    if (channels == 4)
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return data; 
    // TODO: delete [] data;
}

void MyHistogram::processInput(GLFWwindow* window) {
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
