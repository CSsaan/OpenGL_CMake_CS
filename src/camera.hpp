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
#ifndef CAMERA_HPP
#define CAMERA_HPP

#ifndef __GLM_HPP__
#include <glm/glm.hpp>
#endif
#ifndef __MATRIX_TRANSFORM_HPP__
#include <glm/gtc/matrix_transform.hpp>
#endif

#include <vector>

// 定义窗口输入的相机移动动作
enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// 相机默认值
static const float YAW = -90.0f;             //偏航角
static const float PITCH = 0.0f;             //俯仰角
static const float SPEED = 4.5f;             // WASD移动速度2.5f
static const float SENSITIVITY = 0.1f;       //鼠标灵敏度
static const float ZOOM = 45.0f;             //缩放
static const float SCROLL_SENSITIVITY = 2.0f;//滚轮灵敏度

// 相机抽象类
class Camera
{
public:
    // 相机变量
    glm::vec3 Position; //相机位置
    glm::vec3 Front;    //相机方向(z轴)
    glm::vec3 Up;       //上方向量（y轴）
    glm::vec3 Right;    //右侧向量（x轴）
    glm::vec3 WorldUp;  //
    // 欧拉角
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    float ScrollSensitivity;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM), ScrollSensitivity(SCROLL_SENSITIVITY)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM), ScrollSensitivity(SCROLL_SENSITIVITY)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up); //相机位置、目标位置、上向量
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true, GLboolean PitchReversed = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        if (PitchReversed)
        {
            Pitch += yoffset;
        }
        else
            Pitch -= yoffset;

        // std::cout<<yoffset<<":"<<Pitch<<std::endl;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset * ScrollSensitivity;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector（叉乘计算右轴和上轴）
        Right = glm::normalize(glm::cross(Front, WorldUp)); // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
#endif // CAMERA_HPP