#ifndef __CORE_GL__
#define __CORE_GL__

#include "shadermanager.h"
#include <string>
#include <glm/glm.hpp>

namespace core {

class CoreGL {

public:
	static CoreGL *creator(std::string path);    
	~CoreGL() {}
	void render(glm::mat4 trans, glm::mat4 proj);

private:
	void loadShaders();
	ShaderManager shaderManager;

    void initialize(std::string path);
    CoreGL();
};

}

#endif
