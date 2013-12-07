#include "coregl.h"
#include <iostream>
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