/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguyot <bguyot@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 11:10:39 by bguyot            #+#    #+#             */
/*   Updated: 2023/11/24 17:00:58 by bguyot           ###   ########.fr       */
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
	this->vertices = std::vector<float>({
		0.0f, 0.5f, 0.0f,	// S
		0.0f, -0.5f, -0.5f,	// A
		-0.5f, -0.5f, 0.5f,// B
		0.5f, -0.5f, 0.5f,	// C
	});
	this->indices = std::vector<unsigned int>({
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
				this->vertices.push_back(std::stof(token));
				line.erase(0, pos + 1);
			}
			this->vertices.push_back(std::stof(line));
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
				this->indices.push_back(first);
				this->indices.push_back(previous);
				this->indices.push_back(std::stof(token) - 1);

				// std::cout << first << "," << previous << "," << std::stof(token) - 1 << " ; ";

				line.erase(0, pos + 1);
				previous = std::stof(token)- 1;
			}
			this->indices.push_back(first);
			this->indices.push_back(previous);
			this->indices.push_back(std::stof(line) - 1);
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
	auto min_max = std::minmax_element(this->vertices.begin(), this->vertices.end());
	float min = *min_max.first;
	float max = *min_max.second;
	for (int i = 0; i < this->vertices.size() ; i++)
	{
		this->vertices[i] = -0.8f + (this->vertices[i] - min) / (max- min) * (1.6f);
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
		this->vertices = std::vector<float>(rhs.vertices);
		this->indices = std::vector<unsigned int>(rhs.indices);
	}
	return *this;
}

float			*Parser::getVerticesArray(void)
{
	return (&this->vertices[0]);
}

unsigned int	*Parser::getIndicesArray(void)
{
	return (&this->indices[0]);
}

size_t			Parser::getNbVertices(void)
{
	return (this->vertices.size());
}

size_t			Parser::getNbIndices(void)
{
	return (this->indices.size());
}

