#include <gl/glew.h>
#include <gl/glfw3.h>
#include "Renderer.h"
#pragma once

class ApplicationManager
{
	bool MouseClicked;

	static double MouseXPos;
	static double MouseYPos;

	static int WindowSizeHeight;
	static int WindowSizeWidth;
	static int KeyPressed;

	GLFWwindow *window;
	Renderer renderer;

	bool InitalizeApplication();
	void InitalizeComponents();
	void WindowResize(int, int);
	void CloseApplication();
	static void SpecialKeyPressed(GLFWwindow*, int, int, int, int);
	static void MouseMoved(GLFWwindow*, double, double );
public:
	ApplicationManager();
	~ApplicationManager();
	void StartMainLoop();

};