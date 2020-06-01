#pragma once
#include <glad/glad.h> // 包含 glad 来获取所有的必须 OpenGL 头文件
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
	shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	// 使用激活程序
	void use();

	// uniform 工具函数
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setFloat4(const std::string& name, float x, float y, float z, float w) const;
};

