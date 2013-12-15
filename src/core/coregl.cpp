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

	float high_fact = 30.f/255.f;
	float mid_fact = 12.f/255.f;
	float low_fact = 4.f/255.f;

	/*for(unsigned int i=0; i<volume_.xdim(); i++) {
		for(unsigned int j=0; j<volume_.ydim(); ++j) {
			for(unsigned int k=0; k<volume_.zdim(); ++k) {
				snoice_high = 0.5f + 0.5f * snoise3(float(i)*high_fact,float(j)*high_fact,float(k)*high_fact);
				snoice_mid = 0.5f + 0.5f * snoise3(float(i)*mid_fact,float(j)*mid_fact,float(k)*mid_fact);
				snoice_low = 0.5f + 0.5f * snoise3(float(i)*low_fact,float(j)*low_fact,float(k)*low_fact);
				snoice = 0.6f*snoice_low + 0.2f*snoice_mid + 0.2f*snoice_mid;
				volume_.setValueAt(snoice,i,j,k);
			}
		}
	}*/


	// Init sphere instead of just noise. sphere in center of volume
	float radius = 0.6f;//glm::min( glm::min(volume_.xdim(), volume_.ydim()), volume_.zdim()) / 2.f;
	glm::vec3 sphere_center(0.5,0.5,0.5);
	float val = 0.f;
	float snoice_max = FLT_MIN;

	for(unsigned int i=0; i<volume_.xdim(); ++i) {
		for(unsigned int j=0; j<volume_.ydim(); ++j) {
			for(unsigned int k=0; k<volume_.zdim(); ++k) {
				glm::vec3 voxel_pos(float(i)/float(volume_.xdim()), float(j)/float(volume_.ydim()), float(k)/float(volume_.zdim()));
				float dist = glm::length(sphere_center-voxel_pos);
				/*float dist = sqrt( (sphere_center.x-voxel_pos.x)*(sphere_center.x-voxel_pos.x)
					+(sphere_center.y-voxel_pos.y)*(sphere_center.y-voxel_pos.y)
					+(sphere_center.z-voxel_pos.z)*(sphere_center.z-voxel_pos.z));*/
				snoice_high = 0.5f + 0.5f * snoise3(float(i)*high_fact,float(j)*high_fact,float(k)*high_fact);
				snoice_mid = 0.5f + 0.5f * snoise3(float(i)*mid_fact,float(j)*mid_fact,float(k)*mid_fact);
				snoice_low = 0.5f + 0.5f * snoise3(float(i)*low_fact,float(j)*low_fact,float(k)*low_fact);
				snoice = 0.4f*snoice_low + 0.3f*snoice_mid + 0.3f*snoice_mid;

				if(snoice > snoice_max)
					snoice_max = snoice;

				float sign = radius - (dist+0.1*snoice);
				//val = glm::clamp(sign, 0.0f, 1.0f);
				sign = glm::max(sign, 0.0f);
				if(sign > 0.00001f) {
					//sign = 1.f;
				}

				val = sign;

				/*if(sign >= 0.f )
					val = 1.f;
				else
				{
					val = 0.4f;
				}*/
				volume_.setValueAt(val,i,j,k);
			}
		}
	}

	std::cout<<"Snoice max: "<<snoice_max<<std::endl;
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

	// Textur-data i &volumeData
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB32F, volume_.xdim(), volume_.ydim(), volume_.zdim(), 0, GL_RGB, 
		GL_FLOAT, volume_.getData());
	
	int samples = 256;
	float stepSize = 1.0f/GLfloat(samples);
	glUniform1i(glGetUniformLocation(id, "volumeData"), 0);
	glUniform1i(glGetUniformLocation(id, "samples"), samples);
	glUniform1f(glGetUniformLocation(id, "stepSize"), stepSize);
	glUniform1i(glGetUniformLocation(id, "screen_width"), kWidth);
	glUniform1i(glGetUniformLocation(id, "screen_height"), kHeight);
	printError("Init Volume Texture");
	
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

	//objectLoader.drawModels();
	printError("Core render5");

	glUseProgram(id);
	glUniform1i(glGetUniformLocation(id, "volumeTex"), 0);
	printError("Core render6");

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
	//objectLoader = ObjLoader();
	//objectLoader.loadObj(path, shaderManager.getId(ShaderManager::shaderId::BASIC));
	printError("LoadObj");
	std::cout << "Loading done.\n";
}
} // namespace core
