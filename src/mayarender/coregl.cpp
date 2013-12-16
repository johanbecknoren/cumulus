#include "coregl.h"
#include "volume.h"
#include "simplexnoise1234.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#ifndef __USE_CORE_GL__
#include "glincludes.h"
#endif

namespace core {

void CoreGL::printError(const char *functionName)
{
   GLenum error;
   while (( error = glGetError() ) != GL_NO_ERROR)
   {
	  //fprintf (stderr, "GL error 0x%X detected in %s\n", error, functionName);
   }
}

void CoreGL::loadShaders() {
	shaderManager = ShaderManager();
	shaderManager.loadShaders("basic.vert", "basic.frag", ShaderManager::shaderId::BASIC);
}

core::CoreGL::CoreGL() : volume_(256,256,256) {
}

// Arguments are voxel-aligned. Need to be translated to correct voxel only.
// No interpolation needed!!
// Assumes volume is a unit cube with corners in (0,0,0) and (1,1,1)
float CoreGL::getDensityAtWorld(float w_x, float w_y, float w_z, unsigned int dimx, unsigned int dimy, unsigned int dimz) {
	unsigned int i,j,k;

	// Transform world coord. to voxel grid points.
	// Will floor decimal values.
	i = unsigned int(w_x * dimx);
	j = unsigned int(w_y * dimy);
	k = unsigned int(w_z * dimz);

	float snoice_high = 0.f;
	float snoice_mid = 0.f;
	float snoice_low = 0.f;
	float snoice = 0.f;

	/*float high_fact = 30.f/255.f;
	float mid_fact = 12.f/255.f;
	float low_fact = 4.f/255.f;*/

	// Init sphere instead of just noise. sphere in center of volume
	float radius = 0.4f;

	glm::vec3 sphere_center(0.5f,0.5f,0.5f);
	float val = 0.f;
	float snoice_max = FLT_MIN;

	glm::vec3 voxel_pos(w_x, w_y, w_z);
	float dist = glm::length(sphere_center-voxel_pos);

	unsigned int num_octaves = 3;
	float gain = 0.5f;
	float freq = 4.f/255.f;

	for(unsigned int x=0; x<num_octaves; ++x) {
		snoice += gain * (0.5f + 0.5f*snoise3(float(i)*freq, float(j)*freq, float(k)*freq));
		gain *= 0.5;
		freq *= 4.f;
	}

	if(snoice > snoice_max)
		snoice_max = snoice;

	//snoice = 0.0f;
	float sign = radius - (dist+0.1f*snoice);
	//val = glm::clamp(sign, 0.0f, 1.0f);
	sign = glm::max(sign, 0.0f);

	/*if(sign >= 0.0f) {
		sign = 1.f;
	}
	else {
		sign = 0.2f;
	}*/

	sign = glm::smoothstep(0.0f, 0.05f, sign);
	val = sign;

	return val;
}

float CoreGL::getDensityAtVoxel(unsigned int i, unsigned int j, unsigned int k) {

	return CoreGL::getDensityAtWorld(float(i)/float(volume_.xdim()), float(j)/float(volume_.ydim()), float(k)/float(volume_.zdim()),
		volume_.xdim(), volume_.ydim(), volume_.zdim());
}

void CoreGL::setVolumeData() {

	float val;

	for(unsigned int i=0; i<volume_.xdim(); ++i) {
		for(unsigned int j=0; j<volume_.ydim(); ++j) {
			for(unsigned int k=0; k<volume_.zdim(); ++k) {
				val = getDensityAtVoxel(i,j,k);
				volume_.setValueAt(val,i,j,k);
			}
		}
	}

	std::cout<<"Volume max: "<<volume_.getMax()<<std::endl;

	initVolumeTexture();
}

void CoreGL::initVolumeTexture() {
	GLuint texId = 0;
	GLuint id = shaderManager.getId(ShaderManager::shaderId::BASIC);

	glEnable(GL_TEXTURE_3D);

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_3D, texId);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	printError("Init Volume Texture1");
	// Textur-data i &volumeData
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB32F, volume_.xdim(), volume_.ydim(), volume_.zdim(), 0, GL_RGB, 
		GL_FLOAT, volume_.getData());
	printError("Init Volume Texture2");

	glUniform1i(glGetUniformLocation(id, "volumeTex"), 0);
	printError("Init Volume Texture31");
	
}

void core::CoreGL::render(glm::mat4 trans, glm::mat4 proj) {
	glm::mat4 mvp = proj * trans;

	GLuint id = shaderManager.getId(ShaderManager::shaderId::BASIC);
	printError("Core render1");
	glUseProgram(id);
	printError("Core render2");
	GLboolean transposed = GL_FALSE;
	printError("Core render3");

	GLuint matrixLoc = glGetUniformLocation(id, "camTrans");
	printError("Core render4");
	glUniformMatrix4fv(
		matrixLoc, 1, 
		transposed, glm::value_ptr(mvp));

	glUseProgram(id);
	glUniformMatrix4fv(
		matrixLoc, 1, 
		transposed, glm::value_ptr(mvp));
	printError("Core render7");
	glBegin(GL_QUADS);
	glVertex3f(-1.0f,-1.0f, -1.0f);
	glVertex3f( 1.0f,-1.0f, -1.0f);
	glVertex3f( 1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();
	printError("Core render8");
}

core::CoreGL *core::CoreGL::creator(std::string path) {
    core::CoreGL *instance = new core::CoreGL();
    instance->initialize(path);
    return instance;
}

void core::CoreGL::initialize(std::string path) {
	printError("Pre Load");
    std::cout << "Loading object " << path.c_str() << std::endl;
	loadShaders();
	printError("Load Shaders");

	setVolumeData();

	//objectLoader = ObjLoader();
	//objectLoader.loadObj(path, shaderManager.getId(ShaderManager::shaderId::BASIC));
	//printError("LoadObj");
	std::cout << "Loading done.\n";
}
} // namespace core
