//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";

//shaders nuevos se crearían acá
static const char* vShaderRojo = "shaders/shaderrojo.vert";
static const char* fShaderRojo = "shaders/shaderrojo.frag";
static const char* vShaderAzul = "shaders/shaderazul.vert";
static const char* fShaderAzul = "shaders/shaderazul.frag";
static const char* vShaderVerde = "shaders/shaderverde.vert";
static const char* fShaderVerde = "shaders/shaderverde.frag";
static const char* vShaderMagenta = "shaders/shadermagenta.vert";
static const char* fShaderMagenta = "shaders/shadermagenta.frag";
static const char* vShaderCafe = "shaders/shadercafe.vert";
static const char* fShaderCafe = "shaders/shadercafe.frag";
static const char* vShaderNegro = "shaders/shadernegro.vert";
static const char* fShaderNegro = "shaders/shadernegro.frag";
static const char* vShaderAmarillo = "shaders/shaderamarillo.vert";
static const char* fShaderAmarillo = "shaders/shaderamarillo.frag";


float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = {
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3

	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

void CrearLetrasyFiguras() {

	GLfloat vertices_letras[] = {
		//PARA INICIALES SNT
		// PARA S - Color Rojo: 1.0f, 0.0f, 0.0f
		// Barra superior                                                                                           
		-0.75f,  0.35f, 0.0f,  1.0f, 0.0f, 0.0f,   -0.35f,  0.35f, 0.0f,  1.0f, 0.0f, 0.0f,  -0.75f,  0.20f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.35f,  0.35f, 0.0f,  1.0f, 0.0f, 0.0f,   -0.35f,  0.20f, 0.0f,  1.0f, 0.0f, 0.0f,  -0.75f,  0.20f, 0.0f,  1.0f, 0.0f, 0.0f,
		// Bajada superior izquierda
		-0.75f,  0.20f, 0.0f,  1.0f, 0.0f, 0.0f,   -0.60f,  0.20f, 0.0f,  1.0f, 0.0f, 0.0f,  -0.75f,  0.05f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.60f,  0.20f, 0.0f,  1.0f, 0.0f, 0.0f,   -0.60f,  0.05f, 0.0f,  1.0f, 0.0f, 0.0f,  -0.75f,  0.05f, 0.0f,  1.0f, 0.0f, 0.0f,
		// Barra central
		-0.75f,  0.05f, 0.0f,  1.0f, 0.0f, 0.0f,   -0.35f,  0.05f, 0.0f,  1.0f, 0.0f, 0.0f,  -0.75f, -0.10f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.35f,  0.05f, 0.0f,  1.0f, 0.0f, 0.0f,   -0.35f, -0.10f, 0.0f,  1.0f, 0.0f, 0.0f,  -0.75f, -0.10f, 0.0f,  1.0f, 0.0f, 0.0f,
		// Bajada inferior derecha
		-0.50f, -0.10f, 0.0f,  1.0f, 0.0f, 0.0f,   -0.35f, -0.10f, 0.0f,  1.0f, 0.0f, 0.0f,  -0.50f, -0.25f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.35f, -0.10f, 0.0f,  1.0f, 0.0f, 0.0f,   -0.35f, -0.25f, 0.0f,  1.0f, 0.0f, 0.0f,  -0.50f, -0.25f, 0.0f,  1.0f, 0.0f, 0.0f,
		// Barra inferior
		-0.75f, -0.25f, 0.0f,  1.0f, 0.0f, 0.0f,   -0.35f, -0.25f, 0.0f,  1.0f, 0.0f, 0.0f,  -0.75f, -0.40f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.35f, -0.25f, 0.0f,  1.0f, 0.0f, 0.0f,   -0.35f, -0.40f, 0.0f,  1.0f, 0.0f, 0.0f,  -0.75f, -0.40f, 0.0f,  1.0f, 0.0f, 0.0f,

		// PARA N - Color Amarillo Dorado: 1.0f, 0.84f, 0.0f
		// Barra izquierda
		-0.17f,  0.35f, 0.0f,  1.0f, 0.84f, 0.0f,   -0.07f,  0.35f, 0.0f,  1.0f, 0.84f, 0.0f,  -0.17f, -0.40f, 0.0f,  1.0f, 0.84f, 0.0f,
		-0.07f,  0.35f, 0.0f,  1.0f, 0.84f, 0.0f,   -0.07f, -0.40f, 0.0f,  1.0f, 0.84f, 0.0f,  -0.17f, -0.40f, 0.0f,  1.0f, 0.84f, 0.0f,
		// Barra diagonal del centro
		-0.10f,  0.35f, 0.0f,  1.0f, 0.84f, 0.0f,    0.10f, -0.40f, 0.0f,  1.0f, 0.84f, 0.0f,   0.00f,  0.35f, 0.0f,  1.0f, 0.84f, 0.0f,
		0.10f, -0.40f, 0.0f,  1.0f, 0.84f, 0.0f,   -0.10f,  0.35f, 0.0f,  1.0f, 0.84f, 0.0f,   0.00f, -0.40f, 0.0f,  1.0f, 0.84f, 0.0f,
		// Barra derecha
		0.07f,  0.35f, 0.0f,  1.0f, 0.84f, 0.0f,    0.17f,  0.35f, 0.0f,  1.0f, 0.84f, 0.0f,   0.07f, -0.40f, 0.0f,  1.0f, 0.84f, 0.0f,
		0.17f,  0.35f, 0.0f,  1.0f, 0.84f, 0.0f,    0.17f, -0.40f, 0.0f,  1.0f, 0.84f, 0.0f,   0.07f, -0.40f, 0.0f,  1.0f, 0.84f, 0.0f,

		// PARA T - Color Azul: 0.0f, 0.0f, 1.0f
		// Barra horizontal superior
		0.35f,  0.35f, 0.0f,  0.0f, 0.0f, 1.0f,    0.75f,  0.35f, 0.0f,  0.0f, 0.0f, 1.0f,   0.35f,  0.20f, 0.0f,  0.0f, 0.0f, 1.0f,
		0.75f,  0.35f, 0.0f,  0.0f, 0.0f, 1.0f,    0.75f,  0.20f, 0.0f,  0.0f, 0.0f, 1.0f,   0.35f,  0.20f, 0.0f,  0.0f, 0.0f, 1.0f,
		// Barra vertical central
		0.48f,  0.20f, 0.0f,  0.0f, 0.0f, 1.0f,    0.62f,  0.20f, 0.0f,  0.0f, 0.0f, 1.0f,   0.48f, -0.40f, 0.0f,  0.0f, 0.0f, 1.0f,
		0.62f,  0.20f, 0.0f,  0.0f, 0.0f, 1.0f,    0.62f, -0.40f, 0.0f,  0.0f, 0.0f, 1.0f,   0.48f, -0.40f, 0.0f,  0.0f, 0.0f, 1.0f
	};
	MeshColor* letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras, 360);
	meshColorList.push_back(letras);

	GLfloat vertices_triangulorojo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
	};
	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);


	// Para triangulo azul
	GLfloat vertices_trianguloazul[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
	};
	MeshColor* trianguloazul = new MeshColor();
	trianguloazul->CreateMeshColor(vertices_trianguloazul, 18);
	meshColorList.push_back(trianguloazul);

	// Para triangulo verde 
	GLfloat vertices_trianguloverde[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	1.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	1.0f,	0.0f,
	};
	MeshColor* trianguloverde = new MeshColor();
	trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
	meshColorList.push_back(trianguloverde);

	// Para cuadrado rojo
	GLfloat vertices_cuadradorojo[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
	};
	MeshColor* cuadradorojo = new MeshColor();
	cuadradorojo->CreateMeshColor(vertices_cuadradorojo, 36);
	meshColorList.push_back(cuadradorojo);

	// Para cuadrado verde
	GLfloat vertices_cuadradoverde[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
	};
	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde);

	// Para cuadrado café 
	GLfloat vertices_cuadradocafe[] = {
		//X			Y			Z			R        G        B
		-0.5f,	-0.5f,		0.5f,			0.478f,  0.255f,  0.067f,
		0.5f,	-0.5f,		0.5f,			0.478f,  0.255f,  0.067f,
		0.5f,	0.5f,		0.5f,			0.478f,  0.255f,  0.067f,
		-0.5f,	-0.5f,		0.5f,			0.478f,  0.255f,  0.067f,
		0.5f,	0.5f,		0.5f,			0.478f,  0.255f,  0.067f,
		-0.5f,	0.5f,		0.5f,			0.478f,  0.255f,  0.067f,
	};
	MeshColor* cuadradocafe = new MeshColor();
	cuadradocafe->CreateMeshColor(vertices_cuadradocafe, 36);
	meshColorList.push_back(cuadradocafe);

	// Para rectangulo negro 
	GLfloat vertices_cuadradonegro[] = {
		//X			Y			Z			R        G        B
		-0.5f,	-0.5f,		0.5f,			0.0f,    0.0f,    0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,    0.0f,    0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,    0.0f,    0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,    0.0f,    0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,    0.0f,    0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,    0.0f,    0.0f,
	};
	MeshColor* cuadradonegro = new MeshColor();
	cuadradonegro->CreateMeshColor(vertices_cuadradonegro, 36);
	meshColorList.push_back(cuadradonegro);

	// Para triangulo amarillo 
	GLfloat vertices_trianguloamarillo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	1.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	1.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	1.0f,	0.0f,
	};
	MeshColor* trianguloamarillo = new MeshColor();
	trianguloamarillo->CreateMeshColor(vertices_trianguloamarillo, 18);
	meshColorList.push_back(trianguloamarillo);

	// Para cuadrado morado
	GLfloat vertices_cuadradomorado[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.5f,	0.0f,	1.0f,
		0.5f,	-0.5f,		0.5f,			0.5f,	0.0f,	1.0f,
		0.5f,	0.5f,		0.5f,			0.5f,	0.0f,	1.0f,
		-0.5f,	-0.5f,		0.5f,			0.5f,	0.0f,	1.0f,
		0.5f,	0.5f,		0.5f,			0.5f,	0.0f,	1.0f,
		-0.5f,	0.5f,		0.5f,			0.5f,	0.0f,	1.0f,
	};
	MeshColor* cuadradomorado = new MeshColor();
	cuadradomorado->CreateMeshColor(vertices_cuadradomorado, 36);
	meshColorList.push_back(cuadradomorado);

	// Para cuadrado amarillo
	GLfloat vertices_cuadradoamarillo[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			1.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			1.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			1.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			1.0f,	1.0f,	0.0f,
	};
	MeshColor* cuadradoamarillo = new MeshColor();
	cuadradoamarillo->CreateMeshColor(vertices_cuadradoamarillo, 36);
	meshColorList.push_back(cuadradoamarillo);

	// Para cuadrado azul
	GLfloat vertices_cuadradoazul[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
	};
	MeshColor* cuadradoazul = new MeshColor();
	cuadradoazul->CreateMeshColor(vertices_cuadradoazul, 36);
	meshColorList.push_back(cuadradoazul);

	// Para triangulo morado
	GLfloat vertices_triangulomorado[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.5f,	0.0f,	1.0f,
		1.0f,	-1.0f,		0.5f,			0.5f,	0.0f,	1.0f,
		0.0f,	1.0f,		0.5f,			0.5f,	0.0f,	1.0f,
	};
	MeshColor* triangulomorado = new MeshColor();
	triangulomorado->CreateMeshColor(vertices_triangulomorado, 18);
	meshColorList.push_back(triangulomorado);

}


