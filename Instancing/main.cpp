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

float quadVertices[] = {
	// λ��          // ��ɫ
	-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
	 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
	-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

	-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
	 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
	 0.05f,  0.05f,  0.0f, 1.0f, 1.0f
};

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
	camera = new CCamera(window, glm::vec3(0.0f, 0.0f, 50.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), screenWidth / 2, screenHeight / 2);
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

	// ���Ǵ�ʯͷλ�ô�С
	unsigned int amount = 20000;
	glm::mat4* modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime());
	float radius = 50.0f;
	float offset = 5.0f;
	for (unsigned int i = 0; i < amount; i++) {
		glm::mat4 model;

		// 1��λ�ƣ��ֲ��ڰ뾶Ϊ ��radius����Բ���ϣ�ƫ�Ƶķ�Χ�� [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle)*radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.05f;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle)*radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2�����ţ��� 0.05 �� 0.25 ֮������
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// 3����ת:����һ�����룩���ѡ�����ת�����������������ת
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4����Ӿ���������
		modelMatrices[i] = model;
	}

	Model planetModel("resources/planet/planet.obj");
	Model rockModel("resources/rock/rock.obj");

	shader planetShader("resources/planet.vs", "resources/planet.fs");
	shader rockShader("resources/rock.vs", "resources/rock.fs");

	// ���㻺�����
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	for (unsigned int i = 0; i < rockModel.meshes.size(); i++) {
		unsigned int VAO = rockModel.meshes[i].VAO;
		glBindVertexArray(VAO);
		GLsizei vec4Size = sizeof(glm::vec4);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		camera->Update();
		
		// ��ˢ��Ȼ��塢����ɫ
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// ��������
		planetShader.use();
		// �۲����
		glUniformMatrix4fv(glGetUniformLocation(planetShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
		// ͶӰ����
		glUniformMatrix4fv(glGetUniformLocation(planetShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(
			camera->GetPerspectiveMatirx((float)screenWidth / (float)screenHeight, 0.1f, 1000.0f)
		));
		// ģ�;���
		glm::mat4 model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(planetShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		planetModel.Draw(planetShader);

		// �������Ǵ�
		rockShader.use();
		// �۲����
		glUniformMatrix4fv(glGetUniformLocation(rockShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
		// ͶӰ����
		glUniformMatrix4fv(glGetUniformLocation(rockShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(
			camera->GetPerspectiveMatirx((float)screenWidth / (float)screenHeight, 0.1f, 1000.0f)
		));
		// ģ�;���
		//float roudSpeed = 200.0f;
		//float r = 7.0f;
		//lightPos = glm::vec3(cos(glm::radians(roudSpeed * glfwGetTime()))*r, 10.0f, sin(glm::radians(roudSpeed * glfwGetTime()))*r);
		//for (unsigned int i = 0; i < amount; i++) {
		//	glUniformMatrix4fv(glGetUniformLocation(rockShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrices[i]));
		//	rockModel.Draw(rockShader);
		//}
		for (unsigned int i = 0; i < rockModel.meshes.size(); i++) {
			glBindVertexArray(rockModel.meshes[i].VAO);
			glDrawElementsInstanced(GL_TRIANGLES, rockModel.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	delete camera;
	glfwTerminate();
	return 0;
}