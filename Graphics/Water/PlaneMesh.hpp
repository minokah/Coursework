#include "shader.hpp"
#include "GeneralGraphics.h"


class PlaneMesh {

	std::vector<float> verts;
	std::vector<float> normals;
	std::vector<int> indices;

	float min, max, numVerts, numIndices;

	GLuint VAO, posVBO, normalVBO, faceVBO, textureVBO, dispTexVBO;

	GLuint MyShader = LoadShadersAll("WaterShader");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(MyShader, "MVP");
	GLuint timeID = glGetUniformLocation(MyShader, "time");
	GLuint VID = glGetUniformLocation(MyShader, "V");
	GLuint MID = glGetUniformLocation(MyShader, "M");
	GLuint LightPosID = glGetUniformLocation(MyShader, "lightpos");
	//GLuint ModelColorID = glGetUniformLocation(MyShader, "modelColor");

	void planeMeshQuads(float min, float max, float stepsize) {

		// The following coordinate system works as if (min, 0, min) is the origin
		// And then builds up the mesh from that origin down (in z)
		// and then to the right (in x).
		// So, one "row" of the mesh's vertices have a fixed x and increasing z

		//manually create a first column of vertices
		float x = min;
		float y = 0;
		for (float z = min; z <= max; z += stepsize) {
			verts.push_back(x);
			verts.push_back(y);
			verts.push_back(z);
			normals.push_back(0);
			normals.push_back(1);
			normals.push_back(0);
		}

		for (float x = min+stepsize; x <= max; x += stepsize) {
			for (float z = min; z <= max; z += stepsize) {
				verts.push_back(x);
				verts.push_back(y);
				verts.push_back(z);
				normals.push_back(0);
				normals.push_back(1);
				normals.push_back(0);
			}
		}

		int nCols = (max-min)/stepsize + 1;
		int i = 0, j = 0;
		for (float x = min; x < max; x += stepsize) {
			j = 0;
			for (float z = min; z < max; z += stepsize) {
				indices.push_back(i*nCols + j);
				indices.push_back(i*nCols + j + 1);
				indices.push_back((i+1)*nCols + j + 1);
				indices.push_back((i+1)*nCols + j);
				++j;
			}
			++i;
		}
	}

public:

	PlaneMesh(float min, float max, float stepsize) {
		this->min = min;
		this->max = max;
		glm::vec4 modelColor = glm::vec4(0, 1.0f, 1.0f, 1.0f);

		planeMeshQuads(min, max, stepsize);
		numVerts = verts.size()/3;
		numIndices = indices.size();

		// Load texture
		unsigned char* data;
		GLuint width, height;

		loadBMP("Assets/water.bmp", &data, &width, &height);

		// Bind texture and store it into object
		glGenTextures(1, &textureVBO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureVBO);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		free(data);

		// load displacement map
		loadBMP("Assets/displacement-map1.bmp", &data, &width, &height);

		// Bind texture and store it into object
		glGenTextures(1, &dispTexVBO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, dispTexVBO);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		//gen and fill buffers
		//vertex attribute pointers
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &posVBO);
		glBindBuffer(GL_ARRAY_BUFFER, posVBO);
		glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(GLfloat), verts.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		// Normals
		glGenBuffers(1, &normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glGenBuffers(1, &faceVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);

		//shaders and uniforms
	}

	void draw(glm::vec3 lightpos, glm::mat4 MVP, glm::mat4 V, glm::mat4 M, float time) {
		//Bind some stuff
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureVBO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, dispTexVBO);
		
		glUseProgram(MyShader);

		// Set up our shaders
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniform1i(glGetUniformLocation(MyShader, "waterTexture"), 0);
		glUniform1i(glGetUniformLocation(MyShader, "displacementTexture"), 1);

		// Give values to uniforms in shader
		glUniformMatrix4fv(MID, 1, GL_FALSE, &M[0][0]);
		glUniform1f(timeID, time);
		glUniformMatrix4fv(VID, 1, GL_FALSE, &V[0][0]);
		//glUniformMatrix4fv(MID, 1, GL_FALSE, &M[0][0]);
		glUniform3f(LightPosID, lightpos.x, lightpos.y, lightpos.z);

		// Bind and draw
		glBindVertexArray(VAO);
		glDrawElements(GL_PATCHES, indices.size(), GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);


		// Done, unbind stuff
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 1);

		glUseProgram(0);
	}


};
