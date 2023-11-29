/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguyot <bguyot@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 11:10:39 by bguyot            #+#    #+#             */
/*   Updated: 2023/11/29 10:28:37 by bguyot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Parser.class.hpp>

Parser::Parser(void)
{
	// Pointing towards us
	// this->vertices = std::vector<float>({
	// 	0.0f, 0.0f, 0.5f,	// S
	// 	0.0f, 0.5f, -0.5f,	// A
	// 	-0.5f, -0.5f, -0.5f,// B
	// 	0.5f, -0.5f, -0.5f,	// C
	// });

	// Pointing to the top
	this->_vertices = std::vector<double>({
		0.0, 0.5, 0.0, 1.0, 1.0, 1.0,	// S
		0.0, -0.5, -0.5, 1.0, 0.0, 1.0,	// A
		-0.5, -0.5, 0.5, 0.0, 1.0, 1.0,	// B
		0.5, -0.5, 0.5, 1.0, 0.0, 1.0,	// C
	});
	this->_indices = std::vector<unsigned int>({
		0, 1, 2,			// SBA
		0, 2, 3,			// SBC
		0, 3, 1,			// SCA
		1, 3, 2				// ACB
	});
}

Parser::Parser(const std::string &path)
{
	std::ifstream	file(path);
	std::string		line;

	if (!file.is_open())
	{
		std::cerr << "Cannot open file" << std::endl;
		Parser();
		return ;
	}

	while (std::getline(file, line))
	{
		if (line[0] == '#')
			continue;
		else if (line.rfind("v ", 0) == 0)
		{
			size_t pos = line.find(" ");
			line.erase(0, pos + 1);
			std::string token;
			while ((pos = line.find(" ")) != std::string::npos) {
				token = line.substr(0, pos);
				this->_vertices.push_back(std::stod(token));
				line.erase(0, pos + 1);
			}
			this->_vertices.push_back(std::stod(line));
		}
		// if (/*vn*/)
		// 	vertexNormal;
		// if (/*vt*/)
		// 	textureCoordonate;
		// if (/*vp*/)
		// 	paramSpaceVertex;
		else if (line.rfind("f ", 0) == 0)
		{
			// Create random color for face
			double r = (double) rand() / RAND_MAX;
			double g = (double) rand() / RAND_MAX;
			double b = (double) rand() / RAND_MAX;

			// Find all vertices used by the face
			std::vector<unsigned int>	position_indices;
			std::vector<unsigned int>	data_indices;
			size_t pos = line.find(" ");
			line.erase(0, pos + 1);
			std::string token;
			while ((pos = line.find(" ")) != std::string::npos) {
				token = line.substr(0, pos);
				position_indices.push_back(std::stoi(token) - 1);
				line.erase(0, pos + 1);
			}
			position_indices.push_back(std::stoi(line) - 1);

			// Add the vertex position and the face color into vertex_data
			for (unsigned int i : position_indices)
			{
				// X, Y, Z
				this->_vertex_data.push_back(this->_vertices[3 * i]);
				this->_vertex_data.push_back(this->_vertices[3 * i + 1]);
				this->_vertex_data.push_back(this->_vertices[3 * i + 2]);

				// R, G, B
				this->_vertex_data.push_back(r);
				this->_vertex_data.push_back(g);
				this->_vertex_data.push_back(b);

				// Cylinder U, V
				this->_vertex_data.push_back(
					std::atan2(this->_vertices[3 * i], this->_vertices[3 * i + 2])
				); // Angle of the vertex
				this->_vertex_data.push_back(
					this->_vertices[3 * i + 1]
				); // Height of the vertex

				// Add the index of the vertex_data to data_indices
				data_indices.push_back(this->_vertex_data.size() / NB_DATA_FEILD - 1);
			}

			// Add the indices of the vertex_data to indices (trianulized)
			for (int i = 0; i < data_indices.size() - 2; i++)
			{
				this->_indices.push_back(data_indices[0]);
				this->_indices.push_back(data_indices[i + 1]);
				this->_indices.push_back(data_indices[i + 2]);
			}
		}
		// if (/*l*/)
		// 	line;
		// if (/*mtllib*/)
		// 	loadMTL;
		// if (/*usemtl*/)
		// 	mtl;
	}
	// Normalize positions of the vertex data to center it all
	double x_min, x_max, y_min, y_max, z_min, z_max;
	x_min = x_max = this->_vertex_data[0];
	y_min = y_max = this->_vertex_data[1];
	z_min = z_max = this->_vertex_data[2];
	for (size_t i = 0; i < this->_vertex_data.size(); i += NB_DATA_FEILD)
	{
		if (this->_vertex_data[i] < x_min)
			x_min = this->_vertex_data[i];
		if (this->_vertex_data[i] > x_max)
			x_max = this->_vertex_data[i];
		if (this->_vertex_data[i + 1] < y_min)
			y_min = this->_vertex_data[i + 1];
		if (this->_vertex_data[i + 1] > y_max)
			y_max = this->_vertex_data[i + 1];
		if (this->_vertex_data[i + 2] < z_min)
			z_min = this->_vertex_data[i + 2];
		if (this->_vertex_data[i + 2] > z_max)
			z_max = this->_vertex_data[i + 2];
	}
	double x_center = (x_min + x_max) / 2;
	double y_center = (y_min + y_max) / 2;
	double z_center = (z_min + z_max) / 2;
	double ratio = std::max(x_max - x_min, std::max(y_max - y_min, z_max - z_min));
	ratio /= 2;
	for (size_t i = 0; i < this->_vertex_data.size(); i += NB_DATA_FEILD)
	{
		this->_vertex_data[i] = (this->_vertex_data[i] - x_center) / ratio * 0.9;
		this->_vertex_data[i + 1] = (this->_vertex_data[i + 1] - y_center) / ratio * 0.9;
		this->_vertex_data[i + 2] = (this->_vertex_data[i + 2] - z_center) / ratio * 0.9;
	}
}

Parser::Parser(const Parser &other)
{
	*this = other;
}

Parser::~Parser(void)
{}

Parser			&Parser::operator=(const Parser &rhs)
{
	if (this != &rhs)
	{
		this->_vertices = std::vector<double>(rhs._vertices);
		this->_indices = std::vector<unsigned int>(rhs._indices);
		this->_vertex_data = std::vector<double>(rhs._vertex_data);
	}
	return *this;
}

double			*Parser::getVerticesArray(void)
{
	return (&this->_vertices[0]);
}

unsigned int	*Parser::getIndicesArray(void)
{
	return (&this->_indices[0]);
}

size_t			Parser::getNbVertices(void)
{
	return (this->_vertices.size());
}

size_t			Parser::getNbIndices(void)
{
	return (this->_indices.size());
}

double			*Parser::getVertexDataArray(void)
{
	return (&this->_vertex_data[0]);
}

size_t			Parser::getNbVertexData(void)
{
	return (this->_vertex_data.size());
}
