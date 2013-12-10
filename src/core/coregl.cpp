#include "coregl.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace core {

void CoreGL::loadShaders() {
	shaderManager = ShaderManager();
	shaderManager.loadShaders("basic.vert", "basic.frag", ShaderManager::shaderId::BASIC);
}

core::CoreGL::CoreGL() {
}

void core::CoreGL::render(glm::mat4 trans, glm::mat4 proj) {
	glm::mat4 mvp = proj * trans;
	GLuint matrixLoc = 0;
	// glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glUseProgram(shaderManager.getId(ShaderManager::shaderId::BASIC));
	objectLoader.drawModels();
}

core::CoreGL *core::CoreGL::creator(std::string path) {
    core::CoreGL *instance = new core::CoreGL();
    instance->initialize(path);
    return instance;
}

void core::CoreGL::initialize(std::string path) {
    std::cout << "Loading object " << path.c_str() << std::endl;
	loadShaders();
	objectLoader = ObjLoader();
	objectLoader.loadObj(path, shaderManager.getId(ShaderManager::shaderId::BASIC));

	std::cout << "Loading done.\n";
}
} // namespace core
