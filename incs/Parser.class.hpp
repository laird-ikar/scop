/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguyot <bguyot@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 10:58:55 by bguyot            #+#    #+#             */
/*   Updated: 2023/11/28 17:03:05 by bguyot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


class Parser;
#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cmath>

#define NB_DATA_FEILD 8

class Parser {
private:
	std::vector<double>			_vertices;
	std::vector<double>			_vertex_data; // [X, Y, Z, R, G, B, U, V] * nb_vertex
	std::vector<unsigned int>	_indices;

public:
					Parser(void);
					Parser(const std::string &path);
					Parser(const Parser &src);
	virtual			~Parser(void);

	Parser			&operator=(const Parser &rhs);

	double			*getVerticesArray(void);
	double			*getVertexDataArray(void);
	unsigned int	*getIndicesArray(void);
	size_t			getNbVertices(void);
	size_t			getNbVertexData(void);
	size_t			getNbIndices(void);
};
