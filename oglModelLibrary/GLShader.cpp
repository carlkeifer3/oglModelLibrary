#include "GLShader.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>


// code to check for shader compiler errors
void shaderCompilerCheck(GLuint ID)
{
	GLint comp;
	glGetShaderiv (ID, GL_COMPILE_STATUS, &comp);

	if (comp == GL_FALSE)
	{
		std::cout << "Shader Compilation FAILED!!" << std::endl;
		GLchar messages[256];
		glGetShaderInfoLog (ID, sizeof(messages), 0, &messages[0]);
		std::cout << messages;
	}else{
		std::cout << "Shader Compilation SUCCESS!!" << std::endl;
		GLchar messages[256];
		glGetShaderInfoLog (ID, sizeof(messages), 0, &messages[0]);
		std::cout << messages;
	}
};//End void shaderCompilerCheck(GLuint ID)

// Code to check for shader link errors
void shaderLinkCheck(GLuint ID)
{
	GLint linkStatus, validateStatus;
	glGetProgramiv (ID, GL_LINK_STATUS, &linkStatus);

	if (linkStatus == GL_FALSE)
	{
		std::cout << "Shader Linking FAILED!!" << std::endl;
		GLchar messages[256];
		glGetProgramInfoLog (ID, sizeof(messages), 0, &messages[0]);
		std::cout << messages;
	}else{
		std::cout << "Shader Linking SUCCESS!!" << std::endl;
		GLchar messages[256];
		glGetProgramInfoLog (ID, sizeof(messages), 0, &messages[0]);
		std::cout << messages;
	}

	glValidateProgram (ID);
	glGetProgramiv (ID, GL_VALIDATE_STATUS, &validateStatus);

	if (validateStatus == GL_FALSE)
	{
		std::cout << "Shader Validation FAILED!!!" << std::endl;
		GLchar messages[256];
		glGetProgramInfoLog (ID, sizeof(messages), 0, &messages[0]);
		std::cout << messages;
	}else{
		std::cout << "Shader Validation SUCCESS!!!" << std::endl;
		GLchar messages[256];
		glGetProgramInfoLog (ID, sizeof(messages), 0, &messages[0]);
		std::cout << messages;
	}
};//End void shaderLinkCheck(GLuint ID)

std::string readFile(const char *filePath)
{
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if(!fileStream.is_open())
	{
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}

	std::string line = "";
	while(!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

GLuint LoadShader(const char *vertex_path, const char *fragment_path)
{
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Read Shaders
	std::string vertShaderStr = readFile(vertex_path);
	std::string fragShaderStr = readFile(fragment_path);
	const char *vertShaderSrc = vertShaderStr.c_str();
	const char *fragShaderSrc = fragShaderStr.c_str();

	//Compile/check vertex shader
	std::cout << "Compiling vertex shader." << std::endl;
	glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
	shaderCompilerCheck(vertShader);

	//Compile/check fragment shader
	std::cout << "Compiling fragment shader." << std::endl;
	glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
	shaderCompilerCheck(fragShader);

	// linking program
	std::cout << "Linking shaders to program." << std::endl;
	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);
	
	shaderLinkCheck(program);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return program;
}
