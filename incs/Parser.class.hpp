/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bguyot <bguyot@student.42mulhouse.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 10:58:55 by bguyot            #+#    #+#             */
/*   Updated: 2023/11/30 14:19:03 by bguyot           ###   ########.fr       */
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

struct Vector3D {
	double x, y, z;

	Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}
	Vector3D(const Vector3D &src) : x(src.x), y(src.y), z(src.z) {}
	Vector3D(void) : x(0), y(0), z(0) {}
	Vector3D(double n) : x(n), y(n), z(n) {}
	Vector3D(double *array) : x(array[0]), y(array[1]), z(array[2]) {}
	virtual ~Vector3D(void) {}

	Vector3D &operator=(const Vector3D &rhs) {
		if (this != &rhs) {
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
		}
		return *this;
	}

	Vector3D operator+(const Vector3D &rhs) const {
		return Vector3D(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	Vector3D operator-(const Vector3D &rhs) const {
		return Vector3D(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	Vector3D operator*(const double &rhs) const {
		return Vector3D(x * rhs, y * rhs, z * rhs);
	}

	Vector3D operator/(const double &rhs) const {
		return Vector3D(x / rhs, y / rhs, z / rhs);
	}

	Vector3D operator-() const {
		return Vector3D(-x, -y, -z);
	}

	Vector3D &operator+=(const Vector3D &rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	Vector3D &operator-=(const Vector3D &rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	Vector3D &operator*=(const double &rhs) {
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	Vector3D &operator/=(const double &rhs) {
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	bool operator==(const Vector3D &rhs) const {
		return (x == rhs.x && y == rhs.y && z == rhs.z);
	}

	bool operator!=(const Vector3D &rhs) const {
		return (x != rhs.x || y != rhs.y || z != rhs.z);
	}

	double dot(const Vector3D &rhs) const {
		return (x * rhs.x + y * rhs.y + z * rhs.z);
	}

	Vector3D cross(const Vector3D &rhs) const {
		return Vector3D(y * rhs.z - z * rhs.y,
						z * rhs.x - x * rhs.z,
						x * rhs.y - y * rhs.x);
	}

	double length() const {
		return sqrt(x * x + y * y + z * z);
	}

	Vector3D &normalize() {
		double l = length();
		x /= l;
		y /= l;
		z /= l;
		return *this;
	}

	// Méthode pour afficher le vecteur
	void print() const {
		std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
	}
};

struct Face {
	Face(void) : normal(0.0), color(0.0), UV(0.0) {}
	Face(const Face &src) : vertices(src.vertices), normal(src.normal), color(src.color), UV(src.UV) {}
	Face(const std::vector<Vector3D> &vertices) : vertices(vertices), normal(0.0), color(0.0), UV(0.0) {}
	virtual ~Face(void) {}

	std::vector<Vector3D> vertices;
	Vector3D normal;
	Vector3D color;
	Vector3D UV;
};

#define NB_DATA_FEILD 11

class Parser {
private:
	std::vector<Vector3D>			_vertices;
	std::vector<Face>				_faces;

	std::vector<double>				_vertex_data; // [X, Y, Z, R, G, B, L, M , M, U, V] * nb_vertex
	std::vector<unsigned int>		_indices;

	void							_parseFace(std::string &line);
	void							_parseVertex(std::string &line);
	void							_normalizeVerticesPositons(void);
	void							_calculateNormals(void);
	void							_calculateUVs(void);
	void							_generateVertexDataAndIndicies(void);
	void							_generateFaceColors(void);

public:
					Parser(void);
					Parser(const std::string &path);
					Parser(const Parser &src);
	virtual			~Parser(void);

	Parser			&operator=(const Parser &rhs);

	double			*getVertexDataArray(void);
	unsigned int	*getIndicesArray(void);
	size_t			getNbVertexData(void);
	size_t			getNbIndices(void);
};
