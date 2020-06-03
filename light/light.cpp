#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // gml::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "shader.h"
#include "camera.h"

// ��Ļ��С
const int screenWidth = 800;
const int screenHeight = 600;

// �����
CCamera* camera = nullptr;

// ���ڵ����ص�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// �����ص�
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera->mouse_callback(xpos, ypos);
}

// ���ּ��ص�
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera->scroll_callback(xoffset, yoffset);
}

// ��ⰴ������
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	camera->processInput();
}

int main()
{
	// GLFW ��ʼ��
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// ��������
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "map", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// ���������
	camera = new CCamera(window, glm::vec3(0.0f, 1.0f, 6.0f), glm::vec3(0.0f, 1.0f, -1.0f), glm::vec3(0.0f, 2.0f, 0.0f), screenWidth / 2, screenHeight / 2);
	if (camera == nullptr) {
		glfwTerminate();
		return -1;
	}

	// ��ʼ�� GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initalize GLAD." << std::endl;
		return -1;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	float vertices_samp[] = {
		// --λ��--				--������--
		-2.5f, -0.1f, -2.5f,  0.0f,  0.0f, -1.0f,
		 2.5f, -0.1f, -2.5f,  0.0f,  0.0f, -1.0f,
		 2.5f,  0.1f, -2.5f,  0.0f,  0.0f, -1.0f,
		 2.5f,  0.1f, -2.5f,  0.0f,  0.0f, -1.0f,
		-2.5f,  0.1f, -2.5f,  0.0f,  0.0f, -1.0f,
		-2.5f, -0.1f, -2.5f,  0.0f,  0.0f, -1.0f,

		-2.5f, -0.1f,  2.5f,  0.0f,  0.0f, 1.0f,
		 2.5f, -0.1f,  2.5f,  0.0f,  0.0f, 1.0f,
		 2.5f,  0.1f,  2.5f,  0.0f,  0.0f, 1.0f,
		 2.5f,  0.1f,  2.5f,  0.0f,  0.0f, 1.0f,
		-2.5f,  0.1f,  2.5f,  0.0f,  0.0f, 1.0f,
		-2.5f, -0.1f,  2.5f,  0.0f,  0.0f, 1.0f,

		-2.5f,  0.1f,  2.5f, -1.0f,  0.0f,  0.0f,
		-2.5f,  0.1f, -2.5f, -1.0f,  0.0f,  0.0f,
		-2.5f, -0.1f, -2.5f, -1.0f,  0.0f,  0.0f,
		-2.5f, -0.1f, -2.5f, -1.0f,  0.0f,  0.0f,
		-2.5f, -0.1f,  2.5f, -1.0f,  0.0f,  0.0f,
		-2.5f,  0.1f,  2.5f, -1.0f,  0.0f,  0.0f,

		 2.5f,  0.1f,  2.5f,  1.0f,  0.0f,  0.0f,
		 2.5f,  0.1f, -2.5f,  1.0f,  0.0f,  0.0f,
		 2.5f, -0.1f, -2.5f,  1.0f,  0.0f,  0.0f,
		 2.5f, -0.1f, -2.5f,  1.0f,  0.0f,  0.0f,
		 2.5f, -0.1f,  2.5f,  1.0f,  0.0f,  0.0f,
		 2.5f,  0.1f,  2.5f,  1.0f,  0.0f,  0.0f,

		-2.5f, -0.1f, -2.5f,  0.0f, -1.0f,  0.0f,
		 2.5f, -0.1f, -2.5f,  0.0f, -1.0f,  0.0f,
		 2.5f, -0.1f,  2.5f,  0.0f, -1.0f,  0.0f,
		 2.5f, -0.1f,  2.5f,  0.0f, -1.0f,  0.0f,
		-2.5f, -0.1f,  2.5f,  0.0f, -1.0f,  0.0f,
		-2.5f, -0.1f, -2.5f,  0.0f, -1.0f,  0.0f,

		-2.5f,  0.1f, -2.5f,  0.0f,  1.0f,  0.0f,
		 2.5f,  0.1f, -2.5f,  0.0f,  1.0f,  0.0f,
		 2.5f,  0.1f,  2.5f,  0.0f,  1.0f,  0.0f,
		 2.5f,  0.1f,  2.5f,  0.0f,  1.0f,  0.0f,
		-2.5f,  0.1f,  2.5f,  0.0f,  1.0f,  0.0f,
		-2.5f,  0.1f, -2.5f,  0.0f,  1.0f,  0.0f
	};

	// ����һ����Ͷ������� VAO��VBO
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_samp), vertices_samp, GL_STATIC_DRAW);

	// ���ö�λλ��ָ�롢��ɫ����
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	// ������� VAO
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);

	// ������ɫ��
	shader lightingShader("resources/lightingshader.vs", "resources/lightingshader.fs");
	shader sampShader("resources/sampshader.vs", "resources/sampshader.fs");

	// ��Դ��ɫ
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	// ����ԭ������ɫ
	glm::vec3 objectColor = glm::vec3(1.0f, 1.0f, 1.0f);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		camera->Update();

		// ��Դ��λ��
		glm::vec3 lightPos = glm::vec3(sin(float(glfwGetTime()))*2.f, 1.0f, sin(float(glfwGetTime()))*2.f);

		// ��ˢ��Ȼ��塢����ɫ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// ��������
		lightingShader.use();
		lightingShader.setVec3("lightColor", lightColor);
		glBindVertexArray(lightVAO);

		// �۲����
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
		// ��ά���忴������Զ��������
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(
			camera->GetPerspectiveMatirx((float)screenWidth / (float)screenHeight, 0.1f, 100.0f)
		));
		// ģ��ת������
		glm::mat4 model;
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.1f, 2.0f, 0.1f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		sampShader.use();
		sampShader.setVec3("objectColor", objectColor);
		sampShader.setVec3("lightColor", lightColor);
		sampShader.setVec3("lightPos", lightPos);
		sampShader.setVec3("viewPos", camera->Position());
		glBindVertexArray(VAO);
		glUniformMatrix4fv(glGetUniformLocation(sampShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(sampShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(
			camera->GetPerspectiveMatirx((float)screenWidth / (float)screenHeight, 0.1f, 100.0f)
		));
		glm::mat4 samp_model;
		glUniformMatrix4fv(glGetUniformLocation(sampShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(samp_model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ��鲢�����¼�����������
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	delete camera;
	glfwTerminate();
	return 0;
}