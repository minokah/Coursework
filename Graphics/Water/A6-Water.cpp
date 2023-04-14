// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
//GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <iostream>
#include <vector>

#include "PlaneMesh.hpp"
#include "GeneralGraphics.h"
#include "shader.hpp"


//////////////////////////////////////////////////////////////////////////////
// Main
//////////////////////////////////////////////////////////////////////////////

int main( int argc, char* argv[])
{

	///////////////////////////////////////////////////////
	float screenW = 1500;
	float screenH = 1500;
	float stepsize = 1.0f;

	float xmin = -10;
	float xmax = 10;

	if (argc > 1 ) {
		screenW = atoi(argv[1]);
	}
	if (argc > 2) {
		screenH = atoi(argv[2]);
	}
	if (argc > 3) {
		stepsize = atof(argv[3]);
	}
	if (argc > 4) {
		xmin = atof(argv[4]);
	}
	if (argc > 5) {
		xmax = atof(argv[5]);
	}


	///////////////////////////////////////////////////////

	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( screenW, screenH, "The Legend of Zelda: Wind Waker (real!!!)", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
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


	PlaneMesh plane(xmin, xmax, stepsize);
	
	TexturedMesh boat("Assets/boat.ply", "Assets/boat.bmp", "BoatShader.vertexshader", "BoatShader.fragmentshader");
	TexturedMesh head("Assets/head.ply", "Assets/head.bmp", "BoatShader.vertexshader", "BoatShader.fragmentshader");
	TexturedMesh eyes("Assets/eyes.ply", "Assets/eyes.bmp", "BoatShader.vertexshader", "BoatShader.fragmentshader");

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glPatchParameteri(GL_PATCH_VERTICES, 4);

	// Dark blue background
	glClearColor(0.2f, 0.2f, 0.3f, 0.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glDisable(GL_CULL_FACE);

	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), screenW/screenH, 0.001f, 1000.0f);

	glm::vec3 lightpos(5.0f, 30.0f, 5.0f);

	// Look vectors
	// Spherical Coordinate system variables
	// Default XYZ coordinates are (5,5,5), these values are an approximation
	float r = 8.66;
	float theta = -135;
	float phi = 70;

	// Mouse movement variables
	double prevMouseX = 0;
	double prevMouseY = 0;

	//glm::vec3 eye = { 0.5f, 1.0f, 3.0f };
	glm::vec3 up = { 0.0f, 1.0f, 0.0f };
	glm::vec3 look = { 0, 0.5f, 0 };

	float angle = -90.0f;
	float yangle = 0.0f;

	float time = 0;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Get a handle for our "MVP" uniform
	glm::mat4 MVP;

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {
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

		if (r < 3) r = 3;

		// Generate our XYZ for the eye and use lookAt()
		float x = r * sin(radians(phi)) * cos(radians(theta));
		float y = r * cos(radians(phi));
		float z = r * sin(radians(phi)) * sin(radians(theta));

		// Clear the screen
		glClearColor(0.2f, 0.2f, 0.3f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Setup MVP matrices for our screen and world
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), screenW / screenH, 0.001f, 1000.0f);
		// Projection = glm::mat4(1.0f);
		glLoadMatrixf(glm::value_ptr(Projection));

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		//cameraControlsGlobe(eye, look, angle, yangle);
		vec3 eye = vec3(x, y, z);
		glm::mat4 V = glm::lookAt(eye, look, up);

		glm::mat4 M = glm::mat4(1.0f);

		glm::mat4 MV = V * M;
		glLoadMatrixf(glm::value_ptr(V));

		MVP = Projection * V * M;

		plane.draw(lightpos, MVP, V, M, time);

		boat.draw(MVP, time);
		head.draw(MVP, time);
		eyes.draw(MVP, time);

		time += 0.005;

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}

