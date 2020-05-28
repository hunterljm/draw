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

	// ÿ֡ˢ��
	void Update()
	{
		// ����ÿ֡ʱ���
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = deltaTime;
	}

	// ��ⰴ������
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

	// ����������
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

	// ���ֻص�����
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
	// �������ʼλ��
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// ÿ֡��Ⱦʱ��
	float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
	float lastFrame = 0.0f; // ��һ֡ʱ��

	// ����ʼλ��
	float lastX = 400.0f, lastY = 300.0f;
	bool firstMouse = true;

	// �����Ǻ�ƫ��������
	float pitch = 0.0f;
	float yaw = -90.0f;

	// ��Ұ�Ƕ�����
	float fov = 45.0f;

	// ����
	GLFWwindow* window;
};