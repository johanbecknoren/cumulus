#include <GL/glew.h>
#include <coregl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stdlib.h> // Exit and status



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
    std::string path = "box.obj";
    core::CoreGL *gltest = core::CoreGL::creator(path);
	
	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glClearColor(0., 0., 0., 0.);
		glm::mat4 projection =
			glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
		// glm::mat4 transform = 
		gltest->render(projection, glm::mat4(1.0f));
		
    	// Keep running
    	glfwSwapBuffers(window);
        glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
    delete gltest;
	return 0;
}
