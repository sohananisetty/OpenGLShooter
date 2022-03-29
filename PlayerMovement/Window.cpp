#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	xChange = 0.0f;
	yChange = 0.0f;
	mouseLeftClick = false;

	for (size_t i = 0; i < 1024; i++) {
		keys[i] = 0;
	}

}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	xChange = 0.0f;
	yChange = 0.0f;
	mouseLeftClick = false;

	for (size_t i = 0; i < 1024; i++) {
		keys[i] = 0;
	}
}

int Window::intitialise() {

	// Initialise GLFW
	if (!glfwInit())
	{
		printf("GLFW initialisation failed!");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core Profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow Forward Compatbility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the window
	mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get Buffer Size information
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);


	//Handle user input

	createCallbacks();
	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);
	// tell GLFW to capture our mouse
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD");
		return -1;
	}
	//if (glfwRawMouseMotionSupported())
	//	glfwSetInputMode(mainWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	/*GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	glfwSetCursor(mainWindow, cursor);*/

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);


	// Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(mainWindow, this); // to know which object called

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
	glfwSetMouseButtonCallback(mainWindow, handleMouseClick);
	glfwSetScrollCallback(mainWindow, scroll_callback);
	glfwSetFramebufferSizeCallback(mainWindow, framebuffer_size_callback);

}

GLfloat Window::getXChange() {
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;

}
GLfloat Window::getYChange() {
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}
GLfloat Window::getZoomChange() {
	return zoomChange;
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);

	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			theWindow->keys[key] = true;
		}
		if (action == GLFW_RELEASE) {
			theWindow->keys[key] = false;

		}



	}

}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mousedFirstMoved) {
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mousedFirstMoved = false;

	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;

}


void Window::handleMouseClick(GLFWwindow* window, int button, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	//if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	//{
	//	theWindow->mouseLeftClick = true;
	//}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		theWindow->mouseLeftClick = true;
	}

}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	theWindow->zoomChange = (float)yoffset;
	
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}

