#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "stb_image.h"
#include "camera.h"

// 屏幕大小
int screenWidth = 800;
int screenHeight = 600;

// 看到第二个纹理的程度
float see = 0.2f;

// 摄像机
CCamera* camera = nullptr;

// 窗口大小调整回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// 检测按键输入
void processInput(GLFWwindow* window, shader* ptr_shader)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		see += 0.0001f;
		if (see >= 1.0f) {
			see = 1.0f;
		}
		ptr_shader->setFloat("see", see);
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		see -= 0.0001f;
		if (see <= 0.0f) {
			see = 0.0f;
		}
		ptr_shader->setFloat("see", see);
	}
	camera->processInput();
}

// 检测鼠标输入
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera->mouse_callback(xpos, ypos);
}

// 滚轮回调函数
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera->scroll_callback(xoffset, yoffset);
}

int main()
{
	// GLFW 用于为 OpenGl 创建上下文
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);					// OpenGL 主版本 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);					// OpenGL 次版本 3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// OpenGL 使用的是核心模式 Core_profile

	// 创建一个窗口对象
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "test", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);									// 将窗口的上下文设置为该主线程的上下文

	// 告诉 GLFW  每当窗口调整大小的时候需要调用 framebuffer_size_callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// 创建摄像机对象
	camera = new CCamera(window, glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), screenWidth/2, screenHeight/2);

	// 捕捉光标并隐藏光标
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// 在调用任何 OpenGL 的函数之前都需要初始化 GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initalize GLAD" << std::endl;
		return -1;
	}

	// 初始化视口维度
	glViewport(0, 0, screenWidth, screenHeight);

	// 设置 OpenGL 通过 线性模式 绘制图元
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// 设置回默认模式
	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//// 绘制一个三角形
	//float vertices[] = {
	//	// 位置              // 颜色
	//	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
	//	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
	//	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
	//};

	//// 绘制一个四边形
	//float vertices[] = {
	//	//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
	//		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // 右上
	//		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // 右下
	//		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
	//		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // 左上
	//};

	// 绘制六个四边形
	float vertices[] = {
		//     ---- 位置 ----       - 纹理坐标 -
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	//unsigned int indices[] = { // 注意索引从0开始! 
	//	0, 1, 3, // 第一个三角形
	//	1, 2, 3  // 第二个三角形
	//};

	// 定义 10 个立方体位置
	glm::vec3 cubePositions[] = {
	  glm::vec3(0.0f,  0.0f,  0.0f),
	  glm::vec3(2.0f,  5.0f, -15.0f),
	  glm::vec3(-1.5f, -2.2f, -2.5f),
	  glm::vec3(-3.8f, -2.0f, -12.3f),
	  glm::vec3(2.4f, -0.4f, -3.5f),
	  glm::vec3(-1.7f,  3.0f, -7.5f),
	  glm::vec3(1.3f, -2.0f, -2.5f),
	  glm::vec3(1.5f,  2.0f, -2.5f),
	  glm::vec3(1.5f,  0.2f, -1.5f),
	  glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// 创建一个 VAO 对象
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// 0.定义一个顶点缓冲对象
	// 把顶点数据拷贝到缓冲的内存中
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//// 定义一个索引缓存对象
	//unsigned int EBO;
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 1.设置顶点位置属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//// 颜色属性
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	//glEnableVertexAttribArray(1);
	// 纹理属性
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//// test 获取硬件提供的顶点属性上限 一般都是 16 个包含4分量的顶点属性可用
	//int nrAttributes;
	//glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	//std::cout << "Maximum nr of vertex attributes supported:" << nrAttributes << std::endl;

	// 生成纹理
	unsigned int texture1;
	glGenTextures(1, &texture1);
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// 为当前绑定的纹理设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 载入图片
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	// 加载并生成纹理
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	unsigned int texture2;
	glGenTextures(1, &texture2);
	//glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// 为当前绑定的纹理设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 载入图片
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	// 加载并生成纹理
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture1" << std::endl;
	}
	stbi_image_free(data);

	shader ourShader("shader.vs", "shader.fs");
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);

	// 开启深度测试
	glEnable(GL_DEPTH_TEST);

	// 渲染循环
	while (!glfwWindowShouldClose(window)) {
		camera->Update();

		// 检测外部输入
		processInput(window, &ourShader);

		// 渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 清除深度缓冲
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 绘制物体
		ourShader.setFloat("see", see);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAO);

		// 观察矩阵
		unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "view");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

		// 三维物体看起来有远近的区别
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(
			camera->GetPerspectiveMatirx((float)screenWidth/(float)screenHeight, 0.1f, 100.0f)));

		// 模型如何运动
		for (unsigned int i = 0; i < 10; i++) {
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			int j = 0;
			if (i % 3 == 0) {
				j = 1;
			}
			else {
				j = 0;
			}
			float angle = 20.0f * j;
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// 检查并调用事件，交换缓冲
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}