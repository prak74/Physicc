#ifndef _SPHERE_H
#define _SPHERE_H

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "stb_image.h"

class Sphere {
private:
	
public:
	int tri_idx = 0;
	int tesselation = 50;
	int Lat = 10;
	int Long = 10;
	double Radius = 2;
	glm::vec4 v_positions[3600];
	glm::vec4 v_normals[3600];

	GLuint sphereVAO, sphereVBO;

	void sphereGen(double radius, int Lats, int Longs);
	Sphere(unsigned int ID);
};

#endif
