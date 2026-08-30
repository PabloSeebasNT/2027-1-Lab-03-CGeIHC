#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>
#include <windows.h>
#include <random>
#include <time.h> 

// Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 800;
GLuint VAO, VBO, shader;
float red = 1.0f, green = 0.0f, blue = 0.0f;

// LENGUAJE DE SHADER (SOMBRAS) GLSL
static const char* vShader = "						\n\
#version 330										\n\
layout (location = 0) in vec3 pos;					\n\
void main()											\n\
{													\n\
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f); 	\n\
}";

static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
    color = vec4(0.4f, 0.4f, 0.4f, 0.4f);	 		\n\
}";

void CrearTriangulo() {
	GLfloat vertices[] = {
		//PARA INICIALES SNT
		

		// PARA S:
				// Barra superior
				-0.75f,  0.35f, 0.0f,   -0.35f,  0.35f, 0.0f,  -0.75f,  0.20f, 0.0f,
				-0.35f,  0.35f, 0.0f,   -0.35f,  0.20f, 0.0f,  -0.75f,  0.20f, 0.0f,
				// Bajada superior izquierda
				-0.75f,  0.20f, 0.0f,   -0.60f,  0.20f, 0.0f,  -0.75f,  0.05f, 0.0f,
				-0.60f,  0.20f, 0.0f,   -0.60f,  0.05f, 0.0f,  -0.75f,  0.05f, 0.0f,
				// Barra central
				-0.75f,  0.05f, 0.0f,   -0.35f,  0.05f, 0.0f,  -0.75f, -0.10f, 0.0f,
				-0.35f,  0.05f, 0.0f,   -0.35f, -0.10f, 0.0f,  -0.75f, -0.10f, 0.0f,
				// Bajada inferior derecha
				-0.50f, -0.10f, 0.0f,   -0.35f, -0.10f, 0.0f,  -0.50f, -0.25f, 0.0f,
				-0.35f, -0.10f, 0.0f,   -0.35f, -0.25f, 0.0f,  -0.50f, -0.25f, 0.0f,
				// Barra inferior
				-0.75f, -0.25f, 0.0f,   -0.35f, -0.25f, 0.0f,  -0.75f, -0.40f, 0.0f,
				-0.35f, -0.25f, 0.0f,   -0.35f, -0.40f, 0.0f,  -0.75f, -0.40f, 0.0f,

		// PARA N:
				// Barra izquierda
				-0.17f,  0.35f, 0.0f,   -0.07f,  0.35f, 0.0f,  -0.17f, -0.40f, 0.0f,
				-0.07f,  0.35f, 0.0f,   -0.07f, -0.40f, 0.0f,  -0.17f, -0.40f, 0.0f,
				// Barra diagonal del centro
				-0.10f,  0.35f, 0.0f,    0.10f, -0.40f, 0.0f,   0.00f,  0.35f, 0.0f,
				0.10f, -0.40f, 0.0f,  -0.10f,  0.35f, 0.0f,   0.00f, -0.40f, 0.0f,
				// Barra derecha
				0.07f,  0.35f, 0.0f,    0.17f,  0.35f, 0.0f,   0.07f, -0.40f, 0.0f,
				0.17f,  0.35f, 0.0f,    0.17f, -0.40f, 0.0f, 0.07f, -0.40f, 0.0f,


		// PARA T:
			    // Barra horizontal superior
				0.35f,  0.35f, 0.0f,    0.75f,  0.35f, 0.0f,   0.35f,  0.20f, 0.0f,
				0.75f,  0.35f, 0.0f,    0.75f,  0.20f, 0.0f,   0.35f,  0.20f, 0.0f,
				// Barra vertical central
				0.48f,  0.20f, 0.0f,    0.62f,  0.20f, 0.0f,   0.48f, -0.40f, 0.0f,
				0.62f,  0.20f, 0.0f,    0.62f, -0.40f, 0.0f,   0.48f, -0.40f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
	GLuint theShader = glCreateShader(shaderType);
	const GLchar* theCode[1];
	theCode[0] = shaderCode;
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);
	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error al compilar el shader %d es: %s \n", shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);
}

void CompileShaders() {
	shader = glCreateProgram();
	if (!shader) {
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);

	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
}

int main() {
	if (!glfwInit()) {
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Practica 1: Introduccion a OpenGL", NULL, NULL);
	if (!mainWindow) {
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}

	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);
	glfwMakeContextCurrent(mainWindow);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glViewport(0, 0, BufferWidth, BufferHeight);
	CompileShaders();
	CrearTriangulo();

	printf("Version de Opengl: %s \n", glGetString(GL_VERSION));

	srand((unsigned int)time(NULL)); 

	
	double ultimoTiempo = glfwGetTime();
	float r = (float)rand() / RAND_MAX;
	float g = (float)rand() / RAND_MAX;
	float b = (float)rand() / RAND_MAX;
	

	while (!glfwWindowShouldClose(mainWindow)) {
		glfwPollEvents();

		double tiempoActual = glfwGetTime();

		if (tiempoActual - ultimoTiempo >= 2.0) {
			r = (float)rand() / RAND_MAX; 
			g = (float)rand() / RAND_MAX;
			b = (float)rand() / RAND_MAX;
			ultimoTiempo = tiempoActual; 
		}

		// Limpiar la ventana
		glClearColor(r, g, b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Para dibujar iniciales
		glUseProgram(shader);
		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 90);

		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	glfwDestroyWindow(mainWindow);
	glfwTerminate();
	return 0;
}