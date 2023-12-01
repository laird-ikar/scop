/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguyot <bguyot@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 16:03:17 by bguyot            #+#    #+#             */
/*   Updated: 2023/12/01 16:30:54 by bguyot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <scop.hpp>

static void print_help(void)
{
	std::cout << "\033[1mControls\033[0m:" << std::endl;
	std::cout << "  - \033[1mW\033[0m, \033[1mA\033[0m, \033[1mS\033[0m, \033[1mD\033[0m: Move the object" << std::endl;
	std::cout << "  - \033[1mQ\033[0m, \033[1mE\033[0m:       Rotate the object on Z axis" << std::endl;
	std::cout << "  - \033[1mR\033[0m, \033[1mF\033[0m:       Rotate the object on X axis" << std::endl;
	std::cout << "  - \033[1m[\033[0m, \033[1m]\033[0m:       Change display mode (Faces, Lines, Points)" << std::endl;
	std::cout << "  - \033[1m{\033[0m, \033[1m}\033[0m:       Change color mode (Color, Unicorn, Normals, Monochrome)" << std::endl;
	std::cout << "  - \033[1m-\033[0m, \033[1m=\033[0m:       Change the distance of the camera from the object" << std::endl;
	std::cout << "  - \033[1mSPACE\033[0m:      Stop the time" << std::endl;
	std::cout << "  - \033[1mESC\033[0m:        Quit" << std::endl;
}

int main(int argc, char **argv)
{
	Parser parser;

	if (argc < 2)
		parser = Parser();
	else
		parser = Parser(std::string(argv[1]));
	print_help();
	displayer(parser);
}
