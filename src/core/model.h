#ifndef __MODEL_H__
#define __MODEL_H__

#include <GL/glew.h>
#include <GL/glfw.h>
#include <vector>
#include <glm/glm.hpp>

namespace core {
class Model {
public:
	~Model();
	static Model *creator(std::vector<glm::vec3> &v, std::vector<unsigned int> &i);
	unsigned int numIndices;
	unsigned int normalsCount;
	unsigned int numVerts;
	unsigned int numFaces;
	unsigned int texCount;
	GLuint vertAttribLoc;
	GLuint normAttribLoc;
	GLuint texAttribLoc;
	GLfloat *vertexArray;
	GLfloat *normalArray;
	GLfloat *texCoords;
	GLuint *indexArray;
	GLuint *normalsIndex;
	GLuint vao;

protected:
	Model();
	static void generateNormals(Model *m);

private:

};
} // namespace core

#endif //__MODEL_H__
