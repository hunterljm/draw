#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include <iostream>
#include "shader.h"

// ���ڴ�С�����ص�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// ��ⰴ������
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main()
{
	// GLFW ����Ϊ OpenGl ����������
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);					// OpenGL ���汾 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);					// OpenGL �ΰ汾 3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// OpenGL ʹ�õ��Ǻ���ģʽ Core_profile

	// ����һ�����ڶ���
	GLFWwindow* window = glfwCreateWindow(800, 600, "test", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);									// �����ڵ�����������Ϊ�����̵߳�������

	// �ڵ����κ� OpenGL �ĺ���֮ǰ����Ҫ��ʼ�� GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initalize GLAD" << std::endl;
		return -1;
	}

	// ��ʼ���ӿ�ά��
	glViewport(0, 0, 800, 600);

	// ���� GLFW  ÿ�����ڵ�����С��ʱ����Ҫ���� framebuffer_size_callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// ���� OpenGL ͨ�� ����ģʽ ����ͼԪ
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// ���û�Ĭ��ģʽ
	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// ����һ��������
	float vertices[] = {
		// λ��              // ��ɫ
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����
	};

	//// ����һ���ı���
	//float vertices[] = {
	//	0.5f, 0.5f, 0.0f,   // ���Ͻ�
	//	0.5f, -0.5f, 0.0f,  // ���½�
	//	-0.5f, -0.5f, 0.0f, // ���½�
	//	-0.5f, 0.5f, 0.0f   // ���Ͻ�
	//};

	//unsigned int indices[] = { // ע��������0��ʼ! 
	//	0, 1, 3, // ��һ��������
	//	1, 2, 3  // �ڶ���������
	//};

	// ����һ�� VAO ����
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// 0.����һ�����㻺�����
	// �Ѷ������ݿ�����������ڴ���
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//// ����һ�������������
	//unsigned int EBO;
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 1.���ö���λ������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// ��ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	//// test ��ȡӲ���ṩ�Ķ����������� һ�㶼�� 16 ������4�����Ķ������Կ���
	//int nrAttributes;
	//glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	//std::cout << "Maximum nr of vertex attributes supported:" << nrAttributes << std::endl;

	shader ourShader("shader.vs", "shader.fs");

	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(window)) {
		// ����ⲿ����
		processInput(window);

		// ��Ⱦָ��
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//// �����¼��ı���ɫ
		//float timeValue = glfwGetTime();
		//float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

		// 2.������ɫ���������
		// 3.��������
		//glUseProgram(shaderProgram);
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		ourShader.use();
		//ourShader.setFloat4("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
		ourShader.setFloat("pos", 0.0f);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// ��鲢�����¼�����������
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
	//glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}