#ifndef __UTILS_H__
#define __UTILS_H__

#include <glm/glm.hpp>

namespace core {

glm::mat4 carrToGlmM4(const double cmat[][4]) {
	glm::mat4 glmMat;
	for (int i = 0; i < 16; ++i) {
		int x = i % 4;
		int y = i / 4;
		glmMat[x][y] = (float) cmat[x][y];
	}
	return glmMat;
}

}

#endif