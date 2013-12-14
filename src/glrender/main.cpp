#define __USE_CORE_GL__
#include <coregl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stdlib.h> // Exit and status
#include "cam.h"
#include <test.h>
//#include <glincludes.h>

Camera *cam = NULL;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
	if (cam != NULL) {
		if (key == GLFW_KEY_W) {
			if (action == GLFW_PRESS)
				cam->holdingForward = true;
			if (action == GLFW_RELEASE)
				cam->holdingForward = false;
		}
		if (key == GLFW_KEY_S) {
			if (action == GLFW_PRESS)
				cam->holdingBackward = true;
			if (action == GLFW_RELEASE)
				cam->holdingBackward = false;
		}
	}
	if (key == GLFW_KEY_A) {
		if (action == GLFW_PRESS) cam->holdingLeftStrafe = true;
		if (action == GLFW_RELEASE) cam->holdingLeftStrafe = false;
	}
	if (key == GLFW_KEY_D) {
		if (action == GLFW_PRESS) cam->holdingRightStrafe = true;
		if (action == GLFW_RELEASE) cam->holdingRightStrafe = false;
	}
	//if (key == GLFW_KEY_A && action == GLFW_PRESS)
	//	cam.applyMovement(Camera::STRAFE_L);
	//if (key == GLFW_KEY_D && action == GLFW_PRESS)
	//	cam.applyMovement(Camera::STRAFE_R);
	//if (key == GLFW_KEY_X && action == GLFW_PRESS)
	//	cam.applyMovement(Camera::DOWN);
	//if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	//	cam.applyMovement(Camera::UP);
	TestClass t = TestClass();
}

int main(int argc, char **argv) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	
	GLFWwindow* window = glfwCreateWindow(640, 480, "Cumulus", NULL, NULL);
	
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	//Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
	}
	glGetError();
    int OpenGLmajor = 0, OpenGLminor = 0;

    OpenGLmajor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    OpenGLminor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);

	std::cout << "Running OpenGL version " << OpenGLmajor << "." << OpenGLminor << std::endl;
	
    if(OpenGLmajor < 3) {
		std::cout << "OpenGL version support is to damn low!" << std::endl;
        return false;
    }
	
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
    std::string path = "bunny_small.obj";
    core::CoreGL *cloud = core::CoreGL::creator(path);
	
	cam = new Camera(window, 640, 480);
	glfwMakeContextCurrent(window);
	float currentTime, lastTime = 0.0;
	float deltaTime = 0.01f;

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0., 0., 0., 0.);

		currentTime = float(glfwGetTime());
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		cam->move(deltaTime);

		cloud->render(cam->getModelView(), cam->getProjection());
    	// Keep running
		//cam->print();
        glfwPollEvents();
    	glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
    delete cloud;
	return 0;
}
