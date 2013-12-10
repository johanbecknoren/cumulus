#ifndef _SHADERMANAGER_H_
#define _SHADERMANAGER_H_

#ifdef WIN32
#include <Windows.h>
#endif
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <array>

namespace core {
class ShaderManager {
public:
	ShaderManager();
	~ShaderManager() {
	};
	
	enum shaderId {
		BASIC = 0
	};
	GLuint getId(shaderId id);

	bool loadShaders(std::string vertFileName, std::string fragFileName, shaderId id);
	bool loadShadersG(std::string vertFileName, std::string fragFileName, std::string geomFileName, shaderId id);	

private:
	GLuint loadShader(const char *vertFileName, const char *fragFileName);
	GLuint loadShaderG(const char *vertFileName, const char *fragFileName, const char *geomFileName);
	char* readFile(char *file);
	void printShaderInfoLog(GLuint obj, const char *fn);
	void printProgramInfoLog(GLuint obj, const char *vfn, const char *ffn, const char *gfn);
	GLuint compileShaders(const char *vs, const char *fs, const char *gs, const char *vfn, const char *ffn, const char *gfn);
	
	static const int numShaders = 1;
	std::array<GLuint, numShaders> shaders;
	std::string fixPath(std::string in);
};
} //namespace core

#endif
