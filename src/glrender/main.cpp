#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h> // Exit and status
#include <coregl.h>

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(int argc, char **argv) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	
	GLFWwindow* window = glfwCreateWindow(640, 480, "Cumulus", NULL, NULL);
	
	glfwSetKeyCallback(window, key_callback);
	
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
    std::string path = "bunny.obj";
    core::CoreGL *gltest = core::CoreGL::creator(path);
	
	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window))
	{
    	// Keep running
    	glfwSwapBuffers(window);
        glfwPollEvents();
        gltest->render();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
    delete gltest;
	return 0;
}
