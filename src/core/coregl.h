#ifndef __CORE_GL__
#define __CORE_GL__

#include "shadermanager.h"
//#include "objloader.h"
#include "loadobj.h"

#include <glm/glm.hpp>
#include <string>
#include "volume.h"
#include "utils.h"
#include "fbo.h"
//#include "model.h"

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
	void initVolumeTexture();
	float getDensityAtVoxel(unsigned int i, unsigned int j, unsigned int k);
	static float getDensityAtWorld(float w_x, float w_y, float w_z, unsigned int dimx, unsigned int dimy, unsigned int dimz);

private:
	void loadShaders();
	void initialize(std::string path);
    CoreGL();

	ShaderManager shaderManager;
	//ObjLoader objectLoader;

	Volume volume_;

	Fbo *color_backface;
	Fbo *color_frontface;
	Fbo *final_image;

	Model *quad;
	Model *box;
};

}

#endif
