/**
 * MyVectorscope.cpp
 * Contributors:
 *      * CS (author)
 * Licence:
 *      * MIT
 */

#include "MyVectorscope.hpp"
#include "bccolors.hpp"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <vector>

// 调用: VIDEOSCOPE(unsigned char, int, 0xff, 3, 0)
static float radius = MIN(vector_w / 2, vector_h / 2);
#define VIDEOSCOPE(type, temp_type, max, components, use_yuv)                                                \
    {                                                                                                        \
        memset(outPixels, 0, vector_w* vector_h * 3);                                                        \
        for (int i = 0; i < img_height; i++) {                                                               \
            for (int j = 0; j < img_width; j++) {                                                            \
                int index = components * (i * img_width + j);                                                \
                type in_pixel_y = pixels[index + 0];                                                         \
                type in_pixel_u = pixels[index + 1];                                                         \
                type in_pixel_v = pixels[index + 2];                                                         \
                /* Analyze pixel */                                                                          \
                float h, s, v;                                                                               \
                temp_type r, g, b;                                                                           \
                if (use_yuv) {                                                                               \
                    if (sizeof(type) == 2) {                                                                 \
                        YUV::yuv.yuv_to_rgb_16(r, g, b, in_pixel_y, in_pixel_u, in_pixel_v);                 \
                    } else {                                                                                 \
                        YUV::yuv.yuv_to_rgb_8(r, g, b, in_pixel_y, in_pixel_u, in_pixel_v);                  \
                    }                                                                                        \
                } else {                                                                                     \
                    r = in_pixel_y;                                                                          \
                    g = in_pixel_u;                                                                          \
                    b = in_pixel_v;                                                                          \
                }                                                                                            \
                HSV::rgb_to_hsv((float)r / max, (float)g / max, (float)b / max, h, s, v);                    \
                /* Calculate vectorscope */                                                                  \
                float adjacent = cos((h + 90) / 360 * 2 * M_PI);                                             \
                float opposite = sin((h + 90) / 360 * 2 * M_PI);                                             \
                int x = round(vector_w / 2 + adjacent * (s - FLOAT_MIN) / (FLOAT_MAX - FLOAT_MIN) * radius); \
                int y = round(vector_h / 2 - opposite * (s - FLOAT_MIN) / (FLOAT_MAX - FLOAT_MIN) * radius); \
                CLAMP(x, 0, vector_w - 1);                                                                   \
                CLAMP(y, 0, vector_h - 1);                                                                   \
                /* Get color with full saturation & value */                                                 \
                float r_f, g_f, b_f;                                                                         \
                HSV::hsv_to_rgb(r_f, g_f, b_f, h, s, 1);                                                     \
                r = (int)(r_f * 255);                                                                        \
                g = (int)(g_f * 255);                                                                        \
                b = (int)(b_f * 255);                                                                        \
                /* float */                                                                                  \
                if (sizeof(type) == 4) {                                                                     \
                    r = CLIP(r, 0, 0xff);                                                                    \
                    g = CLIP(g, 0, 0xff);                                                                    \
                    b = CLIP(b, 0, 0xff);                                                                    \
                }                                                                                            \
                draw_point(outPixels, colorModel, x, vector_h - y, r, g, b);                                 \
            }                                                                                                \
        }                                                                                                    \
    }

float MyVectorscope::getUpdatedColor(float pixel_cur, float pixel_new, float divider, float constant) {
    float result = pixel_cur + (constant * pixel_new / divider);
    if (result > 255.0f)
        return 255.0f;
    return result;
}

