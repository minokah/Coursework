/*
	CS3388 Assignment 5 - Brandon Luu
	marchingcubes.cpp

	Creates a scalar field of marching cubes.
	It will generate them over a period of time, giving the effect of it growing.
*/

// Include standard headers
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <vector>
#include <iostream>
#include <functional>
#include <fstream>
using namespace std;

// Bitmaps for the vertices of a cube
#define BOTTOM_LEFT  1
#define BOTTOM_RIGHT 2
#define TOP_RIGHT    4
#define TOP_LEFT     8
#define BOTTOM_LEFT2 16
#define BOTTOM_RIGHT2 32
#define TOP_RIGHT2 64
#define TOP_LEFT2 128

//////////////////////////////////////////////////////////////////////////////
// Vertex Data
//////////////////////////////////////////////////////////////////////////////

int marching_cubes_lut[256][16] =
{ {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
{3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
{3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
{3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
{9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
{2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
{8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
{4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
{3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
{1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
{4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
{4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
{5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
{2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
{9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
{0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
{2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
{10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
{5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
{5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
{9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
{1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
{10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
{8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
{2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
{7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
{2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
{11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
{5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
{11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
{11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
{9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
{2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
{6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
{3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
{6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
{10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
{6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
{8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
{7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
{3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
{0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
{9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
{8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
{5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
{0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
{6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
{10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
{10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
{8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
{1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
{0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
{10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
{3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
{6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
{9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
{8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
{3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
{6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
{0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
{10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
{10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
{2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
{7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
{7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
{2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
{1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
{11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
{8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
{0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
{7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
{10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
{2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
{6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
{7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
{2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
{10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
{10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
{0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
{7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
{6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
{8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
{9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
{6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
{4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
{10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
{8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
{0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
{1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
{8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
{10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
{4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
{10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
{11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
{9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
{6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
{7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
{3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
{7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
{3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
{6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
{9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
{1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
{4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
{7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
{6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
{3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
{0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
{6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
{0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
{11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
{6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
{5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
{9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
{1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
{1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
{10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
{0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
{5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
{10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
{11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
{9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
{7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
{2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
{8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
{9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
{9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
{1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
{9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
{5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
{0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
{10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
{2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
{0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
{0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
{9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
{5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
{3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
{5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
{8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
{0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
{9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
{1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
{3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
{4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
{9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
{11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
{11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
{2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
{9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
{3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
{1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
{4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
{3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
{0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
{1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
};


float vertTable[12][3] = {
	{0.5f, 0.0f, 0.0f},
	{1.0f, 0.0f, 0.5f},
	{0.5f, 0.0f, 1.0f},
	{0.0f, 0.0f, 0.5f},
	{0.5f, 1.0f, 0.0f},
	{1.0f, 1.0f, 0.5f},
	{0.5f, 1.0f, 1.0f},
	{0.0f, 1.0f, 0.5f},
	{0.0f, 0.5f, 0.0f},
	{1.0f, 0.5f, 0.0f},
	{1.0f, 0.5f, 1.0f},
	{0.0f, 0.5f, 1.0f},
};



//////////////////////////////////////////////////////////////////////////////
// Helper Objects
//////////////////////////////////////////////////////////////////////////////

class Axes {

	glm::vec3 origin;
	glm::vec3 extents;

	glm::vec3 xcol = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 ycol = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 zcol = glm::vec3(0.0f, 0.0f, 1.0f);

public:

	Axes(glm::vec3 orig, glm::vec3 ex) : origin(orig), extents(ex) {}

	void draw() {

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();


		glLineWidth(2.0f);
		glBegin(GL_LINES);
		glColor3f(xcol.x, xcol.y, xcol.z);
		glVertex3f(origin.x, origin.y, origin.z);
		glVertex3f(origin.x + extents.x, origin.y, origin.z);

		glVertex3f(origin.x + extents.x, origin.y, origin.z);
		glVertex3f(origin.x + extents.x, origin.y, origin.z + 0.1);
		glVertex3f(origin.x + extents.x, origin.y, origin.z);
		glVertex3f(origin.x + extents.x, origin.y, origin.z - 0.1);

		glColor3f(ycol.x, ycol.y, ycol.z);
		glVertex3f(origin.x, origin.y, origin.z);
		glVertex3f(origin.x, origin.y + extents.y, origin.z);

		glVertex3f(origin.x, origin.y + extents.y, origin.z);
		glVertex3f(origin.x, origin.y + extents.y, origin.z + 0.1);
		glVertex3f(origin.x, origin.y + extents.y, origin.z);
		glVertex3f(origin.x, origin.y + extents.y, origin.z - 0.1);

		glColor3f(zcol.x, zcol.y, zcol.z);
		glVertex3f(origin.x, origin.y, origin.z);
		glVertex3f(origin.x, origin.y, origin.z + extents.z);

		glVertex3f(origin.x, origin.y, origin.z + extents.z);
		glVertex3f(origin.x + 0.1, origin.y, origin.z + extents.z);

		glVertex3f(origin.x, origin.y, origin.z + extents.z);
		glVertex3f(origin.x - 0.1, origin.y, origin.z + extents.z);
		glEnd();


		glPopMatrix();
	}

};

// Scalar field functions
float waves(float x, float y, float z) {
	return y - sin(x) * cos(z);
}

float cones(float x, float y, float z) {
	return pow(x, 2) - pow(y, 2) - pow(z, 2) - z;
}

// Marching Cubes function
// Modified marching cubes function, where instead of looping through all the values, we will generate the n-th z plane
vector<float> marching_cubes(function<float(float, float, float)> f, float isoval, float min, float max, float stepsize, float z) {
	std::vector<float> vertices;

	float tl, tr, br, bl; // left side
	float tll, trr, brr, bll; // right side
	int which = 0;
	int* verts;

	for (float y = min; y < max; y += stepsize) {
		for (float x = min; x < max; x += stepsize) {
			//test the square
			bl = (f)(x, y, z);
            br = (f)(x + stepsize, y, z);
            tr = (f)(x + stepsize, y, z + stepsize);
            tl = (f)(x, y, z + stepsize);
            bll = (f)(x, y + stepsize, z);
            brr = (f)(x + stepsize, y + stepsize, z);
            trr = (f)(x + stepsize, y + stepsize, z + stepsize);
            tll = (f)(x, y + stepsize, z + stepsize);

			which = 0;
			if (bl < isoval) { // 1
				which |= BOTTOM_LEFT;
			}
			if (br < isoval) { // 2
				which |= BOTTOM_RIGHT;
			}
			if (tr < isoval) { // 4
				which |= TOP_RIGHT;
			}
			if (tl < isoval) { // 8
				which |= TOP_LEFT;
			}
			if (bll < isoval) { // 16
				which |= BOTTOM_LEFT2;
			}
			if (brr < isoval) { // 32
				which |= BOTTOM_RIGHT2;
			}
			if (trr < isoval) { // 64
				which |= TOP_RIGHT2;
			}
			if (tll < isoval) { // 128
				which |= TOP_LEFT2;
			}

			// Add all the vertices
			verts = marching_cubes_lut[which];
			if (verts[0] >= 0) {
				vertices.emplace_back(x + stepsize * vertTable[verts[0]][0]);
				vertices.emplace_back(y + stepsize * vertTable[verts[0]][1]);
				vertices.emplace_back(z + stepsize * vertTable[verts[0]][2]);
				vertices.emplace_back(x + stepsize * vertTable[verts[1]][0]);
				vertices.emplace_back(y + stepsize * vertTable[verts[1]][1]);
				vertices.emplace_back(z + stepsize * vertTable[verts[1]][2]);
				vertices.emplace_back(x + stepsize * vertTable[verts[2]][0]);
				vertices.emplace_back(y + stepsize * vertTable[verts[2]][1]);
				vertices.emplace_back(z + stepsize * vertTable[verts[2]][2]);
			}
			if (verts[3] >= 0) {
				vertices.emplace_back(x + stepsize * vertTable[verts[3]][0]);
				vertices.emplace_back(y + stepsize * vertTable[verts[3]][1]);
				vertices.emplace_back(z + stepsize * vertTable[verts[3]][2]);
				vertices.emplace_back(x + stepsize * vertTable[verts[4]][0]);
				vertices.emplace_back(y + stepsize * vertTable[verts[4]][1]);
				vertices.emplace_back(z + stepsize * vertTable[verts[4]][2]);
				vertices.emplace_back(x + stepsize * vertTable[verts[5]][0]);
				vertices.emplace_back(y + stepsize * vertTable[verts[5]][1]);
				vertices.emplace_back(z + stepsize * vertTable[verts[5]][2]);
			}
			if (verts[6] >= 0) {
				vertices.emplace_back(x + stepsize * vertTable[verts[6]][0]);
				vertices.emplace_back(y + stepsize * vertTable[verts[6]][1]);
				vertices.emplace_back(z + stepsize * vertTable[verts[6]][2]);
				vertices.emplace_back(x + stepsize * vertTable[verts[7]][0]);
				vertices.emplace_back(y + stepsize * vertTable[verts[7]][1]);
				vertices.emplace_back(z + stepsize * vertTable[verts[7]][2]);
				vertices.emplace_back(x + stepsize * vertTable[verts[8]][0]);
				vertices.emplace_back(y + stepsize * vertTable[verts[8]][1]);
				vertices.emplace_back(z + stepsize * vertTable[verts[8]][2]);
			}
			if (verts[9] >= 0) {
				vertices.emplace_back(x + stepsize * vertTable[verts[9]][0]);
				vertices.emplace_back(y + stepsize * vertTable[verts[9]][1]);
				vertices.emplace_back(z + stepsize * vertTable[verts[9]][2]);
				vertices.emplace_back(x + stepsize * vertTable[verts[10]][0]);
				vertices.emplace_back(y + stepsize * vertTable[verts[10]][1]);
				vertices.emplace_back(z + stepsize * vertTable[verts[10]][2]);
				vertices.emplace_back(x + stepsize * vertTable[verts[11]][0]);
				vertices.emplace_back(y + stepsize * vertTable[verts[11]][1]);
				vertices.emplace_back(z + stepsize * vertTable[verts[11]][2]);
			}
		}
	}

	return vertices;
}

// Function that computes our normals given our marching cubes
vector<float> compute_normals(vector<float> cubes) {
	vector<float> vec;

	for (int i = 0; i != cubes.size(); i += 9) {
		vec3 v1 = vec3(cubes[i], cubes[i + 1], cubes[i + 2]);
		vec3 v2 = vec3(cubes[i + 3], cubes[i + 4], cubes[i + 5]);
		vec3 v3 = vec3(cubes[i + 6], cubes[i + 7], cubes[i + 8]);

		vec3 v21 = v2 - v1;
		vec3 v31 = v3 - v1;

		vec3 cr = normalize(cross(v21, v31));

		// Push it back 3 times each for one tri/3 vertices
		vec.push_back(cr.x);
		vec.push_back(cr.y);
		vec.push_back(cr.z);
		vec.push_back(cr.x);
		vec.push_back(cr.y);
		vec.push_back(cr.z);
		vec.push_back(cr.x);
		vec.push_back(cr.y);
		vec.push_back(cr.z);
	}

	return vec;
}

vector<float> vertices, normal;

// Struct to store our VAO/VBOs
struct Object {
	GLuint VAO, posVBO, normalVBO;

	// Initialize the VAO
	void init() {
		glGenVertexArrays(1, &VAO);
	}

	// Regenerate the field, delete our old VBOs so we don't hit memory cap
	void regenerate() {
		glDeleteBuffers(1, &posVBO);
		glDeleteBuffers(1, &normalVBO);

		// Position
		glBindVertexArray(VAO);
		glGenBuffers(1, &posVBO);
		glBindBuffer(GL_ARRAY_BUFFER, posVBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// Normals
		glGenBuffers(1, &normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(GLfloat), normal.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBindVertexArray(0);
	}

	// Draw function, bind it and draw
	void draw() {
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glBindVertexArray(0);
	}
};

//////////////////////////////////////////////////////////////////////////////
// Main
//////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	///////////////////////////////////////////////////////
	int currentStep = 1;
	int substep = 1;
	if (argc > 1) {
		currentStep = atoi(argv[1]);
	}
	if (argc > 2) {
		substep = atoi(argv[2]);
	}
	///////////////////////////////////////////////////////

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	float screenW = 1400;
	float screenH = 900;
	window = glfwCreateWindow(screenW, screenH, "Marching Cubes Generator", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.2f, 0.2f, 0.3f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	std::string VertexShaderCode = "\
    	#version 400\n\
		// Input vertex data, different for all executions of this shader.\n\
		layout(location = 0) in vec3 vertexPosition_modelspace;\n\
		layout(location = 1) in vec3 vertexNormal_modelspace;\n\
		// Output data ; will be interpolated for each fragment.\n\
		out vec3 Normal_cameraspace;\n\
		out vec3 EyeDirection_cameraspace;\n\
		out vec3 LightDirection_cameraspace;\n\
		// Values that stay constant for the whole mesh.\n\
		uniform mat4 MVP;\n\
		uniform mat4 V;\n\
		uniform mat4 M;\n\
		uniform vec3 LightPosition_worldspace;\n\
		void main(){ \n\
			// Output position of the vertex, in clip space : MVP * position\n\
			gl_Position =  MVP * vec4(vertexPosition_modelspace,1);\n\
			vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition_modelspace,1)).xyz;\n\
			EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;\n\
			vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace,1)).xyz;\n\
			LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;\n\
			Normal_cameraspace = (V * M * vec4(vertexNormal_modelspace, 0)).xyz;\n\
		}";

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode = "\
		#version 400\n\
		// Interpolated values from the vertex shaders\n\
		in vec3 Normal_cameraspace;\n\
		in vec3 EyeDirection_cameraspace;\n\
		in vec3 LightDirection_cameraspace;\n\
		out vec4 color;\n\
		uniform vec3 modelColor;\n\
		void main() {\n\
			// material colors\n\
			vec4 diffuse = vec4(modelColor.x, modelColor.y, modelColor.z, 1.0); // blue -green color\n\
			vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0);\n\
			vec4 specular = vec4(1.0, 1.0, 1.0, 1.0);\n\
			vec3 n = normalize( Normal_cameraspace );\n\
			vec3 l = normalize(LightDirection_cameraspace);\n\
			float cosTheta = clamp(dot(n, l), 0, 1);\n\
			// ensure dot product is between 0 and 1\n\
			vec3 E = normalize(EyeDirection_cameraspace);\n\
			vec3 R = reflect(-l, n);\n\
			float cosAlpha = clamp(dot(E, R), 0, 1);\n\
			float alpha = 64;\n\
			color = ambient + diffuse * cosTheta + specular * pow(cosAlpha, alpha);\n\
	}";
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("Vertex: %s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("Shader: %s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	//glDeleteShader(VertexShaderID);
	//glDeleteShader(FragmentShaderID);

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(ProgramID, "MVP");
	GLuint VID = glGetUniformLocation(ProgramID, "V");
	GLuint MID = glGetUniformLocation(ProgramID, "M");
	GLuint LightPosID = glGetUniformLocation(ProgramID, "LightPosition_worldspace");
	GLuint ModelColorID = glGetUniformLocation(ProgramID, "modelColor");

	glm::mat4 MVP;

	glm::vec3 up = { 0.0f, 1.0f, 0.0f };
	glm::vec3 center = { 0.0f, 0.0f, 0.0f };

	// Spherical Coordinate system variables
	// Default XYZ coordinates are (5,5,5), these values are an approximation
	float r = 8.66;
	float theta = 45;
	float phi = 54.74;

	// Mouse movement variables
	double prevMouseX = 0;
	double prevMouseY = 0;

	// Marching Cubes generation variables
	float isoval = -1.5;
	float max = 5;
	float min = -5;
	float stepsize = 0.05f;
	
	// Special "z" to generate the cubes over the frames
	float zgen = min;

	// XYZ axis
	Axes ax(glm::vec3(min, min, min), glm::vec3(max - min, max - min, max - min));

	// Our light position/vector
	glm::vec3 lightpos(-5.0f, -5.0f, -5.0f);

	// Generation delay
	int genDelay = 0;

	// Initialize the VAOs n stuff
	Object theshape;
	theshape.init();

	// Write and written or not
	bool writeEnabled = false;
	bool written = false;

	cout << "Generating..." << endl;

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), screenW / screenH, 0.001f, 1000.0f);
		// Projection = glm::mat4(1.0f);
		glLoadMatrixf(glm::value_ptr(Projection));

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		//glm::mat4 V;

		// Check cursor movement, then adjust phi and theta as needed
		int leftDown = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		if (leftDown == GLFW_PRESS) {
			double currentMouseX, currentMouseY;
			glfwGetCursorPos(window, &currentMouseX, &currentMouseY);
			if (currentMouseX < prevMouseX) theta -= 1.5f;
			else if (currentMouseX > prevMouseX) theta += 1.5f;

			if (currentMouseY < prevMouseY && phi + 1.5f < 180) phi += 1.5f;
			else if (currentMouseY > prevMouseY && phi - 1.5 > 0) phi -= 1.5f;
		}

		glfwGetCursorPos(window, &prevMouseX, &prevMouseY);

		// Check if up or down is pressed to change the radius (zoom)
		int upState = glfwGetKey(window, GLFW_KEY_UP);
		if (upState == GLFW_PRESS) r -= 0.5f;

		int downState = glfwGetKey(window, GLFW_KEY_DOWN);
		if (downState == GLFW_PRESS) r += 0.5f;

		if (r < 0.5f) r = 0.5f;

		// Generate our XYZ for the eye and use lookAt()
		float x = r * sin(radians(phi)) * cos(radians(theta));
		float y = r * cos(radians(phi));
		float z = r * sin(radians(phi)) * sin(radians(theta));

		glm::vec3 eye = { x, y, z };
		glm::mat4 V = glm::lookAt(eye, center, up);		

		glm::mat4 M = glm::mat4(1.0f);

		glm::mat4 MV = V * M;
		glLoadMatrixf(glm::value_ptr(V));

		MVP = Projection * V * M;

		// Draw cube boundaries at (min, min, min), (max, max, max)
		glColor3f(0.7f, 0.7f, 0.7f);
		glBegin(GL_LINE_STRIP);
		glVertex3f(min, max, min);
		glVertex3f(min, max, max);
		glVertex3f(max, max, max);
		glVertex3f(max, max, min);
		glVertex3f(min, max, min);
		glEnd();

		glBegin(GL_LINE_STRIP);
		glVertex3f(max, min, max);
		glVertex3f(max, max, max);
		glVertex3f(max, max, min);
		glVertex3f(max, min, min);
		glVertex3f(max, min, max);

		glVertex3f(max, max, max);
		glVertex3f(min, max, max);
		glVertex3f(min, min, max);
		glVertex3f(max, min, max);
		glEnd();

		// Set up our shaders
		glUseProgram(ProgramID);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(VID, 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(MID, 1, GL_FALSE, &M[0][0]);
		glUniform3f(LightPosID, lightpos.x, lightpos.y, lightpos.z);
		glUniform3f(ModelColorID, 0.0f, 1.0f, 1.0f);

		// Draw it and finish shader
		theshape.draw();
		glUseProgram(0);

		ax.draw();

		genDelay++;

		// This will be used to generate the marching fields, frame by frame
		if (zgen < max) {
			// 2 frame generation delay
			if (genDelay > 2) {
				zgen += stepsize;
				vector<float> cubeResult = marching_cubes(cones, isoval, min, max, stepsize, zgen);
				vertices.insert(vertices.end(), cubeResult.begin(), cubeResult.end());

				vector<float> normalResult = compute_normals(cubeResult);
				normal.insert(normal.end(), normalResult.begin(), normalResult.end());

				theshape.regenerate();
			}
		}
		// We are done generating, lets write the file
		else if (!written && writeEnabled) {
			written = true;

			cout << "Writing to fileName.ply..." << endl;

			ofstream file;

			// Open the file and overwrite what was already there
			file.open("fileName.ply", ios::out);

			// Write the headers
			file << "ply\n";
			file << "format ascii 1.0\n";
			file << "element vertex " << vertices.size()/3 << "\n";
			file << "property float x\n";
			file << "property float y\n";
			file << "property float z\n";
			file << "property float nx\n";
			file << "property float ny\n";
			file << "property float nz\n";
			file << "element face " << vertices.size()/9 << "\n";
			file << "property list uchar uint vertex_indices\n";
			file << "end_header\n";
			
			// Write the vertices
			for (int i = 0; i != vertices.size(); i += 3) {
				file << vertices[i] << " " << vertices[i + 1] << " " << vertices[i + 2] << " " << normal[i] << " " << normal[i + 1] << " " << normal[i + 2] << "\n";
			}

			int count = 0;

			// Write the faces/tris
			for (int i = 0; i != vertices.size(); i += 9) {
				file << "3 " << count << " " << count + 1 << " " << count + 2 << "\n";
				count += 3;
			}

			file.close();

			cout << "Done!" << endl;
		}

		// Give it a frame delay so it doesn't grow too too fast
		if (genDelay > 2) genDelay = 0;

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();


	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup shader
	glDeleteProgram(ProgramID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
