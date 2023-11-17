/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguyot <bguyot@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 16:03:17 by bguyot            #+#    #+#             */
/*   Updated: 2023/11/17 11:05:13 by bguyot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

unsigned int	compileShader(const std::string &path, GLenum shader_type)
{
	std::ifstream		file(path);
	std::stringstream	buffer;
	unsigned int		shader;
	GLint success;

	if (!file.is_open())
		return (0);
	buffer << file.rdbuf();
	shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &buffer.str().c_str(), nullptr);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
		std::cerr << "Shader compilation error in " << path << ":\n" << infoLog << std::endl;
		return 0;
	}

	return shader;
}

int main(void)
{
	unsigned int	vertex_shader;
	unsigned int	fragment_shader;
	unsigned int	shaderProgram;
	GLFWwindow*		window;

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		return -1;
	}

	fragment_shader = compileShader("../shaders/red.gls1", GL_COMPUTE_SHADER);
	if (!vertex_shader)
	{
		glfwTerminate();
		return -1;
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, fragment_shader);
	glLinkProgram(shaderProgram);
	glDeleteShader(fragment_shader);
	glUseProgram(shaderProgram);


	while (!glfwWindowShouldClose(window)) {
		glUniform1f(
			glGetUniformLocation(shaderProgram, "time"),
			(float)glfwGetTime()
		);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shaderProgram);
}
