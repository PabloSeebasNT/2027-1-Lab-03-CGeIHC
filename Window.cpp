#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 2.0f;
	articulacion1 = 0.0f;
	articulacion2 = 0.0f;
	articulacion3 = 0.0f;
	articulacion4 = 0.0f;
	articulacion5 = 0.0f;
	articulacion6 = 0.0f;
	articulacion7 = 0.0f;
	articulacion8 = 0.0f;
	articulacion9 = 0.0f;
	articulacion10 = 0.0f;
	articulacion11 = 0.0f;
	articulacion12 = 0.0f;
	articulacion13 = 0.0f;
	articulacion14 = 0.0f;
	articulacion15 = 0.0f;
	articulacion16 = 0.0f;
	articulacion17 = 0.0f;
	articulacion18 = 0.0f;
	articulacion19 = 0.0f;
	articulacion20 = 0.0f;
	articulacion21 = 0.0f;
	articulacion22 = 0.0f;
	articulacion23 = 0.0f;
	articulacion24 = 0.0f;
	articulacion25 = 0.0f;
	articulacion26 = 0.0f;
	articulacion27 = 0.0f;
	mueveyavion = 2.0f;
	muevexavion = 2.0f;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "PracticaXX:Nombre de la practica", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_Y)
	{
		theWindow-> muevex += 1.0;
		theWindow->articulacion7 += 10.0f;
		theWindow->articulacion9 += 10.0f;
		theWindow->articulacion11 += 10.0f;
		theWindow->articulacion13 += 10.0f;
		theWindow->articulacion16 += 10.0f;
		theWindow->articulacion19 += 10.0f;
	}
	if (key == GLFW_KEY_U)
	{
		theWindow-> muevex -= 1.0;
		theWindow->articulacion7 -= 10.0f;
		theWindow->articulacion9 -= 10.0f;
		theWindow->articulacion11 -= 10.0f;
		theWindow->articulacion13 -= 10.0f;
		theWindow->articulacion16 -= 10.0f;
		theWindow->articulacion19 -= 10.0f;
	}


	if (key == GLFW_KEY_Q)
	{
		theWindow->articulacion1 += 10.0f;
	}

	// Y, U, I, O (Movimientos / Articulaciones) 
	if (key == GLFW_KEY_1 && (mode & GLFW_MOD_SHIFT))
	{
		theWindow->articulacion2 += 10.0f;
	}
	if (key == GLFW_KEY_2 && (mode & GLFW_MOD_SHIFT))
	{
		theWindow->articulacion3 += 10.0f;
	}
	if (key == GLFW_KEY_I && !(mode & GLFW_MOD_SHIFT))
	{
		theWindow->articulacion4 += 10.0f;
	}
	if (key == GLFW_KEY_O && !(mode & GLFW_MOD_SHIFT))
	{
		theWindow->articulacion5 += 10.0f;
	}

	// F y G 
	if (key == GLFW_KEY_F)
	{
		theWindow->articulacion6 -= 2.0f;
		if (theWindow->articulacion6 < -45.0f)
		{
			theWindow->articulacion6 = -45.0f;
		}
	}

	if (key == GLFW_KEY_G)
	{
		theWindow->articulacion6 += 2.0f;
		if (theWindow->articulacion6 > 45.0f)
		{
			theWindow->articulacion6 = 45.0f;
		}
	}

	if (key == GLFW_KEY_H)
	{
		theWindow->articulacion7 += 10.0f;
	}

	//  J y K 
	if (key == GLFW_KEY_J)
	{
		theWindow->articulacion8 -= 2.0f;
		if (theWindow->articulacion8 < -45.0f)
		{
			theWindow->articulacion8 = -45.0f;
		}
	}

	if (key == GLFW_KEY_K)
	{
		theWindow->articulacion8 += 2.0f;
		if (theWindow->articulacion8 > 45.0f)
		{
			theWindow->articulacion8 = 45.0f;
		}
	}

	if (key == GLFW_KEY_L)
	{
		theWindow->articulacion9 += 10.0f;
	}

	// Z y X (Articulación 10) 
	if (key == GLFW_KEY_Z && !(mode & GLFW_MOD_SHIFT))
	{
		theWindow->articulacion10 -= 2.0f;
		if (theWindow->articulacion10 < -45.0f)
		{
			theWindow->articulacion10 = -45.0f;
		}
	}

	if (key == GLFW_KEY_X && !(mode & GLFW_MOD_SHIFT))
	{
		theWindow->articulacion10 += 2.0f;
		if (theWindow->articulacion10 > 45.0f)
		{
			theWindow->articulacion10 = 45.0f;
		}
	}

	if (key == GLFW_KEY_C && !(mode & GLFW_MOD_SHIFT))
	{
		theWindow->articulacion11 += 10.0f;
	}

	// V y B 
	if (key == GLFW_KEY_V)
	{
		theWindow->articulacion12 -= 2.0f;
		if (theWindow->articulacion12 < -45.0f)
		{
			theWindow->articulacion12 = -45.0f;
		}
	}

	if (key == GLFW_KEY_B)
	{
		theWindow->articulacion12 += 2.0f;
		if (theWindow->articulacion12 > 45.0f)
		{
			theWindow->articulacion12 = 45.0f;
		}
	}

	if (key == GLFW_KEY_N)
	{
		theWindow->articulacion13 += 10.0f;
	}

	// M y P 
	if (key == GLFW_KEY_M)
	{
		theWindow->articulacion14 -= 2.0f;
		if (theWindow->articulacion14 < -45.0f)
		{
			theWindow->articulacion14 = -45.0f;
		}
	}

	if (key == GLFW_KEY_P)
	{
		theWindow->articulacion14 += 2.0f;
		if (theWindow->articulacion14 > 45.0f)
		{
			theWindow->articulacion14 = 45.0f;
		}
	}

	// NÚMEROS (7, 8, 9) 
	if (key == GLFW_KEY_7 && !(mode & GLFW_MOD_SHIFT))
	{
		theWindow->articulacion15 -= 2.0f;
		if (theWindow->articulacion15 < -45.0f)
		{
			theWindow->articulacion15 = -45.0f;
		}
	}

	if (key == GLFW_KEY_8 && !(mode & GLFW_MOD_SHIFT))
	{
		theWindow->articulacion15 += 2.0f;
		if (theWindow->articulacion15 > 45.0f)
		{
			theWindow->articulacion15 = 45.0f;
		}
	}

	if (key == GLFW_KEY_9 && !(mode & GLFW_MOD_SHIFT))
	{
		theWindow->articulacion16 += 10.0f;
	}

	// NÚMEROS (1, 2, 3, 4, 5) 
	if (key == GLFW_KEY_1 && !(mode & GLFW_MOD_SHIFT))
	{
		theWindow->articulacion17 -= 2.0f;
		if (theWindow->articulacion17 < -45.0f)
		{
			theWindow->articulacion17 = -45.0f;
		}
	}

	if (key == GLFW_KEY_2 && !(mode & GLFW_MOD_SHIFT))
	{
		theWindow->articulacion17 += 2.0f;
		if (theWindow->articulacion17 > 45.0f)
		{
			theWindow->articulacion17 = 45.0f;
		}
	}

	if (key == GLFW_KEY_3 && !(mode & GLFW_MOD_SHIFT))
	{
		theWindow->articulacion18 -= 2.0f;
		if (theWindow->articulacion18 < -45.0f)
		{
			theWindow->articulacion18 = -45.0f;
		}
	}

	if (key == GLFW_KEY_4 && !(mode & GLFW_MOD_SHIFT))
	{
		theWindow->articulacion18 += 2.0f;
		if (theWindow->articulacion18 > 45.0f)
		{
			theWindow->articulacion18 = 45.0f;
		}
	}

	if (key == GLFW_KEY_5 && !(mode & GLFW_MOD_SHIFT))
	{
		theWindow->articulacion19 += 10.0f;
	}

	if (key == GLFW_KEY_6 && !(mode & GLFW_MOD_SHIFT))
	{
		theWindow->mueveyavion += 1.0;
	}
	if (key == GLFW_KEY_7 && !(mode & GLFW_MOD_SHIFT))
	{
		theWindow->mueveyavion -= 1.0;
	}
	if (key == GLFW_KEY_8 && !(mode & GLFW_MOD_SHIFT))
	{
		theWindow->muevexavion += 1.0;
	}
	if (key == GLFW_KEY_9 && !(mode & GLFW_MOD_SHIFT))
	{
		theWindow->muevexavion -= 1.0;
	}
	





	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
