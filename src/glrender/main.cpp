#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h> // Exit and status

int main(int argc, char **argv) {
	std::cout << "hej\n";
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwTerminate();

	return 0;
}