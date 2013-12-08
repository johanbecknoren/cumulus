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
	static Model creator(std::vector<glm::vec3> v, std::vector<unsigned int> i);
	
private:
	Model();
	
};
} // namespace core

#endif //__MODEL_H__
