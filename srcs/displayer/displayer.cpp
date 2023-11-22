/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   displayer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguyot <bguyot@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 16:31:09 by bguyot            #+#    #+#             */
/*   Updated: 2023/11/22 17:50:08 by bguyot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <displayer.hpp>

unsigned int	compileShader(const std::string &path, GLenum shader_type);
GLFWwindow	*createWindow();
unsigned int	setupProgram(void);
void	setupTriangles(float vertices[], unsigned int indices[], unsigned int nb_vertices, unsigned int nb_indices);
void keys(GLFWwindow *window, int key, int scancode, int action, int modes);

void	displayer(float vertices[], unsigned int indices[], unsigned int nb_vertices, unsigned int nb_indices)
{
	GLFWwindow*		window;
	unsigned int	shaderProgram;

	window = createWindow();
	shaderProgram = setupProgram();
	setupTriangles(vertices, indices, nb_vertices, nb_indices);

	glfwSetKeyCallback(window, keys);

	while (!glfwWindowShouldClose(window)) {
		glUniform1f(
			glGetUniformLocation(shaderProgram, "time"),
			(float)glfwGetTime()
		);

		glClearColor(0.2f, 0.3f ,0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	glDeleteProgram(shaderProgram);
}

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

GLFWwindow	*createWindow()
{
	GLFWwindow*		window;

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return nullptr;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		std::cerr << "Failed to open window" << std::endl;
		return nullptr;
	}

	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		std::cerr << "Failed to init glew" << std::endl;
		return nullptr;
	}
	return window;
}

unsigned int	setupProgram(void)
{
	unsigned int	fragment_shader;
	unsigned int	vertex_shader;
	unsigned int	shaderProgram;

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
	glUseProgram(shaderProgram);
	return (shaderProgram);
}

void	setupTriangles(float vertices[], unsigned int indices[], unsigned int nb_vertices, unsigned int nb_indices)
{
	unsigned int	VBO;
	unsigned int	VAO;
	unsigned int	EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, nb_vertices, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nb_indices, indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glBindVertexArray(VAO);
}

void keys(GLFWwindow *window, int key, int scancode, int action, int modes)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}