void CreateShaders()
{

	Shader* shader0 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader0->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader0);

	Shader* shader1 = new Shader();//shader para usar color como parte del VAO: letras 
	shader1->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader(); 
	shader2->CreateFromFiles(vShaderRojo, fShaderRojo);
	shaderList.push_back(*shader2);

	Shader* shader3 = new Shader(); 
	shader3->CreateFromFiles(vShaderAzul, fShaderAzul);
	shaderList.push_back(*shader3);

	Shader* shader4 = new Shader();
	shader4->CreateFromFiles(vShaderVerde, fShaderVerde);
	shaderList.push_back(*shader4);

	Shader* shader5 = new Shader();
	shader5->CreateFromFiles(vShaderCafe, fShaderCafe);
	shaderList.push_back(*shader5);

	Shader* shader6 = new Shader();
	shader6->CreateFromFiles(vShaderAmarillo, fShaderAmarillo);
	shaderList.push_back(*shader6);

	Shader* shader7 = new Shader();
	shader7->CreateFromFiles(vShaderMagenta, fShaderMagenta);
	shaderList.push_back(*shader7);

	Shader* shader8 = new Shader();
	shader8->CreateFromFiles(vShaderNegro, fShaderNegro);
	shaderList.push_back(*shader8);
}


