#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // gml::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "shader.h"
#include "camera.h"

// 屏幕大小
const int screenWidth = 800;
const int screenHeight = 600;

// 摄像机
CCamera* camera = nullptr;

// 窗口调整回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// 鼠标检测回调
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera->mouse_callback(xpos, ypos);
}

// 滚轮检测回调
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera->scroll_callback(xoffset, yoffset);
}

// 检测按键输入
void processInput(GLFWwindow* window) 
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	camera->processInput();
}

int main()
{
	// GLFW 初始化
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建窗口
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

	// 创建摄像机
	camera = new CCamera(window, glm::vec3(0.0f, 1.0f, 3.0f), glm::vec3(0.0f, 1.0f, -1.0f), glm::vec3(0.0f, 2.0f, 0.0f), screenWidth / 2, screenHeight / 2);
	if (camera == nullptr) {
		glfwTerminate();
		return -1;
	}

	// 初始化 GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initalize GLAD." << std::endl;
		return -1;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	// 绘制一个地图板块六面体， 总计 8 个点
	float vertices[] = {
		// --位置--				--颜色--
		 2.5f,  0.1f, 0.0f, 0.2f, 0.4f, 0.3f,  // 正面 top right		0
		 2.5f, 0.0f, 0.0f, 0.2f, 0.2f, 0.3f, // bottom right	1
		-2.5f, 0.0f, 0.0f, 0.2f, 0.2f, 0.3f, // bottom left		2
		-2.5f,  0.1f, 0.0f, 0.2f, 0.4f, 0.3f,  // top left		3	

		2.5f,  0.1f, -5.5f, 0.2f, 0.4f, 0.3f, // 背面 top right		4
		 2.5f, 0.0f,-5.5f, 0.2f, 0.2f, 0.3f, // bottom right	5
		-2.5f, 0.0f,-5.5f, 0.2f, 0.2f, 0.3f, // bottom left		6
		-2.5f,  0.1f,-5.5f, 0.2f, 0.4f, 0.3f, // top left		7
	};

	// 绘制索引 6 个面，每个面 2 个三角形，总计 12 个三角形
	unsigned int indices[] = {
		0, 1, 3,	// 左侧面
		1, 2, 3,
		
		4, 5, 6,	// 右侧面
		6, 7, 4,	

		6, 7, 2,	// 正面 
		7, 3, 2,

		2, 1, 5,	// 上面 
		5, 6, 2,

		1, 5, 4,	// 背面 
		4, 0, 1,

		0, 4, 7,	// 下面
		7, 3, 0
	};

	// 地标所在位置
	glm::vec3 cubePositions[] = {
	  glm::vec3(2.5f,  0.11f,  0.0f),
	  glm::vec3(-2.5f,  0.11f,  0.0f),
	  glm::vec3(2.5f,  0.11f,  -5.5f),
	  glm::vec3(-2.5f,  0.11f,  -5.5f)
	};

	// 创建一个 VAO、VBO、EBO
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 设置定位位置指针、颜色属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// 地标所在位置
	float item_pos[] = {
		2.5f,	0.11f,	0.0f,	2.0f, 2.0f, 1.0f,
		-2.5f,  0.11f,  0.0f,	2.0f, 2.0f, 1.0f,
		2.5f,	0.11f,  -5.5f,	2.0f, 2.0f, 1.0f,
		-2.5f,  0.11f,  -5.5f,	2.0f, 2.0f, 1.0f
	};
	unsigned int VAO_POS, VBO_POS;
	glGenVertexArrays(1, &VAO_POS);
	glGenBuffers(1, &VBO_POS);
	glBindVertexArray(VAO_POS);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_POS);
	glBufferData(GL_ARRAY_BUFFER, sizeof(item_pos), item_pos, GL_STATIC_DRAW);

	// 设置定位位置指针、颜色属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// 开启深度测试
	glEnable(GL_DEPTH_TEST);

	// 创建着色器
	shader ourShader("resources/shader.vs", "resources/shader.fs");
	ourShader.use();

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		camera->Update();

		// 重刷深度缓冲、背景色
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 绘制物体
		glBindVertexArray(VAO);

		// 观察矩阵
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
		// 三维物体看起来有远近的区别
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(
			camera->GetPerspectiveMatirx((float)screenWidth / (float)screenHeight, 0.1f, 100.0f)
		));
		// 模型转换矩阵
		glm::mat4 model;
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		int size = sizeof(cubePositions)/sizeof(glm::vec3);
		for (int i = 0; i < size; i++) {
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			model = glm::scale(model, glm::vec3(0.01f, 0.1f, 0.01f));
			model = glm::rotate(model, glm::radians(180.f), glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}

		glBindVertexArray(VAO_POS);
		glm::mat4 model_item;
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model_item));
		glDrawArrays(GL_LINES, 0, size);

		// 检查并调用事件，交换缓冲
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	delete camera;
	glfwTerminate();
	return 0;
}