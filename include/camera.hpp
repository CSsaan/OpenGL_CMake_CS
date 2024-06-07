/**
 ******************************************************************************
 * @file camera.h
 * @brief Camera类创建相机（）
 * 处理相机运动的输入，视角欧拉角移动、缩放。
 * GetViewMatrix()   ProcessKeyboard()   ProcessMouseMovement()   ProcessMouseScroll()
 * @author ChenShuai_01344
 * @version V1.0.0
 * @date 08/29/2022
 ******************************************************************************
 */
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// 定义窗口输入的相机移动动作
enum class Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// 相机抽象类
class Camera
{
public:
    // 欧拉角
    float Yaw{-90.0f}; //偏航角
    float Pitch{0.0f}; //俯仰角
    // 相机变量
    glm::vec3 Position{0.0f, 0.0f, 0.0f}; //相机位置
    glm::vec3 Front{0.0f, 0.0f, -1.0f};   //相机方向(z轴)
    glm::vec3 Up;                         //上方向量（y轴）
    glm::vec3 Right;                      //右侧向量（x轴）
    glm::vec3 WorldUp{0.0f, 1.0f, 0.0f};
    // camera options
    float MovementSpeed{4.5f};    // WASD移动速度
    float MouseSensitivity{0.1f}; // 鼠标灵敏度
    float Zoom{45.0f};            // 缩放
    float ScrollSensitivity{2.0f};// 滚轮灵敏度

    // constructor with vectors
    Camera(glm::vec3 position)
    {
        Position = position;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    ~Camera() = default;

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix() const
    {
        return glm::lookAt(Position, Position + Front, Up); //相机位置、目标位置、上向量
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        const auto& movement = [&]() {
            switch (direction) {
                case Camera_Movement::FORWARD:
                    return Front;
                case Camera_Movement::BACKWARD:
                    return -Front;
                case Camera_Movement::LEFT:
                    return -Right;
                default:
                    return Right;
            }
        }();
        Position += movement * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true, bool PitchReversed = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        if (PitchReversed) 
            Pitch += yoffset;
        else
            Pitch -= yoffset;
        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
            Pitch = glm::clamp(Pitch, -89.0f, 89.0f);
        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= yoffset * ScrollSensitivity;
        Zoom = glm::clamp(Zoom, 1.0f, 45.0f);
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = std::cos(glm::radians(Yaw)) * std::cos(glm::radians(Pitch));
        front.y = std::sin(glm::radians(Pitch));
        front.z = std::sin(glm::radians(Yaw)) * std::cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector（叉乘计算右轴和上轴）
        Right = glm::normalize(glm::cross(Front, WorldUp)); // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
