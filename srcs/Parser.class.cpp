/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguyot <bguyot@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 11:10:39 by bguyot            #+#    #+#             */
/*   Updated: 2023/11/27 11:43:02 by bguyot           ###   ########.fr       */
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
		0.0, 0.5, 0.0,	// S
		0.0, -0.5, -0.5,	// A
		-0.5, -0.5, 0.5,// B
		0.5, -0.5, 0.5,	// C
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
			// std::cout << "face: ";
			unsigned int first;
			unsigned int previous;
			std::string token;

			// skip f
			size_t pos = line.find(" ");
			line.erase(0, pos + 1);

			// load first vertex
			pos = line.find(" ");
			token = line.substr(0, pos);
			first = std::stoul(token)- 1;
			line.erase(0, pos + 1);

			// load second vertex
			pos = line.find(" ");
			token = line.substr(0, pos);
			previous = std::stoul(token)- 1;
			line.erase(0, pos + 1);


			while ((pos = line.find(" ")) != std::string::npos) {
				token = line.substr(0, pos);
				this->_indices.push_back(first);
				this->_indices.push_back(previous);
				this->_indices.push_back(std::stoi(token) - 1);

				// std::cout << first << "," << previous << "," << std::stof(token) - 1 << " ; ";

				line.erase(0, pos + 1);
				previous = std::stof(token)- 1;
			}
			this->_indices.push_back(first);
			this->_indices.push_back(previous);
			this->_indices.push_back(std::stod(line) - 1);
			// std::cout << first << "," << previous << "," << std::stof(line) - 1;
			// std::cout << std::endl;
		}
		// if (/*l*/)
		// 	line;
		// if (/*mtllib*/)
		// 	loadMTL;
		// if (/*usemtl*/)
		// 	mtl;
	}
	auto min_max = std::minmax_element(this->_vertices.begin(), this->_vertices.end());
	double min = *min_max.first;
	double max = *min_max.second;
	for (int i = 0; i < this->_vertices.size() ; i++)
	{
		this->_vertices[i] = -0.8 + (this->_vertices[i] - min) / (max- min) * (1.6);
	}
	double	Xmin = this->_vertices[0],
			Xmax = this->_vertices[0],
			Ymin = this->_vertices[1],
			Ymax = this->_vertices[1],
			Zmin = this->_vertices[2],
			Zmax = this->_vertices[2];
	int		count = 0;
	for (double &e : this->_vertices)
	{
		switch (count)
		{
		case 0:
			if (Xmin > e)
				Xmin = e;
			if (Xmax < e)
				Xmax = e;
			break;
		case 1:
			if (Ymin > e)
				Ymin = e;
			if (Ymax < e)
				Ymax = e;
			break;
		case 2:
			if (Zmin > e)
				Zmin = e;
			if (Zmax < e)
				Zmax = e;
			break;
		}
		count++;
		count %= 3;
	}
	count = 0;
	for (double &e : this->_vertices)
	{
		switch (count)
		{
		case 0:
			e -= (Xmin + Xmax) / 2;
			break;
		case 1:
			e -= (Ymin + Ymax) / 2;
			break;
		case 2:
			e -= (Zmin + Zmax) / 2;
			break;
		}
		count++;
		count %= 3;
	}
}

Parser::Parser(const Parser &other)
{}

Parser::~Parser(void)
{}

Parser			&Parser::operator=(const Parser &rhs)
{
	if (this != &rhs)
	{
		this->_vertices = std::vector<double>(rhs._vertices);
		this->_indices = std::vector<unsigned int>(rhs._indices);
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

