#ifndef __CORE_GL__
#define __CORE_GL__

#include "shadermanager.h"
#include <string>

namespace core {

class CoreGL {

public:
	static CoreGL *creator(std::string path);    
	~CoreGL() {}
	void render();

private:
	void loadShaders();
	ShaderManager shaderManager;

    void initialize(std::string path);
    CoreGL();
};

}

#endif
