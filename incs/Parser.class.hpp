/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguyot <bguyot@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 10:58:55 by bguyot            #+#    #+#             */
/*   Updated: 2023/11/23 15:58:11 by bguyot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


class Parser;
#pragma once
#include <vector>
#include <string>

class Parser {
private:
	std::vector<float>			vertices;
	std::vector<float>			normals;
	std::vector<unsigned int>	indices;

public:
					Parser(void);
					Parser(const std::string &path);
					Parser(const Parser &src);
	virtual			~Parser(void);

	Parser			&operator=(const Parser &rhs);

	float			*getVerticesArray(void);
	float			*getNormalsArray(void);
	unsigned int	*getIndicesArray(void);
	size_t			getNbVertices(void);
	size_t			getNbNormals(void);
	size_t			getNbIndices(void);
};
