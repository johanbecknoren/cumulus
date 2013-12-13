#ifndef __CORE_GL__
#define __CORE_GL__

#include "shadermanager.h"
#include "objloader.h"
#include "glincludes.h"

namespace core {

class CoreGL {

public:
	static CoreGL *creator(std::string path);    
	~CoreGL() {}
	void render(glm::mat4 trans, glm::mat4 proj);
	static void printError(const char *functionName);

private:
	void loadShaders();
	void initialize(std::string path);
    CoreGL();

	ShaderManager shaderManager;
	ObjLoader objectLoader;
};

}

#endif
