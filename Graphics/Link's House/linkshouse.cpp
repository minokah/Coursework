/*
	CS3888 Assignment 4 - Brandon Luu
	linkshouse.cpp

	Link's House from The Legend of Zelda: Ocarina of Time

	Use WASD or arrow keys to move
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
#include <sstream>
#include <fstream>
using namespace std;

// My camera implementation. I use an angle and sin/cos to turn towards an angle,
// add it to the current eye position and use the lookAt() method to turn.
// I also create a vector from look and eye, normalize it and then use it to move in that direction.
// Supports WASD and arrow keys
void cameraControlsGlobe(glm::vec3& eye, glm::vec3& look, float& angle, float& yangle) {
	float ysin = sin((yangle * 3.14) / 180);
	float xcos = cos((yangle * 3.14) / 180);
	float zcos = cos((yangle * 3.14) / 180);

	float xpos = cos((angle * 3.14) / 180);
	float zpos = sin((angle * 3.14) / 180);
	glm::vec3 vec = glm::normalize(look - eye);
	glm::vec3 yvec = { 0.0f, 0.02f, 0.0f };
	vec.x *= 0.02;
	vec.y *= 0.02;
	vec.z *= 0.02;

	int aState = glfwGetKey(window, GLFW_KEY_A);
	int leftState = glfwGetKey(window, GLFW_KEY_LEFT);
	if (aState == GLFW_PRESS || leftState == GLFW_PRESS) {
		angle -= 0.5f;
	}

	int dState = glfwGetKey(window, GLFW_KEY_D);
	int rightState = glfwGetKey(window, GLFW_KEY_RIGHT);
	if (dState == GLFW_PRESS || rightState == GLFW_PRESS) {
		angle += 0.5f;
	}

	int wState = glfwGetKey(window, GLFW_KEY_W);
	if (wState == GLFW_PRESS) {
		eye += vec;
	}

	int sState = glfwGetKey(window, GLFW_KEY_S);
	if (sState == GLFW_PRESS) {
		eye -= vec;
	}

	int upState = glfwGetKey(window, GLFW_KEY_UP);
	if (upState == GLFW_PRESS) {
		yangle += 0.5f;
	}

	int downState = glfwGetKey(window, GLFW_KEY_DOWN);
	if (downState == GLFW_PRESS) {
		yangle -= 0.5f;
	}

	look.y = eye.y + ysin;
	look.z = eye.z + (zpos * zcos);
	look.x = eye.x + (xpos * xcos);

	int spaceState = glfwGetKey(window, GLFW_KEY_SPACE);
	if (spaceState == GLFW_PRESS) {
		eye += yvec;
		look += yvec;
	}

	int lCtrlState = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);
	if (lCtrlState == GLFW_PRESS) {
		eye -= yvec;
		look -= yvec;
	}
}

// Structs for VertexData and TriData
struct VertexData {
	VertexData(float a, float b, float c) {
		x = a;
		y = b;
		z = c;
	}
	float x, y, z, nx, ny, nz, r, g, b, u, v;
};

struct TriData {
	TriData(float a, float b, float c) {
		v1 = a;
		v2 = b;
		v3 = c;
	}
	float v1, v2, v3;
};

// declare beforehand
void readPLYFile(string fname, vector<VertexData>& vertices, vector<TriData>& tris);
void loadARGB_BMP(const char* imagepath, unsigned char** data, unsigned int* width, unsigned int* height);

// TexturedMesh object for drawing the object
struct TexturedMesh {
	TexturedMesh(string ply, string bmp) {
		vector<VertexData> vertices;
		vector<TriData> triangles;
		readPLYFile(ply, vertices, triangles);

		// Load shader
		// Create the shaders
		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		std::string VertexShaderCode = "\
    	#version 330 core\n\
		// Input vertex data, different for all executions of this shader.\n\
		layout(location = 0) in vec3 vertexPosition;\n\
		layout(location = 1) in vec2 uv;\n\
		// Output data ; will be interpolated for each fragment.\n\
		out vec2 uv_out;\n\
		// Values that stay constant for the whole mesh.\n\
		uniform mat4 MVP;\n\
		void main(){ \n\
			// Output position of the vertex, in clip space : MVP * position\n\
			gl_Position =  MVP * vec4(vertexPosition,1);\n\
			// The color will be interpolated to produce the color of each fragment\n\
			uv_out = uv;\n\
		}\n";

		// Read the Fragment Shader code from the file
		std::string FragmentShaderCode = "\
		#version 330 core\n\
		in vec2 uv_out; \n\
		uniform sampler2D tex;\n\
		void main() {\n\
			gl_FragColor = texture(tex, uv_out);\n\
		}\n";
		char const* VertexSourcePointer = VertexShaderCode.c_str();
		glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
		glCompileShader(VertexShaderID);

		// Compile Fragment Shader
		char const* FragmentSourcePointer = FragmentShaderCode.c_str();
		glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
		glCompileShader(FragmentShaderID);

		shaderID = glCreateProgram();

		glAttachShader(shaderID, VertexShaderID);
		glAttachShader(shaderID, FragmentShaderID);
		glLinkProgram(shaderID);

		glDetachShader(shaderID, VertexShaderID);
		glDetachShader(shaderID, FragmentShaderID);

		matrixID = glGetUniformLocation(shaderID, "MVP");

		//glDeleteShader(VertexShaderID);
		//glDeleteShader(FragmentShaderID);

		// Load texture
		unsigned char* data;
		GLuint width, height;

		loadARGB_BMP(bmp.c_str(), &data, &width, &height);

		// Bind texture and store it into object
		glGenTextures(1, &bmpTex);
		glBindTexture(GL_TEXTURE_2D, bmpTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Go through vertices to create our VAOs, and VBOs for vertexes, UV, and index list
		// Get GLfloats for vertices, triangles, UV
		vector<GLfloat> vertexCoords;
		vector<GLfloat> uvCoords;
		for (int i = 0; i != vertices.size(); i++) {
			vertexCoords.push_back(vertices[i].x);
			vertexCoords.push_back(vertices[i].y);
			vertexCoords.push_back(vertices[i].z);

			uvCoords.push_back(vertices[i].u);
			uvCoords.push_back(vertices[i].v);
		}

		for (int i = 0; i != triangles.size(); i++) {
			triangleFaces.push_back(triangles[i].v1);
			triangleFaces.push_back(triangles[i].v2);
			triangleFaces.push_back(triangles[i].v3);
		}

		// Make our VAO, VBO stuff
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &vertexVBO);
		glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
		glBufferData(GL_ARRAY_BUFFER, vertexCoords.size() * sizeof(GLfloat), vertexCoords.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glGenBuffers(1, &textureVBO);
		glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
		glBufferData(GL_ARRAY_BUFFER, uvCoords.size() * sizeof(GLfloat), uvCoords.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glGenBuffers(1, &faceVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangleFaces.size() * sizeof(GLfloat), triangleFaces.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);
	}

	// Draw it
	void draw(glm::mat4 MVP) {
		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, bmpTex);

		glUseProgram(shaderID);
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, triangleFaces.size(), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	GLuint shaderID, matrixID, VAO, vertexVBO, textureVBO, faceVBO, bmpTex;
	vector<GLuint> triangleFaces;
};

// My implementaiton of reading the PLY files. Takes in vectors for VertedData and TriData
void readPLYFile(string fname, vector<VertexData>& vertices, vector<TriData>& tris) {
	cout << "Reading model " << fname << endl;
	ifstream file(fname);
	string currentLine;

	int lineSkip = 0;
	while (getline(file, currentLine)) {
		// Each file seems consistent with 15 lines to reach the coordinates, so let's skip the header stuff
		if (lineSkip < 15) {
			lineSkip++;
			continue;
		}

		// If the start has a '3', I know it is a triangle (for this file at least!)
		if (currentLine[0] == '3') {
			float v, x, y, z;
			istringstream stream(currentLine);
			while (stream >> v >> x >> y >> z) {
				TriData newTri(x, y, z);
				tris.push_back(newTri);
			}
		}
		// Read vertices, normals, uv, etc.
		else {
			// I will skip rgb, as none of the object files provided have rgb properties
			float x, y, z, nx, ny, nz, u, v;
			istringstream stream(currentLine);
			while (stream >> x >> y >> z >> nx >> ny >> nz >> u >> v) {
				VertexData newVertex(x, y, z);
				newVertex.nx = nx;
				newVertex.ny = ny;
				newVertex.nz = nz;
				newVertex.u = u;
				newVertex.v = v;
				vertices.push_back(newVertex);
			}
		}
	}
}

// Provided function for reading in the BMP files
void loadARGB_BMP(const char* imagepath, unsigned char** data, unsigned int* width, unsigned int* height) {

	printf("Reading image %s\n", imagepath);

	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGBA data

	// Open the file
	FILE* file = fopen(imagepath, "rb");
	if (!file) {
		printf("%s could not be opened. Are you in the right directory?\n", imagepath);
		getchar();
		return;
	}

	// Read the header, i.e. the 54 first bytes

	// If less than 54 bytes are read, problem
	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file1\n");
		fclose(file);
		return;
	}

	// Read the information about the image
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	*width = *(int*)&(header[0x12]);
	*height = *(int*)&(header[0x16]);
	// A BMP files always begins with "BM"
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file2\n");
		fclose(file);
		return;
	}
	// Make sure this is a 32bpp file
	if (*(int*)&(header[0x1E]) != 3) {
		printf("Not a correct BMP file3\n");
		fclose(file);
		return;
	}
	// fprintf(stderr, "header[0x1c]: %d\n", *(int*)&(header[0x1c]));
	// if ( *(int*)&(header[0x1C])!=32 ) {
	//     printf("Not a correct BMP file4\n");
	//     fclose(file);
	//     return;
	// }

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = (*width) * (*height) * 4; // 4 : one byte for each Red, Green, Blue, Alpha component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

	// Create a buffer
	*data = new unsigned char[imageSize];

	if (dataPos != 54) {
		fread(header, 1, dataPos - 54, file);
	}

	// Read the actual data from the file into the buffer
	fread(*data, 1, imageSize, file);

	// Everything is in memory now, the file can be closed.
	fclose(file);

}


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
	window = glfwCreateWindow(screenW, screenH, "The Legend of Zelda: Ocarina of Time (real!!!)", NULL, NULL);
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

	// Enable stuff
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	// Vector of objects, VAOTex stores the VAO and texture ID
	vector<TexturedMesh> objects;

	// Create all of our objects
	objects.push_back(TexturedMesh("./LinksHouse/Bottles.ply", "./LinksHouse/bottles.bmp"));
	objects.push_back(TexturedMesh("./LinksHouse/Floor.ply", "./LinksHouse/floor.bmp"));
	objects.push_back(TexturedMesh("./LinksHouse/Walls.ply", "./LinksHouse/walls.bmp"));
	objects.push_back(TexturedMesh("./LinksHouse/WoodObjects.ply", "./LinksHouse/woodobjects.bmp"));
	objects.push_back(TexturedMesh("./LinksHouse/Patio.ply", "./LinksHouse/patio.bmp"));
	objects.push_back(TexturedMesh("./LinksHouse/Table.ply", "./LinksHouse/table.bmp"));
	objects.push_back(TexturedMesh("./LinksHouse/WindowBG.ply", "./LinksHouse/windowbg.bmp"));
	objects.push_back(TexturedMesh("./LinksHouse/DoorBG.ply", "./LinksHouse/doorbg.bmp"));
	objects.push_back(TexturedMesh("./LinksHouse/MetalObjects.ply", "./LinksHouse/metalobjects.bmp"));
	objects.push_back(TexturedMesh("./LinksHouse/Curtains.ply", "./LinksHouse/curtains.bmp"));

	// Get a handle for our "MVP" uniform
	glm::mat4 MVP;

	glm::vec3 eye = { 0.5f, 0.4f, 0.5f };
	// glm::vec3 eye = {-5.0f, 2.0f, -5.0f};
	glm::vec3 up = { 0.0f, 1.0f, 0.0f };
	glm::vec3 center = { 0.0f, 0.0f, 0.0f };
	glm::vec3 look = eye;

	float angle = -90.0f;
	float yangle = 0.0f;

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {
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

		cameraControlsGlobe(eye, look, angle, yangle);
		glm::mat4 V = glm::lookAt(eye, look, up);

		glm::mat4 M = glm::mat4(1.0f);

		glm::mat4 MV = V * M;
		glLoadMatrixf(glm::value_ptr(V));

		MVP = Projection * V * M;

		// Draw all our objects
		for (int i = 0; i != objects.size(); i++) objects[i].draw(MVP);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup shader
	for (int i = 0; i != objects.size(); i++) glDeleteProgram(objects[i].shaderID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
