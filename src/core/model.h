#ifndef __MODEL_H__
#define __MODEL_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>

namespace core {
class Model {
public:
	~Model();
	static Model *creator(GLuint program, std::vector<glm::vec3> &v, std::vector<unsigned int> &i);
	unsigned int numIndices;
	unsigned int normalsCount;
	unsigned int numVerts;
	unsigned int numFaces;
	unsigned int texCount;
	GLuint vertAttribLoc;
	GLuint normAttribLoc;
	GLuint texAttribLoc;
	std::vector<glm::vec3> vertexArray;
	GLfloat *normalArray;
	GLfloat *texCoords;
	std::vector<unsigned int> indexArray;
	GLuint *normalsIndex;
	GLuint vao;
	static void generateNormals(Model *m);
protected:
	Model();
	

private:

};
} // namespace core

#endif //__MODEL_H__
