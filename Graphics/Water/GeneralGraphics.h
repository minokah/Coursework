#ifndef GENERAL_GRAPHICS_H
#define GENERAL_GRAPHICS_H

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

GLFWwindow* window;

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
	GLuint MyShader, MVPID, timeID, VAO, vertexVBO, textureVBO, faceVBO, bmpTex;
	vector<GLuint> triangleFaces;

	TexturedMesh(string ply, string bmp, string vs, string fs) {
		vector<VertexData> vertices;
		vector<TriData> triangles;
		readPLYFile(ply, vertices, triangles);

		MyShader = LoadShaders(vs.c_str(), fs.c_str());
		MVPID = glGetUniformLocation(MyShader, "MVP");
		timeID = glGetUniformLocation(MyShader, "time");

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
	}

	// Draw it
	void draw(glm::mat4 MVP, float time) {
		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, bmpTex);

		glUseProgram(MyShader);
		glUniformMatrix4fv(MVPID, 1, GL_FALSE, &MVP[0][0]);
		glUniform1f(timeID, time);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, triangleFaces.size(), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
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

void loadBMP(const char* imagepath, unsigned char** data, unsigned int* width, unsigned int* height) {

	// printf("Reading image %s\n", imagepath);

	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGB data

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
		printf("Not a correct BMP file\n");
		fclose(file);
		return;
	}
	// A BMP files always begins with "BM"
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		fclose(file);
		return;
	}
	// Make sure this is a 24bpp file
	if (*(int*)&(header[0x1E]) != 0) {
		printf("Not a correct BMP file\n");
		fclose(file);
		return;
	}
	if (*(int*)&(header[0x1C]) != 24) {
		printf("Not a correct BMP file\n");
		fclose(file);
		return;
	}

	// Read the information about the image
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	*width = *(int*)&(header[0x12]);
	*height = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = (*width) * (*height) * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

	// Create a buffer
	*data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(*data, 1, imageSize, file);

	//fprintf(stderr, "Done reading!\n");

	// Everything is in memory now, the file can be closed.
	fclose(file);

}

#endif