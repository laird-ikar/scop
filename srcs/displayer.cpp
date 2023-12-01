/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   displayer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguyot <bguyot@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 16:31:09 by bguyot            #+#    #+#             */
/*   Updated: 2023/12/01 13:59:51 by bguyot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <displayer.hpp>

unsigned int	compileShader(const std::string &path, GLenum shader_type);
GLFWwindow		*createWindow();
unsigned int	setupProgram(void);
void			setupVertex(Parser &parser);
void			setupTexture(void);
void			keys(GLFWwindow *window, int key, int scancode, int action, int modes);

float	w = 1.2;
int		mode = 0;
int		do_time_flow = 1;
float	deltaX = 0;
float	deltaY = 0;
float	rotationZ = 0;

void	displayer(Parser &parser)
{
	GLFWwindow*		window;
	unsigned int	shaderProgram;
	float	time = 0;

	window = createWindow();
	shaderProgram = setupProgram();
	if (shaderProgram < 0 || !window)
		return ;
	setupVertex(parser);
	setupTexture();
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture"), 0);

	glfwSetKeyCallback(window, keys);
	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_CULL_FACE);
	while (!glfwWindowShouldClose(window)) {
		// Send data to the shaders
		if (do_time_flow)
			time += 0.01;
		glUniform1f(glGetUniformLocation(shaderProgram, "time"), time);
		glUniform1f(glGetUniformLocation(shaderProgram, "view_depth"), w);
		glUniform1i(glGetUniformLocation(shaderProgram, "mode"), mode);
		glUniform1f(glGetUniformLocation(shaderProgram, "deltaX"), deltaX);
		glUniform1f(glGetUniformLocation(shaderProgram, "deltaY"), deltaY);
		glUniform1f(glGetUniformLocation(shaderProgram, "rotationZ"), rotationZ);

		// Clear up the screan with a background color
		glClearColor(0.2f, 0.3f ,0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Draw the triangles (AND ONLY THE TRIANGLES)
		// glDrawArrays(GL_TRIANGLES, 0, parser.getNbVertices());
		glDrawElements(GL_TRIANGLES, parser.getNbIndices(), GL_UNSIGNED_INT, 0);

		// Show the frame
		glfwSwapBuffers(window);

		// Check if keys and stuff have been interacted with
		glfwPollEvents();
	}

	glfwTerminate();
	glDeleteProgram(shaderProgram);
}

void			setupTexture(void)
{
	const char* texturePath = "./resources/unicorn.bmp";
	int width, height;
	std::vector<unsigned char> imageData;
	if (!bmp_parser(texturePath, imageData, width, height))
		return ;
	GLuint textureID;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &imageData[0]);

	// Set texture parameters (optional)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
	window = glfwCreateWindow(1600, 1200, "OpenGL Window", nullptr, nullptr);
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
		std::cerr << "Failed to compile shader" << std::endl;
		glfwTerminate();
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

void	setupVertex(Parser &parser)
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
	glBufferData(GL_ARRAY_BUFFER, parser.getNbVertexData() * sizeof (double), parser.getVertexDataArray(), GL_STATIC_DRAW);

	// Specifies how the data is organised inside VBO
	glVertexAttribPointer(
		0,					// Index of the vertex attribute
		3,					// Specifies there is 3 components (X, Y, Z)
		GL_DOUBLE,			// Data is in floats
		GL_FALSE,			// No need to normalize
		sizeof (double) * NB_DATA_FEILD, // Offset between vertices (3 floats / vertex)
		(void*)0			// No offset at the begining
	);
	// Starts the vertices at the 0th element of the array
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1,					// Index of the vertex attribute
		3,					// Specifies there is 3 components (X, Y, Z)
		GL_DOUBLE,			// Data is in floats
		GL_FALSE,			// No need to normalize
		sizeof (double) * NB_DATA_FEILD, // Offset between vertices (3 floats / vertex)
		(void*) (sizeof (double) * 3)		// No offset at the begining
	);
	// Starts the vertices at the 0th element of the array
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(
		2,					// Index of the vertex attribute
		2,					// Specifies there is 3 components (X, Y, Z)
		GL_DOUBLE,			// Data is in floats
		GL_FALSE,			// No need to normalize
		sizeof (double) * NB_DATA_FEILD, // Offset between vertices (3 floats / vertex)
		(void*) (sizeof (double) * 9)		// No offset at the begining
	);
	// Starts the vertices at the 0th element of the array
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(
		3,					// Index of the vertex attribute
		3,					// Specifies there is 3 components (X, Y, Z)
		GL_DOUBLE,			// Data is in floats
		GL_FALSE,			// No need to normalize
		sizeof (double) * NB_DATA_FEILD, // Offset between vertices (3 floats / vertex)
		(void*) (sizeof (double) * 6)		// No offset at the begining
	);
	// Starts the vertices at the 0th element of the array
	glEnableVertexAttribArray(3);

	// Same as VBO but for Element Buffer Object
	unsigned int	EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, parser.getNbIndices() * sizeof (unsigned int), parser.getIndicesArray(), GL_STATIC_DRAW);

	// Unbinds the buffer and array bc no nead no more
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void keys(GLFWwindow *window, int key, int scancode, int action, int modes)
{
	int				fillModes[] = {GL_FILL, GL_LINE, GL_POINT};
	static int		fillModeIndex = 0;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	if ((key == GLFW_KEY_LEFT_BRACKET || key == GLFW_KEY_RIGHT_BRACKET)
		&& action == GLFW_PRESS)
	{
		if (modes & GLFW_MOD_SHIFT)
		{
			mode += (92 - key) + 4;
			mode %= 4;
		}
		else
		{
			fillModeIndex += (92 - key) + 3;
			fillModeIndex %= 3;
			glPolygonMode(GL_FRONT_AND_BACK, fillModes[fillModeIndex]);
		}
	}
	if ((key == '=' || key == '-')
			&& (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		if (key == '=')
			w *= 1.01;
		else
			w /= 1.01;
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		do_time_flow = !do_time_flow;
	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
		deltaY += 0.01;
	if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
		deltaY -= 0.01;
	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
		deltaX -= 0.01;
	if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
		deltaX += 0.01;
	if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT))
		rotationZ += 0.03;
	if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT))
		rotationZ -= 0.03;
}
