/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguyot <bguyot@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 11:10:39 by bguyot            #+#    #+#             */
/*   Updated: 2023/11/23 16:58:57 by bguyot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Parser.class.hpp>

Parser::Parser(void)
{
	this->vertices = std::vector<float>({
		0.0f, 0.5f, 0.0f,	// S
		0.0f, 0.0f, 0.5f,	// A
		-0.5f, -0.5f, -0.5f,// B
		0.5f, -0.5f, -0.5f,	// C
	});
	this->indices = std::vector<unsigned int>({
		1, 2, 3,	// ABC
		0, 2, 1,	// SAB
		0, 3, 2,	// SBC
		0, 1, 3,	// SCA
	});
	this->normals = std::vector<float>({
		1.0f, 0.0f, 0.0f,	// S
		1.0f, 0.0f, 0.0f,	// A
		1.0f, 0.0f, 0.0f,	// B
		1.0f, 0.0f, 0.0f,	// C
	});
}

Parser::Parser(const std::string &path)
{}

Parser::Parser(const Parser &other)
{}

Parser::~Parser(void)
{}

Parser			&Parser::operator=(const Parser &rhs)
{
	if (this != &rhs)
	{
		this->vertices = rhs.vertices;
		this->indices = rhs.indices;
	}
	return *this;
}

float			*Parser::getVerticesArray(void)
{
	return (&this->vertices[0]);
}

float			*Parser::getNormalsArray(void)
{
	return (&this->normals[0]);
}

unsigned int	*Parser::getIndicesArray(void)
{
	return (&this->indices[0]);
}

size_t			Parser::getNbVertices(void)
{
	return (this->vertices.size());
}

size_t			Parser::getNbNormals(void)
{
	return (this->normals.size());
}

size_t			Parser::getNbIndices(void)
{
	return (this->indices.size());
}

