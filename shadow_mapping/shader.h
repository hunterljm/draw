#pragma once
#include <glad/glad.h> // ���� glad ����ȡ���еı��� OpenGL ͷ�ļ�
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class shader
{
public:
	// ����ID
	unsigned int ID;

	// ��������ȡ��������ɫ��
	// ������ɫ����������ɫ����Ƭ����ɫ��
	shader(const GLchar* vertexPath, const GLchar* geometryPath, const GLchar* fragmentPath);
	// ������ɫ����Ƭ����ɫ��
	shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	// ʹ�ü������
	void use();

	// uniform ���ߺ���
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setFloat4(const std::string& name, float x, float y, float z, float w) const;
	void setFloat3(const std::string& name, float x, float y, float z) const;
	void setVec3(const std::string& name, glm::vec3 value) const;
	void setVec2(const std::string& name, glm::vec2 value) const;
	void setMat4(const std::string& name, glm::mat4 mat) const;
};

