#include "model.h"

namespace core {

	Model::Model() {

	}
	
	Model *Model::creator(std::vector<glm::vec3> &v, std::vector<unsigned int> &i) {
		Model *model = new Model();
		model->numIndices = (unsigned int) i.size();
		model->numVerts = (unsigned int) v.size();
		model->numFaces = (unsigned int) i.size() / 3;
		
		return model;
	}


}; // namespace core
