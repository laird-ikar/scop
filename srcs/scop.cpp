/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguyot <bguyot@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 16:03:17 by bguyot            #+#    #+#             */
/*   Updated: 2023/11/23 15:54:11 by bguyot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <scop.hpp>

int main(int argc, char **argv)
{
	Parser parser;

	if (argc < 2)
		parser = Parser();
	else
		parser = Parser(std::string(argv[1]));
	displayer(parser);
}
