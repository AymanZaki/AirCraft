#include "ApplicationManager.h"

//static members initalization.
int ApplicationManager::KeyPressed = -1;
double ApplicationManager::MouseXPos = -1.0;
double ApplicationManager::MouseYPos = -1.0;
int ApplicationManager::WindowSizeWidth = 0;
int ApplicationManager::WindowSizeHeight = 0;

ApplicationManager::ApplicationManager()
{
	InitalizeApplication();
}


ApplicationManager::~ApplicationManager()
{
	delete window;
}

void ApplicationManager::SpecialKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//if (action == GLFW_PRESS)
		KeyPressed = key;
}

bool ApplicationManager::InitalizeApplication()
{
	InitalizeComponents();
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //OpenGL version 3.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //If requesting an OpenGL version below 3.2, GLFW_OPENGL_ANY_PROFILE

	WindowResize(WindowSizeWidth, WindowSizeHeight);

	if (window == NULL){
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	// ******************** Initialize GLEW ******************** //
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return false;
	}

	glfwSetKeyCallback(window, &this->SpecialKeyPressed);
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);	

	MouseXPos = WindowSizeWidth / 2;
	MouseYPos = WindowSizeHeight / 2;
	glfwSetCursorPos(window, MouseXPos, MouseYPos);
	glfwSetCursorPosCallback(window, &this->MouseMoved);

	renderer.Initialize();
}

void ApplicationManager::InitalizeComponents()
{
	KeyPressed = -1;
	MouseClicked = false;
	WindowSizeHeight = 768;
	WindowSizeWidth = 1024;
}

void ApplicationManager::StartMainLoop()
{
	do{ // Loop

		if (KeyPressed != -1)
		{
			renderer.HandleKeyboardInput(KeyPressed);
			KeyPressed = -1;
		}

		if (ApplicationManager::MouseXPos != ApplicationManager::WindowSizeWidth / 2
			|| ApplicationManager::MouseYPos != ApplicationManager::WindowSizeHeight / 2)
		{
			double mouseSpeed = 0.05; //it is just there to speed up or slow down the movements.
			double movedDistanceX;
			double movedDistanceY;

			//  the distance (old position - new position)
			// in other words:  how far is the mouse from the center of the window ? The bigger this value, the more we want to turn.
			// note that the old position (x_old, y_old) is fixed in this application (x_old = WindoSizeWidth/2, y_old = WindowSizeHeight/2)
			movedDistanceX = double(WindowSizeWidth / 2 - MouseXPos)*mouseSpeed;
			movedDistanceY = double(WindowSizeHeight / 2 - MouseYPos)*mouseSpeed;

			// Pass the two distances to the Renderer (our drawings)
			renderer.HandleMouse(movedDistanceX, movedDistanceY);

			//Force the new position of the mouse to be in the middle of the window
			MouseXPos = WindowSizeWidth / 2;
			MouseYPos = WindowSizeHeight / 2;
			glfwSetCursorPos(window, MouseXPos, MouseYPos);
		}

		renderer.Draw();
		renderer.Update();

		// Swap buffers
		glfwSwapBuffers(window); //Displaying our finished scene
		glfwPollEvents(); //try to comment it

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&	glfwWindowShouldClose(window) == 0);
}

void ApplicationManager::MouseMoved(GLFWwindow* window, double xpos, double ypos)
{
	//set the new mouse position to the MousXPos and MousYPos
	MouseXPos = xpos;
	MouseYPos = ypos;
}


void ApplicationManager::WindowResize(int Width, int Height)
{
	window = glfwCreateWindow(Width, Height, "Aircraft", NULL, NULL);
}

void ApplicationManager::CloseApplication()
{
	glfwTerminate();
}