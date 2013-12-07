#ifndef __CORE_GL__
#define __CORE_GL__

#include "shadermanager.h"

namespace core {

class CoreGL {
public:
	CoreGL();
	~CoreGL() {}
	void te();
private:
	void loadShaders();
	ShaderManager shaderManager;
};

}

#endif
