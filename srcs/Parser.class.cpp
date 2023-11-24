/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguyot <bguyot@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 11:10:39 by bguyot            #+#    #+#             */
/*   Updated: 2023/11/24 11:51:06 by bguyot           ###   ########.fr       */
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
		0, 3, 2,			// SBC
		0, 1, 3,			// SCA
		1, 2, 3				// ACB
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

