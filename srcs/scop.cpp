/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguyot <bguyot@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 16:03:17 by bguyot            #+#    #+#             */
/*   Updated: 2023/11/23 11:55:06 by bguyot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <scop.hpp>

int main(int argc, char **argv)
{
	Parser p;

	if (argc < 2)
		p = Parser();
	else
		p = Parser(std::string(argv[1]));
	displayer(
		p.getVerticesArray(),
		p.getIndicesArray(),
		p.getNbVertices(),
		p.getNbIndices()
	);
}