void MyVectorscope::draw_point(unsigned char* outPixels, int color_model, int x, int y, int r, int g, int b) {
    switch (color_model) {
        case CS_RGB8888GRAY: {
            break;
        }
        case CS_RGB8888RGB: {
            int index = 4 * (x * vector_w + y);
            outPixels[index + 0] = b;
            outPixels[index + 1] = g;
            outPixels[index + 2] = r;
            break;
        }
        case CS_RGB888GRAY: {
            int index = 3 * (x * vector_w + y);
            // outPixels[index + 1] + 16.0f * 256.0f / img_height;
            unsigned char result = HSV::rgb_to_gray(outPixels[index], outPixels[index + 1], outPixels[index + 2]) + 16.0f * 256.0f / img_height;
            if (result > 255)
                result = 255;
            outPixels[index + 0] = result;
            outPixels[index + 1] = result;
            outPixels[index + 2] = result;
            break;
        }
        case CS_RGB888RGB: {
            int index = 3 * (x * vector_w + y);
            outPixels[index + 0] = getUpdatedColor((float)outPixels[index + 0], (float)r, img_height, 16.0f);
            outPixels[index + 1] = getUpdatedColor((float)outPixels[index + 1], (float)g, img_height, 16.0f);
            outPixels[index + 2] = getUpdatedColor((float)outPixels[index + 2], (float)b, img_height, 16.0f);
            break;
        }
        case CS_RGB565: {
            outPixels[2 * (x * vector_w + y) + 0] = (r & 0xf8) | (g >> 5);
            outPixels[2 * (x * vector_w + y) + 1] = ((g & 0xfc) << 5) | (b >> 3);
            break;
        }
        case CS_BGR565:
            break;
        case CS_RGB8:
            break;
    }
}

MyVectorscope::MyVectorscope() {
    funcIndex = CS_VECTORSCOPE;
    // setting the window size and aspect ratio
    setWindowAspectRatio(texture->getWidth(), texture->getHeight());

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 加载图像数据
    std::string imgPath = RES_DIR + imgPathItems[0];
    pixels = stbi_load(imgPath.c_str(), &img_width, &img_height, &img_components, 0);
    if (pixels == nullptr) {
        std::cout << "ERROR:Could not load file:" << imgPath << std::endl;
        return;
    }
    std::cout << "[" << imgPath << "]ImageSize x:" << img_width << ", y:" << img_height << std::endl;
    // 创建纹理
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    // glTexStorage2D(GL_TEXTURE_2D, 2 /* mip map levels */, GL_RGB8, 512, 512);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, vector_w, vector_h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

MyVectorscope::~MyVectorscope() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDisable(GL_BLEND);

    if (outPixels)
        delete[] outPixels;
}

void MyVectorscope::anotherImGui() {
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
        if (e == 0) {
            colorModel = CS_RGB888GRAY;
        } else if (e == 1) {
            colorModel = CS_RGB888RGB;
        }

        // Select Image
        if (ImGui::CollapsingHeader("Image")) {
            std::string selectedImagePath = imgPathItems[0];
            ImGui::SetNextItemWidth(ImGui::GetFontSize() * 10);
            if (ImGui::BeginCombo("select picture", selectedImagePath.c_str())) {
                for (const auto& imgPath : imgPathItems) {
                    if (ImGui::Selectable(imgPath.c_str())) {
                        selectedImagePath = imgPath;
                        if (pixels) {
                            stbi_image_free(pixels);
                            pixels = nullptr;
                        }
                        pixels = stbi_load((RES_DIR + selectedImagePath).c_str(), &img_width, &img_height, &img_components, 0);
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
                    if (pixels) {
                        stbi_image_free(pixels);
                        pixels = nullptr;
                    }
                    pixels = stbi_load(inputBuffer, &img_width, &img_height, &img_components, 0);
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

void MyVectorscope::loop() {
    processInput(getWindow());
    if (glfwWindowShouldClose(getWindow())) {
        exit();
        return;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    render();
}

void MyVectorscope::render() {
    // get FPS
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    if (fCounter > 500) {
        std::cout << "FPS: " << 1 / deltaTime << std::endl;
        fCounter = 0;
    } else {
        fCounter++;
    }

    shaderProgram_picture->use();
    glBindVertexArray(VAO);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(1.0f));
    shaderProgram_picture->setUniformMat4("model", model);
    glCheckError(__FILE__, __LINE__);
    texture->bind(0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glCheckError(__FILE__, __LINE__);
    shaderProgram_picture->unuse();

    VIDEOSCOPE(unsigned char, int, 0xff, 3, 0)

    shaderProgram_vectorscope->use();
    glBindVertexArray(VAO);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.5f, -0.5f, 0.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(0.25f, 0.25f * texture->getHeight() / texture->getWidth(), 0.25f));
    shaderProgram_vectorscope->setUniformMat4("model", model);
    glCheckError(__FILE__, __LINE__);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, vector_w, vector_h, GL_RGB, GL_UNSIGNED_BYTE, outPixels);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glCheckError(__FILE__, __LINE__);
    shaderProgram_vectorscope->unuse();
}

void MyVectorscope::processInput(GLFWwindow* window) {
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
