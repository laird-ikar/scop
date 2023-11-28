/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp_parser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguyot <bguyot@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:01:40 by bguyot            #+#    #+#             */
/*   Updated: 2023/11/28 17:56:21 by bguyot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <iostream>
#include <vector>

struct BMPData
{
	char		magic_number[2];
	int			file_size;
	int			reserved;
	int			data_offset;
};

bool bmp_parser(const char* filePath, std::vector<unsigned char>& imageData, int& width, int& height)
{
	std::ifstream file(filePath, std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "Error opening BMP file: " << filePath << std::endl;
		return false;
	}

	BMPData bmpData;

	file.read(bmpData.magic_number, 2);
	file.read((char*)&bmpData.file_size, 4);
	file.read((char*)&bmpData.reserved, 4);
	file.read((char*)&bmpData.data_offset, 4);

	if (bmpData.magic_number[0] != 'B' || bmpData.magic_number[1] != 'M') {
		std::cerr << "Error: " << filePath << " is not a valid BMP file (magic number)" << std::endl;
		return false;
	}

	if (bmpData.data_offset > bmpData.file_size) {
		std::cerr << "Error: " << filePath << " is not a valid BMP file (offset)" << std::endl;
		return false;
	}

	int headerSize;
	file.read((char*)&headerSize, 4);

	std::cout << headerSize << std::endl;

	int bmpWidth;
	int bmpHeight;
	file.read((char*)&bmpWidth, 4);
	file.read((char*)&bmpHeight, 4);

	width = bmpWidth;
	height = bmpHeight;

	int numColorPlanes;
	file.read((char*)&numColorPlanes, 2);

	std::cout << numColorPlanes << std::endl;

	int bitsPerPixel;
	file.read((char*)&bitsPerPixel, 2);

	int compressionMethod;
	file.read((char*)&compressionMethod, 4);

	int XpixelsPerMeter;
	int YpixelsPerMeter;
	file.read((char*)&XpixelsPerMeter, 4);
	file.read((char*)&YpixelsPerMeter, 4);

	int numColorsInPalette;
	file.read((char*)&numColorsInPalette, 4);

	int numImportantColors;
	file.read((char*)&numImportantColors, 4);

	file.seekg(bmpData.data_offset);

	std::vector<unsigned char> palette;
	for (int i = 0; i < bmpData.data_offset - 54; i++) {
		char c;
		file.read(&c, 1);
		palette.push_back(c);
	}

	for (int i = 0; i < bmpHeight; i++) {
		for (int j = 0; j < bmpWidth; j++) {
			char c;
			file.read(&c, 1);
			imageData.push_back(c);
		}
	}
	return true;
}
