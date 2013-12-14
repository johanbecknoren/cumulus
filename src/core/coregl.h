#ifndef __CORE_GL__
#define __CORE_GL__

#include "shadermanager.h"
#include "objloader.h"
#include "volume.h"
#ifdef __USE_CORE_GL__
#include "glincludes.h"
#endif

namespace core {

class CoreGL {

public:
	static CoreGL *creator(std::string path);    
	~CoreGL() {}
	void render(glm::mat4 trans, glm::mat4 proj);
	static void printError(const char *functionName);
	void setVolumeData();

private:
	void loadShaders();
	void initialize(std::string path);
    CoreGL();

	ShaderManager shaderManager;
	ObjLoader objectLoader;

	Volume volume_;
};

}

#endif
