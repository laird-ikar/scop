/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguyot <bguyot@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 11:10:39 by bguyot            #+#    #+#             */
/*   Updated: 2023/12/01 15:51:05 by bguyot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Parser.class.hpp>

std::vector<std::string> split(std::string s, std::string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr (pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		if (token.length() > 0)
			res.push_back (token);
	}

	res.push_back (s.substr (pos_start));
	return res;
}

Parser::Parser(void):
	_vertices({
		Vector3D({0.0, 0.5, 0.0,}),	// S
		Vector3D({0.0, -0.5, -0.5,}),	// A
		Vector3D({-0.5, -0.5, 0.5,}),	// B
		Vector3D({0.5, -0.5, 0.5,}),	// C
	}),
	_faces({
		Face({this->_vertices[0], this->_vertices[1], this->_vertices[2]}),
		Face({this->_vertices[0], this->_vertices[2], this->_vertices[3]}),
		Face({this->_vertices[0], this->_vertices[3], this->_vertices[1]}),
		Face({this->_vertices[1], this->_vertices[2], this->_vertices[3]}),
	})
{
	this->_normalizeVerticesPositons();
	this->_calculateNormals();
	this->_calculateUVs();
	this->_generateFaceColors();
	this->_generateVertexDataAndIndicies();
}

Parser::Parser(const std::string &path)
{
	std::ifstream	file(path);
	std::string		line;

	if (!file.is_open())
	{
		std::cerr << "Cannot open file" << std::endl;
		*this = Parser();
		return ;
	}
	while (std::getline(file, line))
	{
		try {
			if (line.rfind("v ", 0) == 0)
				this->_parseVertex(line);
			else if (line.rfind("f ", 0) == 0)
				this->_parseFace(line);
		}
		catch (std::exception &e) {
			std::cerr << "Error while parsing file: " << e.what() << "" << std::endl;
			*this = Parser();
			return ;
		}
	}
	file.close();
	this->_normalizeVerticesPositons();
	this->_generateFaceColors();
	this->_calculateNormals();
	this->_calculateUVs();
	this->_generateVertexDataAndIndicies();
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
		this->_vertices = rhs._vertices;
		this->_faces = rhs._faces;
		this->_vertex_data = rhs._vertex_data;
		this->_indices = rhs._indices;
	}
	return *this;
}

void	Parser::_parseFace(std::string &line)
{
	Face						face;
	std::vector<std::string>	token_list;
	std::string					token;
	unsigned int 				index;

	token_list = split(line, " ");
	for (int i = 1; i < token_list.size(); i++)
	{
		if (token_list[i].length() == 0)
			continue ;
		token = token_list[i].substr(0, token_list[i].find("/"));
		index = std::stoi(token) - 1;
		if (index >= this->_vertices.size())
			throw std::runtime_error("Invalid vertex index");
		face.vertices.push_back(this->_vertices[index]);
	}
	this->_faces.push_back(face);
}

void	Parser::_parseVertex(std::string &line)
{
	Vector3D					vertex;
	std::vector<std::string>	token_list;

	token_list = split(line, " ");
	vertex.x = std::stod(token_list[1]);
	vertex.y = std::stod(token_list[2]);
	vertex.z = std::stod(token_list[3]);
	this->_vertices.push_back(vertex);
}

void	Parser::_normalizeVerticesPositons(void)
{
	Vector3D	middle;
	Vector3D	max;
	Vector3D	min;
	double			diameter;

	// Calculate middle, max and min
	for (size_t i = 0; i < this->_vertices.size(); i++)
	{
		middle.x += this->_vertices[i].x;
		middle.y += this->_vertices[i].y;
		middle.z += this->_vertices[i].z;
		if (this->_vertices[i].x > max.x)
			max.x = this->_vertices[i].x;
		if (this->_vertices[i].y > max.y)
			max.y = this->_vertices[i].y;
		if (this->_vertices[i].z > max.z)
			max.z = this->_vertices[i].z;
		if (this->_vertices[i].x < min.x)
			min.x = this->_vertices[i].x;
		if (this->_vertices[i].y < min.y)
			min.y = this->_vertices[i].y;
		if (this->_vertices[i].z < min.z)
			min.z = this->_vertices[i].z;
	}
	middle.x /= this->_vertices.size();
	middle.y /= this->_vertices.size();
	middle.z /= this->_vertices.size();

	// Calculate diameter
	diameter = std::max(max.x - min.x, std::max(max.y - min.y, max.z - min.z));

	for (Face &face : this->_faces)
	{
		for (Vertex &vertex : face.vertices)
		{
			vertex.pos.x = (vertex.pos.x - middle.x) / diameter;
			vertex.pos.y = (vertex.pos.y - middle.y) / diameter;
			vertex.pos.z = (vertex.pos.z - middle.z) / diameter;
		}
	}
}

void	Parser::_calculateNormals(void)
{
	Vector3D	normal;
	Vertex		v1;
	Vertex		v2;
	Vertex		v3;

	for (Face &face : this->_faces)
	{
		v1 = face.vertices[0];
		v2 = face.vertices[1];
		v3 = face.vertices[2];
		normal = (v2.pos - v1.pos).cross(v3.pos - v1.pos);
		normal.normalize();
		face.normal = normal;
	}
}

void	Parser::_calculateUVs(void)
{
	for (Face &face : this->_faces)
	{
		for (Vertex &vertex : face.vertices)
		{
			// Calculate u and v using the normal and the position
			if (std::abs(face.normal.x) > std::abs(face.normal.y) && std::abs(face.normal.x) > std::abs(face.normal.z))
				vertex.uv = Vector3D(vertex.pos.y, vertex.pos.z, 0.0);
			else if (std::abs(face.normal.y) > std::abs(face.normal.x) && std::abs(face.normal.y) > std::abs(face.normal.z))
				vertex.uv = Vector3D(vertex.pos.x, vertex.pos.z, 0.0);
			else
				vertex.uv = Vector3D(vertex.pos.x, vertex.pos.y, 0.0);
		}
	}
}

void	Parser::_generateFaceColors(void)
{
	for (Face &face : this->_faces)
	{
		face.color = Vector3D(
			(double)rand() / RAND_MAX,
			(double)rand() / RAND_MAX,
			(double)rand() / RAND_MAX
		);
	}
}

void	Parser::_generateVertexDataAndIndicies(void)
{
	unsigned int	index = 0;

	for (Face &face : this->_faces)
	{
		for (int i = 0; i < face.vertices.size() - 2; i++)
		{
			int indexes[3] = {0, i + 1, i + 2};
			for (int j : indexes)
			{
				this->_vertex_data.push_back(face.vertices[j].pos.x);
				this->_vertex_data.push_back(face.vertices[j].pos.y);
				this->_vertex_data.push_back(face.vertices[j].pos.z);
				this->_vertex_data.push_back(face.color.x);
				this->_vertex_data.push_back(face.color.y);
				this->_vertex_data.push_back(face.color.z);
				this->_vertex_data.push_back(face.normal.x);
				this->_vertex_data.push_back(face.normal.y);
				this->_vertex_data.push_back(face.normal.z);
				this->_vertex_data.push_back(face.vertices[j].uv.x);
				this->_vertex_data.push_back(face.vertices[j].uv.y);
				this->_indices.push_back(index);
				index++;
			}
		}
	}
}

unsigned int	*Parser::getIndicesArray(void)
{
	return (&this->_indices[0]);
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
