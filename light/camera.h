#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <GLFW\glfw3.h>

const float CAMERA_SPEED = 0.00005f;	// 摄像机移动速度
const float SENSITIVITY = 0.1f;		// 鼠标移动的角速度
const float PITCH = 0.0f;			// 俯仰角
const float YAW = -90.f;			// 偏航角
const float FOV = 45.0f;			// 视野角度

class CCamera
{
public:
	CCamera(GLFWwindow* window_, glm::vec3 pos, glm::vec3 front, glm::vec3 up, float mouse_x, float mouse_y) : 
			window(window_), cameraPos(pos), cameraFront(front), cameraUp(up), lastX(mouse_x), lastY(mouse_y) {
		camera_speed = CAMERA_SPEED;
		sensitivity = SENSITIVITY;
		WorldUp = up;
		UpdateCameraVectors();
	}
	~CCamera() {

	}

	// 每帧刷新
	void Update()
	{
		// 计算每帧时间差
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = deltaTime;
	}

	// 检测按键输入
	void processInput()
	{
		float cameraSpeed = camera_speed * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			cameraPos -= Right * cameraSpeed;
		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			cameraPos += Right * cameraSpeed;
		}
		else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			cameraPos += cameraSpeed * cameraFront;
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			cameraPos -= cameraSpeed * cameraFront;
		}
		//cameraPos.y = 0.0f;
		else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			cameraPos += cameraUp * cameraSpeed;
		}
		else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			cameraPos -= cameraUp * cameraSpeed;
		}
	}

	// 检测鼠标输入
	void mouse_callback(double xpos, double ypos)
	{
		if (firstMouse) {
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		pitch = pitch > 89.0f ? 89.0f : pitch;
		pitch = pitch < -89.0f ? -89.0f : pitch;

		UpdateCameraVectors();
	}

	// 滚轮回调函数
	void scroll_callback(double xoffset, double yoffset)
	{
		fov -= yoffset;
		fov = fov <= 1.0f ? 1.0f : fov;
		fov = fov >= 45.0f ? 45.0f : fov;
	}

	// 获取观察矩阵
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}

	// 获取透视矩阵
	glm::mat4 GetPerspectiveMatirx(float w_h_rate, float min, float max)
	{
		return glm::perspective(glm::radians(fov), w_h_rate, min, max);
	}

	// 获取镜头的位置
	glm::vec3 Position()
	{
		return cameraPos;
	}

private:
	void UpdateCameraVectors() {
		glm::vec3 front;
		front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		cameraFront = glm::normalize(front);
		Right = glm::normalize(glm::cross(cameraFront, WorldUp));
		cameraUp = glm::normalize(glm::cross(Right, cameraFront));
	}

private:
	// 摄像机初始位置
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	// 摄像机移动向量
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// 摄像机移动速度
	float camera_speed;

	// 每帧渲染时间
	float deltaTime = 0.0f; // 当前帧与上一帧的时间差
	float lastFrame = 0.0f; // 上一帧时间

	// 鼠标初始位置
	float lastX, lastY;
	bool firstMouse = true;

	// 鼠标转向角速度
	float sensitivity;

	// 俯仰角和偏航角配置
	float pitch = PITCH;
	float yaw = YAW;

	// 视野角度配置
	float fov = FOV;

	// 窗口
	GLFWwindow* window;
};