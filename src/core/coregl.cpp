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
	  fprintf (stderr, "GL error 0x%X detected in %s\n", error, functionName);
   }
}

void CoreGL::loadShaders() {
	shaderManager = ShaderManager();
	shaderManager.loadShaders("basic.vert", "basic.frag", ShaderManager::shaderId::BASIC);
}

core::CoreGL::CoreGL() : volume_(256,256,256) {
	setVolumeData();
}

void CoreGL::setVolumeData() {

	/*for(unsigned int i=0; i<volume_.xdim(); ++i) {
		for(unsigned int j=0; j<volume_.ydim(); ++j) {
			for(unsigned int k=0; k<volume_.zdim(); ++k) {
				volume_.setValueAt(1.f,i,j,k);//(float)rand()/((float)RAND_MAX), i,j,k);
			}
		}
	}*/

	// Write simplex noise instead of rand here
	float snoice_high = 0.f;
	float snoice_mid = 0.f;
	float snoice_low = 0.f;
	float snoice = 0.f;


	/*for(size_t i=0; i<volume_.xdim()*volume_.ydim()*volume_.zdim(); i+=3) {
		volume_.setValueAt(0.f, i);
		volume_.setValueAt(1.f, i+1);
		volume_.setValueAt(1.f, i+2);
	}*/

	/*for(size_t i=0; i<volume_.xdim()*volume_.ydim()*volume_.zdim(); i+=3) {
		volume_.setValueAt((float)rand()/((float)RAND_MAX), i);
		volume_.setValueAt((float)rand()/((float)RAND_MAX), i+1);
		volume_.setValueAt((float)rand()/((float)RAND_MAX), i+2);
	}*/

	float high_fact = 12.f/255.f;
	float mid_fact = 4.f/255.f;
	float low_fact = 1.f/255.f;

	for(unsigned int i=0; i<volume_.xdim()*3; i+=3) {
		for(unsigned int j=0; j<volume_.ydim(); ++j) {
			for(unsigned int k=0; k<volume_.zdim(); ++k) {
				snoice_high = 0.5f + 0.5f * snoise3(float(i)*high_fact,float(j)*high_fact,float(k)*high_fact);
				snoice_mid = 0.5f + 0.5f * snoise3(float(i)*mid_fact,float(j)*mid_fact,float(k)*mid_fact);
				snoice_low = 0.5f + 0.5f * snoise3(float(i)*low_fact,float(j)*low_fact,float(k)*low_fact);
				snoice = 0.6f*snoice_low + 0.2f*snoice_mid + 0.2f*snoice_mid;
				volume_.setValueAt(snoice,i,j,k);
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
	/*glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);*/

	// Textur-data i &volumeData
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB32F, volume_.xdim(), volume_.ydim(), volume_.zdim(), 0, GL_RGB, 
		GL_FLOAT, volume_.getData());
	
	int samples = 256;
	float stepSize = 1.0f/GLfloat(samples);
	glUniform1i(glGetUniformLocation(id, "volumeData"), 0);
	glUniform1i(glGetUniformLocation(id, "samples"), samples);
	glUniform1f(glGetUniformLocation(id, "stepSize"), stepSize);
	
}

void core::CoreGL::render(glm::mat4 trans, glm::mat4 proj) {
	glm::mat4 mvp = proj * trans;

	//GLuint id = shaderManager.getId(ShaderManager::shaderId::BASIC);
	//glUseProgram(id);
	//GLboolean transposed = GL_FALSE;

	//GLuint matrixLoc = glGetUniformLocation(id, "camTrans");

	//glUniformMatrix4fv(
	//	matrixLoc, 1, 
	//	transposed, glm::value_ptr(mvp));

	//objectLoader.drawModels();
	//printError("Core render");

	//glUseProgram(id);
	//glUniform1i(glGetUniformLocation(id, "volumeTex"), 0);

	//glUseProgram(id);

	//glUniformMatrix4fv(
	//	matrixLoc, 1, 
	//	transposed, glm::value_ptr(mvp));

	//glBegin(GL_QUADS);
	//glVertex3f(-1.0f,-1.0f, -1.0f);
	//glVertex3f( 1.0f,-1.0f, -1.0f);
	//glVertex3f( 1.0f, 1.0f, -1.0f);
	//glVertex3f(-1.0f, 1.0f, -1.0f);
	//glEnd();
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
	//objectLoader = ObjLoader();
	//objectLoader.loadObj(path, shaderManager.getId(ShaderManager::shaderId::BASIC));
	printError("LoadObj");
	std::cout << "Loading done.\n";
}
} // namespace core
