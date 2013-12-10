#include <coregl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stdlib.h> // Exit and status
#include "camera.h"

Camera cam;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		cam.applyMovement(Camera::FORWARD);
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
		cam.applyMovement(Camera::BACKWARD);
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
    std::string path = "box.obj";
    core::CoreGL *cloud = core::CoreGL::creator(path);
	
	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glClearColor(0., 0., 0., 0.);
		cam.update();
		
		cloud->render(cam.getProjection(), cam.getView());
		
    	// Keep running
        glfwPollEvents();
    	glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
    delete cloud;
	return 0;
}