int main() {
	mainWindow = Window(800, 800);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList (incluyendo el cuadrado negro en índice 5)
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana con color azul claro 
		glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad

		/*
		// PARA LETRAS DE INICIALES CON COLOR DIFERENTE CADA UNA
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();
		*/
		
		// PARA FIGURAS DE IMAGEN DE CLASE
		// Para barra negra rectangular de base
		shaderList[8].useShader(); 
		uniformModel = shaderList[8].getModelLocation();
		uniformProjection = shaderList[8].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.75f, -3.0f));
		model = glm::scale(model, glm::vec3(1.9f, 0.12f, 1.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh(); 

		// Para primer figura
		// Poste izquierdo cafe
		shaderList[5].useShader(); 
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.8f, -0.32f, -3.0f));
		model = glm::scale(model, glm::vec3(0.06f, 0.75f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh(); 

		// Poste derecho cafe
		shaderList[5].useShader(); 
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.45f, -0.32f, -3.0f));
		model = glm::scale(model, glm::vec3(0.06f, 0.75f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh(); 

		// Para triangulo verde inferior invertido
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.625f, -0.63f, -3.0f)); // Un poco más abajo
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.24f, 0.21f, 1.8f)); // Ligeramente más compactos
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// Para triangulo rojo medio invertido
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.625f, -0.40f, -3.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.24f, 0.21f, 1.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// Para triangulo amarillo superior invertido
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.625f, -0.17f, -3.0f)); // Mayor espacio libre arriba en los postes
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.24f, 0.21f, 1.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// Para segunda figura
		// Para cubo morado (Magente)
		shaderList[7].useShader(); 
		uniformModel = shaderList[7].getModelLocation();
		uniformProjection = shaderList[7].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.2f, -0.564f, -3.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh(); 

		// Para cubo verde
		shaderList[4].useShader(); 
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.05f, -0.564f, -3.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh(); 

		// Para cubo amarillo
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.2f, -0.313f, -3.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh(); 

		// Para cubo rojo
		shaderList[2].useShader(); 
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.05f, -0.313f, -3.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh(); 

		// Para cubo azul
		shaderList[3].useShader(); 
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-.075f, -0.44f, -2.9f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); 
		model = glm::scale(model, glm::vec3(0.355f, 0.355f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh(); 

		// Para cubo Cafe
		shaderList[5].useShader(); 
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-.075f, -0.44f, -2.8f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh(); 

		// Para tercer figura
		// Para piramide verde
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.35f, -0.44f, -2.1f));
		model = glm::scale(model, glm::vec3(0.18f, 0.28f, 2.0f)); // Z muy delgado para que sea plano
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// Para piramide roja
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.53f, -0.44f, -2.1f));
		model = glm::scale(model, glm::vec3(0.18f, 0.28f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// Para piramide amarilla invertida
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.44f, -0.48f, -2.1f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.18f, 0.28f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// Para piramide morada (magenta)
		shaderList[7].useShader();
		uniformModel = shaderList[7].getModelLocation();
		uniformProjection = shaderList[7].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.42f, -0.2f, -2.1f));
		model = glm::scale(model, glm::vec3(0.18f, 0.28f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();
		






		/*
		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		//
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();

		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0 en ShaderList
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		*/

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/