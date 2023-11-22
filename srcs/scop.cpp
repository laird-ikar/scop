/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguyot <bguyot@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 16:03:17 by bguyot            #+#    #+#             */
/*   Updated: 2023/11/22 17:44:43 by bguyot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <scop.hpp>

int main(void)
{
	float vertices[] = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 1.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		// 1, 2, 3    // second triangle
	};

	displayer(vertices, indices, sizeof (vertices), sizeof (indices));
}
