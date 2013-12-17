#include "coregl.h"
#include "volume.h"
#include "simplexnoise1234.h"
//#include "fbo.h"

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
	shaderManager.loadShaders("color_cube.vert", "color_cube.frag", ShaderManager::shaderId::COLOR_CUBE);
	shaderManager.loadShaders("textureToScreen.vert", "textureToScreen.frag", ShaderManager::shaderId::TEX2SCREEN);
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
	GLuint texId = 3;
	GLuint id = shaderManager.getId(ShaderManager::shaderId::BASIC);
	glEnable (GL_BLEND);
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
	glUseProgram(id);
	glUniform1i(glGetUniformLocation(shaderManager.getId(ShaderManager::shaderId::BASIC), "volumeTex"), texId);
	printError("Init Volume Texture31");
	
}

void core::CoreGL::render(glm::mat4 trans, glm::mat4 proj) {
	glm::mat4 mvp = proj * trans;
	
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/*GLuint id = shaderManager.getId(ShaderManager::shaderId::BASIC);
	printError("Core render1");
	glUseProgram(id);
	printError("Core render2");
	
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
	printError("Core render7");*/

	
	GLboolean transposed = GL_FALSE;

	// Rendera colorcube här och spara i FBOer
	// Backface
/*	Fbo::useFbo(color_backface, 0L, 0L);*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glCullFace(GL_FRONT);
	glUseProgram(shaderManager.getId(ShaderManager::shaderId::COLOR_CUBE));
	glUniformMatrix4fv(
		glGetUniformLocation(shaderManager.getId(ShaderManager::shaderId::COLOR_CUBE), "camTrans"),
		1, transposed, glm::value_ptr(mvp));
	// Draw box w/ front face culling here
#ifdef __USE_FAST_OBJ__
	DrawModel(box);
#else
	DrawCube();
#endif
	glFlush();
/*	
	// Frontface
	Fbo::useFbo(color_frontface, 0L, 0L);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glCullFace(GL_BACK);
	glUseProgram(shaderManager.getId(ShaderManager::shaderId::COLOR_CUBE));
	glUniformMatrix4fv(
		glGetUniformLocation(shaderManager.getId(ShaderManager::shaderId::COLOR_CUBE), "camTrans"), 
		1, transposed, glm::value_ptr(mvp));
	// Draw box w/ back face culling here
#ifdef __USE_FAST_OBJ__
	DrawModel(box);
#else
	DrawCube();
#endif
	glFlush();

	Fbo::useFbo(final_image, color_frontface, color_backface);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glUseProgram(shaderManager.getId(ShaderManager::shaderId::BASIC));
	glUniform1i(glGetUniformLocation(shaderManager.getId(ShaderManager::shaderId::BASIC), "tex_frontface"), 0);
	glUniform1i(glGetUniformLocation(shaderManager.getId(ShaderManager::shaderId::BASIC), "tex_backface"), 1);
	glBindTexture(GL_TEXTURE_3D, 3);
	glUniform1i(glGetUniformLocation(shaderManager.getId(ShaderManager::shaderId::BASIC), "volumeTex"), 3);
	DrawModel(quad);
	glFlush();

	
	// Draw to viewport quad for debugging purpose only.
	Fbo::useFbo(0L,final_image,0L);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glUseProgram(shaderManager.getId(ShaderManager::shaderId::TEX2SCREEN));
	glUniform1i(glGetUniformLocation(shaderManager.getId(ShaderManager::shaderId::TEX2SCREEN), "texUnit"), 0);
	#ifdef __USE_FAST_OBJ__
	DrawModel(quad);
#else
	DrawQuad();
#endif
	glFlush();
	*/
	//GLuint in_texCoord = glGetAttribLocation(shaderManager.getId(ShaderManager::shaderId::TEX2SCREEN), "in_texCoord");
	//glBegin(GL_QUADS);
	//glVertexAttrib2f(in_texCoord, 0, 0);
	////glTexCoord2f(0.0,0.0);
	//glVertex3f(-1.0f,-1.0f, -1.0f);
	//glVertexAttrib2f(in_texCoord, 1, 0); 
	////glTexCoord2f(1.0,0.0);
	//glVertex3f( 1.0f,-1.0f, -1.0f);
	//glVertexAttrib2f(in_texCoord, 1, 1); 
	////glTexCoord2f(1.0,1.0);
	//glVertex3f( 1.0f, 1.0f, -1.0f);
	//glVertexAttrib2f(in_texCoord, 0, 1); 
	////glTexCoord2f(0.0,1.0);
	//glVertex3f(-1.0f, 1.0f, -1.0f);
	//glEnd();
	//printError("Tex2Screen");
	//glBegin(GL_QUADS);
	/*glTexCoord2f(0.0f,0.0f); glVertex3f(-1.0f,-1.0f, -1.0f);*/
	/*glTexCoord2f(1.0f,0.0f); glVertex3f( 1.0f,-1.0f, -1.0f);*/
	/*glTexCoord2f(1.0f,1.0f); glVertex3f( 1.0f, 1.0f, -1.0f);*/
	/*glTexCoord2f(0.0f,1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);*/
	//glEnd();
	
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	Fbo::useFbo(0L,0L,0L);
	glUseProgram(0);
}

core::CoreGL *core::CoreGL::creator(int w, int h) {
    core::CoreGL *instance = new core::CoreGL();
    instance->initialize(w, h);
    return instance;
}

void core::CoreGL::initialize(int w, int h) {
	//glfwInit();
	winWidth = w;
	winHeight = h;
	glewInit();
	
	printError("Pre Load");
    //std::cout << "Loading object " << path.c_str() << std::endl;

	loadShaders();
	printError("Load Shaders");

	setVolumeData();


	color_backface = new Fbo(winWidth, winHeight,0);
	color_frontface = new Fbo(winWidth, winHeight,0);
	maya_in_store = new Fbo(winWidth, winHeight, 0);
	final_image = new Fbo(winWidth, winHeight,0);
#ifdef __USE_FAST_OBJ__
	quad = LoadModelPlus(const_cast<char*>(fixPath("quad.obj").c_str()), shaderManager.getId(ShaderManager::shaderId::TEX2SCREEN), "in_Position", "in_Normal", "in_texCoord");
	box = LoadModelPlus(const_cast<char*>(fixPath("cube.obj").c_str()), shaderManager.getId(ShaderManager::shaderId::COLOR_CUBE), "in_Position", "in_Normal", "in_texCoord");
#endif
	std::cout << "Loading done.\n";
}
} // namespace core
