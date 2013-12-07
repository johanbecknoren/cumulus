#ifndef _SHADERMANAGER_H_
#define _SHADERMANAGER_H_

extern "C" {
#ifdef WIN32
#include <Windows.h>
#endif
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace core {
class ShaderManager {
public:
	ShaderManager();
	~ShaderManager() {
		delete shaders;
	};
	
	enum shaderId {
		BASIC = 0
	};
	GLuint getId(shaderId id);

	bool loadShaders(const char *vertFileName, const char *fragFileName, shaderId id);
	bool loadShadersG(const char *vertFileName, const char *fragFileName, const char *geomFileName, shaderId id);	

private:
	GLuint loadShader(const char *vertFileName, const char *fragFileName);
	GLuint loadShaderG(const char *vertFileName, const char *fragFileName, const char *geomFileName);
	char* readFile(char *file);
	void printShaderInfoLog(GLuint obj, const char *fn);
	void printProgramInfoLog(GLuint obj, const char *vfn, const char *ffn, const char *gfn);
	GLuint compileShaders(const char *vs, const char *fs, const char *gs, const char *vfn, const char *ffn, const char *gfn);
	GLuint *shaders;
	static const int numShaders = 1;
};
//const int ShaderManager::numShaders = 1;
} //namespace core
}
#endif
