/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp_parser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguyot <bguyot@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:01:40 by bguyot            #+#    #+#             */
/*   Updated: 2023/11/29 10:49:52 by bguyot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <iostream>
#include <vector>

bool bmp_parser(const char* filePath, std::vector<unsigned char>& imageData, int& width, int& height)
{
	std::ifstream file(filePath, std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "Error opening BMP file: " << filePath << std::endl;
		return false;
	}

	char magic_number[2];
	file.read(magic_number, 2);
	std::cout << "Magic number: " << magic_number[0] << magic_number[1] << std::endl;

	if (magic_number[0] != 'B' || magic_number[1] != 'M') {
		std::cerr << "Invalid BMP file: " << filePath << std::endl;
		return false;
	}

	uint32_t file_size;
	file.read((char*)&file_size, 4);
	std::cout << "File size: " << file_size << std::endl;

	uint32_t reserved;
	file.read((char*)&reserved, 4);
	std::cout << "Reserved: " << reserved << std::endl;

	uint32_t data_offset;
	file.read((char*)&data_offset, 4);
	std::cout << "Data offset: " << data_offset << std::endl;

	uint32_t header_size;
	file.read((char*)&header_size, 4);
	std::cout << "Header size: " << header_size << std::endl;

	file.read((char*)&width, 4);
	std::cout << "Width: " << width << std::endl;

	file.read((char*)&height, 4);
	std::cout << "Height: " << height << std::endl;

	uint16_t planes;
	file.read((char*)&planes, 2);
	std::cout << "Planes: " << planes << std::endl;

	uint16_t bits_per_pixel;
	file.read((char*)&bits_per_pixel, 2);
	std::cout << "Bits per pixel: " << bits_per_pixel << std::endl;

	uint32_t compression;
	file.read((char*)&compression, 4);
	std::cout << "Compression: " << compression << std::endl;

	uint32_t data_size;
	file.read((char*)&data_size, 4);
	std::cout << "Data size: " << data_size << std::endl;

	uint32_t h_resolution;
	file.read((char*)&h_resolution, 4);
	std::cout << "Horizontal resolution: " << h_resolution << std::endl;

	uint32_t v_resolution;
	file.read((char*)&v_resolution, 4);
	std::cout << "Vertical resolution: " << v_resolution << std::endl;

	uint32_t colors;
	file.read((char*)&colors, 4);
	std::cout << "Colors: " << colors << std::endl;

	uint32_t important_colors;
	file.read((char*)&important_colors, 4);
	std::cout << "Important colors: " << important_colors << std::endl;

	// Skip the rest of the header
	file.seekg(data_offset, std::ios::beg);

	std::vector<unsigned char> palette;
	if (bits_per_pixel != 24) {
		// Load the palette
		palette.resize(colors * 4);
		file.read((char*)palette.data(), colors * 4);
	}

	// Read the data
	imageData.resize(data_size);
	if (bits_per_pixel == 24)
		file.read((char*)imageData.data(), data_size);
	else if (bits_per_pixel == 8) {
		// Read the indices of the palette
		std::vector<unsigned char> indices(data_size);
		file.read((char*)indices.data(), data_size);

		// Convert the indices to RGB
		for (int i = 0; i < data_size; i++) {
			imageData[i * 3 + 0] = palette[indices[i] * 4 + 2];
			imageData[i * 3 + 1] = palette[indices[i] * 4 + 1];
			imageData[i * 3 + 2] = palette[indices[i] * 4 + 0];
		}
	}
	else {
		std::cerr << "Unsupported bits per pixel: " << bits_per_pixel << std::endl;
		return false;
	}

	return (true);
}
