#include "coregl.h"
#include "objloader.h"

#include <iostream>
namespace core {
CoreGL::CoreGL() {
	te();
}

void CoreGL::te() {
	std::cout << "testing testing" << std::endl;
	ObjLoader objectLoader = ObjLoader();
	objectLoader.loadObj("bunny.obj");
}

void CoreGL::loadShaders() {
	shaderManager = ShaderManager();
	shaderManager.loadShaders("", "", ShaderManager::shaderId::BASIC);
}

} // namespace core
