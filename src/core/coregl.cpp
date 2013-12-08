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
	shaderManager.loadShaders("basic.vert", "basic.frag", ShaderManager::shaderId::BASIC);
}

core::CoreGL::CoreGL() {
}

void core::CoreGL::render() {

}

core::CoreGL *core::CoreGL::creator(std::string path) {
    core::CoreGL *instance = new core::CoreGL();
    instance->initialize(path);
    return instance;
}

void core::CoreGL::initialize(std::string path) {
    std::cout << "Loading object " << path.c_str() << std::endl;
}
} // namespace core
