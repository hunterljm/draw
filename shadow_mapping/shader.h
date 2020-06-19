#pragma once
#include <glad/glad.h> // 包含 glad 来获取所有的必须 OpenGL 头文件
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class shader
{
public:
	// 程序ID
	unsigned int ID;

	// 构造器读取并构建着色器
	// 顶点着色器、几何着色器、片段着色器
	shader(const GLchar* vertexPath, const GLchar* geometryPath, const GLchar* fragmentPath);
	// 顶点着色器、片段着色器
	shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	// 使用激活程序
	void use();

	// uniform 工具函数
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setFloat4(const std::string& name, float x, float y, float z, float w) const;
	void setFloat3(const std::string& name, float x, float y, float z) const;
	void setVec3(const std::string& name, glm::vec3 value) const;
	void setVec2(const std::string& name, glm::vec2 value) const;
	void setMat4(const std::string& name, glm::mat4 mat) const;
};

