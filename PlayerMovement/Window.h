#pragma once

#include<stdio.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>


class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int intitialise();

	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
//	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getZoomChange();
	GLfloat getXpos() { return lastX; }
	GLfloat getYpos() { return lastY; }
	bool mouseLeftClick;

	void swapBuffers() { glfwSwapBuffers(mainWindow); }


	~Window();

private:

	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];
	GLfloat lastX, lastY, xChange, yChange,zoomChange;
	bool mousedFirstMoved;



	void createCallbacks();
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode); // static required for callback
	static void handleMouse(GLFWwindow* window, double xPos, double yPos); // static required for callback
	static void handleMouseClick(GLFWwindow* window, int button, int action, int mode);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

