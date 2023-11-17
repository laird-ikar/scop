/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguyot <bguyot@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 16:03:17 by bguyot            #+#    #+#             */
/*   Updated: 2023/11/17 18:18:13 by bguyot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>

unsigned int	compileShader(const std::string &path, GLenum shader_type)
{
	std::ifstream		file(path);
	std::stringstream	buffer;
	unsigned int		shader;
	char*				source;
	GLint success;

	if (!file.is_open())
	{
		std::cerr << "Cannot open file" << std::endl;
		return (0);
	}
	buffer << file.rdbuf();
	shader = glCreateShader(shader_type);
	source = strdup(buffer.str().c_str());
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);
	free(source);

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
	unsigned int	fragment_shader;
	unsigned int	vertex_shader;
	unsigned int	shaderProgram;
	GLFWwindow*		window;


	float vertices[] = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

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
		std::cerr << "Failed to open window" << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		std::cerr << "Failed to init glew" << std::endl;
		return -1;
	}

	fragment_shader = compileShader("srcs/shaders/rainbow.frag", GL_FRAGMENT_SHADER);
	if (!fragment_shader)
	{
		glfwTerminate();
		std::cerr << "Failed to compile shader" << std::endl;
		return -1;
	}
	vertex_shader = compileShader("srcs/shaders/rainbow.vert", GL_VERTEX_SHADER);
	if (!vertex_shader)
	{
		glfwTerminate();
		std::cerr << "Failed to compile shader" << std::endl;
		return -1;
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, fragment_shader);
	glAttachShader(shaderProgram, vertex_shader);
	glLinkProgram(shaderProgram);
	glDeleteShader(fragment_shader);
	glDeleteShader(vertex_shader);

	unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

	while (!glfwWindowShouldClose(window)) {
		glUniform1f(
			glGetUniformLocation(shaderProgram, "time"),
			(float)glfwGetTime()
		);

		glClearColor(0.2f, 0.3f ,0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		  glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shaderProgram);
}
