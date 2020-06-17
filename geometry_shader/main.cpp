#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // gml::translate, glm::rotate, glm::scale, glm::perspective

#include "model.h"
#include "shader.h"
#include "camera.h"
#include "stb_image.h"

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

//// ����λ�ã������� z ƽ����
//float points[] = {
//	-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // ����
//	 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // ����
//	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // ����
//	-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // ����
//};

int main()
{
	// GLFW ��ʼ��
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// ��������
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "model", nullptr, nullptr);
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
	camera = new CCamera(window, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), screenWidth / 2, screenHeight / 2);
	if (camera == nullptr) {
		glfwTerminate();
		return -1;
	}

	// ��ʼ�� GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initalize GLAD." << std::endl;
		return -1;
	}

	stbi_set_flip_vertically_on_load(true);
	// glViewport(0, 0, screenWidth, screenHeight);

	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);

	//// ���ƶ���
	//unsigned int VAO, VBO;
	//glGenVertexArrays(1, &VAO);
	//glBindVertexArray(VAO);
	//glGenBuffers(1, &VBO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

	shader sampShader("resources/model.vs", "resources/model.gs", "resources/model.fs");
	shader normalShader("resources/model1.vs", "resources/model1.gs", "resources/model1.fs");
	Model tModel("resources/nanosuit/nanosuit.obj");
	//shader shader("resources/shader.vs", "resources/shader.gs", "resources/shader.fs");

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		camera->Update();
		
		// ��ˢ��Ȼ��塢����ɫ
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//// ���Ƶ�
		//shader.use();
		//// ���ù۲����
		//glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
		//// ����ͶӰ����
		//glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(
		//	camera->GetPerspectiveMatirx((float)screenWidth / (float)screenHeight, 0.1f, 100.0f)
		//));
		//glBindVertexArray(VAO);
		//glDrawArrays(GL_POINTS, 0, 4);
		//glBindVertexArray(0);


		// ��Ⱦģ��
		sampShader.use();
		//sampShader.setVec3("viewPos", camera->Position());
		sampShader.setFloat("time", glfwGetTime());
		// �۲����
		glUniformMatrix4fv(glGetUniformLocation(sampShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
		// ͶӰ����
		glUniformMatrix4fv(glGetUniformLocation(sampShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(
			camera->GetPerspectiveMatirx((float)screenWidth / (float)screenHeight, 0.1f, 100.0f)
		));
		// ģ��ת������
		glm::mat4 model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f));
		//model = glm::scale(model, glm::vec3(1.0f));
		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(sampShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		tModel.Draw(sampShader);
		normalShader.use();
		glUniformMatrix4fv(glGetUniformLocation(normalShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
		// ͶӰ����
		glUniformMatrix4fv(glGetUniformLocation(normalShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(
			camera->GetPerspectiveMatirx((float)screenWidth / (float)screenHeight, 0.1f, 100.0f)
		));
		// ģ��ת������
		model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f));
		//model = glm::scale(model, glm::vec3(1.0f));
		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(normalShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		tModel.Draw(normalShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	delete camera;
	glfwTerminate();
	return 0;
}