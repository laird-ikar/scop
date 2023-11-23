/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguyot <bguyot@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 11:10:39 by bguyot            #+#    #+#             */
/*   Updated: 2023/11/23 14:28:11 by bguyot           ###   ########.fr       */
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
		0, 1, 2,	// SAB
		0, 2, 3,	// SBC
		0, 3, 1,	// SCA
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

