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

	core::CoreGL gltest = core::CoreGL();
	gltest.te();
	
	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glClearColor(0., 0., 0., 0.);

    	// Keep running
    	glfwSwapBuffers(window);
        glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
