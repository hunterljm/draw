#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <GLFW\glfw3.h>

const float CAMERA_SPEED = 0.00005f;	// ������ƶ��ٶ�
const float SENSITIVITY = 0.1f;		// ����ƶ��Ľ��ٶ�
const float PITCH = 0.0f;			// ������
const float YAW = -90.f;			// ƫ����
const float FOV = 45.0f;			// ��Ұ�Ƕ�

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

		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		pitch = pitch > 89.0f ? 89.0f : pitch;
		pitch = pitch < -89.0f ? -89.0f : pitch;

		UpdateCameraVectors();
	}

	// ���ֻص�����
	void scroll_callback(double xoffset, double yoffset)
	{
		fov -= yoffset;
		fov = fov <= 1.0f ? 1.0f : fov;
		fov = fov >= 45.0f ? 45.0f : fov;
	}

	// ��ȡ�۲����
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}

	// ��ȡ͸�Ӿ���
	glm::mat4 GetPerspectiveMatirx(float w_h_rate, float min, float max)
	{
		return glm::perspective(glm::radians(fov), w_h_rate, min, max);
	}

	// ��ȡ��ͷ��λ��
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
	// �������ʼλ��
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	// ������ƶ�����
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// ������ƶ��ٶ�
	float camera_speed;

	// ÿ֡��Ⱦʱ��
	float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
	float lastFrame = 0.0f; // ��һ֡ʱ��

	// ����ʼλ��
	float lastX, lastY;
	bool firstMouse = true;

	// ���ת����ٶ�
	float sensitivity;

	// �����Ǻ�ƫ��������
	float pitch = PITCH;
	float yaw = YAW;

	// ��Ұ�Ƕ�����
	float fov = FOV;

	// ����
	GLFWwindow* window;
};