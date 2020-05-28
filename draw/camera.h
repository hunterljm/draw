#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <GLFW\glfw3.h>

class CCamera
{
public:
	CCamera(GLFWwindow* window_) : window(window_) {

	}
	CCamera() {

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
		float cameraSpeed = 0.0003f * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			cameraPos += cameraSpeed * cameraFront;
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			cameraPos -= cameraSpeed * cameraFront;
		}
		cameraPos.y = 0.0f;
		//else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		//	cameraPos += cameraUp * cameraSpeed;
		//}
		//else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		//	cameraPos -= cameraUp * cameraSpeed;
		//}
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

		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		pitch = pitch > 89.0f ? 89.0f : pitch;
		pitch = pitch < -89.0f ? -89.0f : pitch;

		glm::vec3 front;
		front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		cameraFront = glm::normalize(front);
	}

	// 滚轮回调函数
	void scroll_callback(double xoffset, double yoffset)
	{
		if (fov >= 1.0f && fov <= 45.0f) {
			fov -= yoffset;
		}
		fov = fov <= 1.0f ? 1.0f : fov;
		fov = fov >= 45.0f ? 45.0f : fov;
	}

	glm::vec3 GetCameraPos() {
		return cameraPos;
	}

	glm::vec3 GetCameraFront() {
		return cameraFront;
	}

	glm::vec3 GetCameraUp() {
		return cameraUp;
	}

	float GetFov() {
		return fov;
	}

private:
	// 摄像机初始位置
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// 每帧渲染时间
	float deltaTime = 0.0f; // 当前帧与上一帧的时间差
	float lastFrame = 0.0f; // 上一帧时间

	// 鼠标初始位置
	float lastX = 400.0f, lastY = 300.0f;
	bool firstMouse = true;

	// 俯仰角和偏航角配置
	float pitch = 0.0f;
	float yaw = -90.0f;

	// 视野角度配置
	float fov = 45.0f;

	// 窗口
	GLFWwindow* window;
};