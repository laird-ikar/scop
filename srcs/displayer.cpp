/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   displayer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguyot <bguyot@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 16:31:09 by bguyot            #+#    #+#             */
/*   Updated: 2023/11/23 14:52:56 by bguyot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <displayer.hpp>

unsigned int	compileShader(const std::string &path, GLenum shader_type);
GLFWwindow	*createWindow();
unsigned int	setupProgram(void);
void	setupTriangles(float vertices[], unsigned int indices[], size_t nb_vertices, size_t nb_indices);
void keys(GLFWwindow *window, int key, int scancode, int action, int modes);

void	displayer(float vertices[], unsigned int indices[], size_t nb_vertices, size_t nb_indices)
{
	GLFWwindow*		window;
	unsigned int	shaderProgram;

	window = createWindow();
	shaderProgram = setupProgram();
	setupTriangles(vertices, indices, nb_vertices, nb_indices);

	glfwSetKeyCallback(window, keys);

	while (!glfwWindowShouldClose(window)) {
		// Send data to the shaders
		glUniform1f(
			glGetUniformLocation(shaderProgram, "time"),
			(float)glfwGetTime()
		);

		// Clear up the screan with a background color
		glClearColor(0.2f, 0.3f ,0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw the triangles (AND ONLY THE TRIANGLES)
		glDrawArrays(GL_TRIANGLES, 0, nb_vertices);
		glDrawElements(GL_TRIANGLES, nb_indices, GL_UNSIGNED_INT, 0);

		// Show the frame
		glfwSwapBuffers(window);

		// Check if keys and stuff have been interacted with
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
	unsigned int	geometry_shader;
	unsigned int	shaderProgram;

	fragment_shader = compileShader("srcs/shaders/rainbow.frag", GL_FRAGMENT_SHADER);
	vertex_shader = compileShader("srcs/shaders/rainbow.vert", GL_VERTEX_SHADER);
	// geometry_shader = compileShader("srcs/shaders/test.geom", GL_GEOMETRY_SHADER);
	if (!vertex_shader || ! fragment_shader /* || !geometry_shader*/)
	{
		glfwTerminate();
		std::cerr << "Failed to compile shader" << std::endl;
		return -1;
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, fragment_shader);
	glAttachShader(shaderProgram, vertex_shader);
	// glAttachShader(shaderProgram, geometry_shader);
	glLinkProgram(shaderProgram);
	glDeleteShader(fragment_shader);
	glDeleteShader(vertex_shader);
	// glDeleteShader(geometry_shader);
	glUseProgram(shaderProgram);
	return (shaderProgram);
}

void	setupTriangles(float vertices[], unsigned int indices[], size_t nb_vertices, size_t nb_indices)
{
	// Generate and bind Vertex Array Object
	unsigned int	VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); // Now the VAO interactions will be with *this* VAO

	// Generate and bind Vertex Buffer Object
	unsigned int	VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Init buffer with size and data from vertices
	glBufferData(GL_ARRAY_BUFFER, nb_vertices * sizeof (float), vertices, GL_STATIC_DRAW);

	// Specifies how the data is organised inside VBO
	glVertexAttribPointer(
		0,					// Index of the vertex attribute
		3,					// Specifies there is 3 components (X, Y, Z)
		GL_FLOAT,			// Data is in floats
		GL_FALSE,			// No need to normalize
		3 * sizeof (float),	// Offset between vertices (3 floats / vertex)
		(void*)0			// No additionnal data
	);
	// Starts the vertices at the 0th element of the array
	glEnableVertexAttribArray(0);

	// Same as VBO but for Element Buffer Object
	unsigned int	EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nb_indices * sizeof (unsigned int), indices, GL_STATIC_DRAW);

	// Unbinds the buffer and array bc no nead no more
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void keys(GLFWwindow *window, int key, int scancode, int action, int modes)
{
	int			fillModes[] = {GL_FILL, GL_LINE, GL_POINT};
	static int	fillModeIndex = 0;


	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	if ((key == GLFW_KEY_LEFT_BRACKET || key == GLFW_KEY_RIGHT_BRACKET)
		&& action == GLFW_PRESS)
	{
		fillModeIndex += (92 - key) + 3;
		fillModeIndex %= 3;
		glPolygonMode(GL_FRONT_AND_BACK, fillModes[fillModeIndex]);
	}
}
