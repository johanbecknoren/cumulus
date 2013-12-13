#include "objloader.h"
#include <tinyobjloader/tiny_obj_loader.h>
#include <glm/ext.hpp>
#include <iostream>
#include <sstream>

namespace core {
void ObjLoader::loadObj(std::string path, unsigned int id) {
	std::stringstream fullPath(CMAKE_PROJECT_ROOT_DIR);
	fullPath << CMAKE_PROJECT_ROOT_DIR << "/models/" << path;

	std::vector<tinyobj::shape_t> shapes;
	std::vector<glm::vec3> verts;
	std::vector<unsigned int> indices;

	std::string err = tinyobj::LoadObj(shapes, fullPath.str().c_str());
	if (!err.empty()) {
		std::cerr << err << std::endl;
		return;
	}

	for (unsigned int n = 0, vertOffset = 0; n < shapes.size(); ++n) {
		unsigned int numFaces = (unsigned int) shapes[n].mesh.indices.size() / 3;
		unsigned int numVerts = (unsigned int) shapes[n].mesh.positions.size() / 3;

		for (unsigned int i = 0; i < numVerts; ++i) {
			verts.push_back(glm::vec3(shapes[n].mesh.positions[i * 3],
									  shapes[n].mesh.positions[i * 3 + 1],
									  shapes[n].mesh.positions[i * 3 + 2]));
		}
		for (unsigned int i = 0; i < numFaces; ++i) {
			// std::vector<unsigned int> val = std::vector<unsigned int>(3);
			for (unsigned int u = 0; u < 3; ++u) {
				// val[u] = shapes[n].mesh.indices[i * 3 + u + vertOffset];
				unsigned int v = shapes[n].mesh.indices[i * 3 + u + vertOffset];
				indices.push_back(v);
			}

			// glm::vec3 v0 = verts[val[0]];
			// glm::vec3 v1 = verts[val[1]];
			// glm::vec3 v2 = verts[val[2]];			
		}
		vertOffset += numVerts;
	}
	std::cout << "Found " << indices.size() / 3 << " faces and " << verts.size() << " verts\n";
	models.push_back(*Model::creator(id, verts, indices));
	std::cout << glm::to_string(models[0].vertexArray[0]) << std::endl;
}

void ObjLoader::drawModels() {
	std::vector<Model>::iterator it = models.begin();
	while(it != models.end()) {
		std::cout << "Drawing model" << it->numIndices << std::endl;
		glBindVertexArray((it)->vao);
		glDrawElements(GL_TRIANGLES, it->numIndices, GL_UNSIGNED_INT, 0L);
		++it;
	}
};

void ObjLoader::drawModelsWireFrame() {
	std::vector<Model>::iterator it = models.begin();
	while(it != models.end()) {
		glBindVertexArray((it)->vao);
		glDrawElements(GL_LINE_STRIP, it->numFaces * 3, GL_UNSIGNED_INT, 0L);
		++it;
	}
}

} // namespace core
