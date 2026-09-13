//práctica 3: Modelado Geométrico y Cámara Sintética.
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
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z


using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks




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

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* piramidet = new Mesh();
	piramidet->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(piramidet);

}


//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	unsigned int piramidecuadrangular_indices[] = {
		0,3,4,//frontal
		3,2,4,//izquierda
		2,1,4,//trasera
		1,0,4,//derecha
		0,1,2,//abajo1
		0,2,3//abajo2

	};
	GLfloat piramidecuadrangular_vertices[] = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramidec = new Mesh();
	piramidec->CreateMesh(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramidec);
}

// Función agregada para separar cada cara de una piramide cuadrangular
void CrearCarasPiramideCuadrangular()
{
	GLfloat v[] = {
		0.5f,-0.5f,0.5f,   
		0.5f,-0.5f,-0.5f,  
		-0.5f,-0.5f,-0.5f, 
		-0.5f,-0.5f,0.5f,  
		0.0f,0.5f,0.0f     
	};

	// Caras
	unsigned int ind_t1[] = { 0, 3, 4 }; // Cara 1
	unsigned int ind_t2[] = { 3, 2, 4 }; // Cara 2
	unsigned int ind_t3[] = { 2, 1, 4 }; // Cara 3
	unsigned int ind_t4[] = { 1, 0, 4 }; // Cara 4
	unsigned int ind_base[] = { 0, 1, 2, 0, 2, 3 }; // Cara cuadrada (Base)

	Mesh* cara1 = new Mesh(); cara1->CreateMesh(v, ind_t1, 15, 3); meshList.push_back(cara1); 
	Mesh* cara2 = new Mesh(); cara2->CreateMesh(v, ind_t2, 15, 3); meshList.push_back(cara2); 
	Mesh* cara3 = new Mesh(); cara3->CreateMesh(v, ind_t3, 15, 3); meshList.push_back(cara3); 
	Mesh* cara4 = new Mesh(); cara4->CreateMesh(v, ind_t4, 15, 3); meshList.push_back(cara4); 
	Mesh* base = new Mesh(); base->CreateMesh(v, ind_base, 15, 6); meshList.push_back(base);   
}



/*
Crear cilindro, cono y esferas con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
		else {
			x = R * cos((0)*dt);
			z = R * sin((0)*dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los vértices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh *cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res,float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);
	
	//ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res+2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh *cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(5, 1.0f);//índice 2 en MeshList
	CrearCono(25, 2.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
	CrearCarasPiramideCuadrangular(); // Para caras separadas
	CreateShaders();
	
	

	/*Cámara se usa el comando: glm::lookAt(vector de posición, vector de orientación, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posición,
	glm::vec3 vector up,
	GlFloat yaw rotación para girar hacia la derecha e izquierda
	GlFloat pitch rotación para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posición inicial está en 0,0,1 y ve hacia 0,0,-1.
	*/

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);

	
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);
	
	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4

	glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		/*
		// ---------------------------------------------------------------------------------Cohete Espacial ---------------------------------------------------------------------------------
		// Cuerpo principal de la nave
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f)); // Posición central en la escena
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 modelCuerpo = model;
		model = glm::scale(model, glm::vec3(3.0f, 0.8f, 3.0f)); // Aumentamos X y Z de 2.5f a 3.0f
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.85f, 0.85f, 0.85f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		// Franja de cubos negros que rodea el cuerpo principal de la nave
		int numCubosFranja = 60;
		float radioFranja = 2.85f;
		for (int i = 0; i < numCubosFranja; i++) {
			float angulo = i * (2.0f * 3.1416f / numCubosFranja);
			float x = cos(angulo) * radioFranja;
			float z = sin(angulo) * radioFranja;
			model = modelCuerpo;
			model = glm::translate(model, glm::vec3(x, 0.0f, z));
			model = glm::rotate(model, -angulo, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.55f, 0.25f, 0.35f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.08f, 0.08f, 0.08f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[0]->RenderMesh();
		}

		// Accesorios naranjas de la nave
		float radioAnillo = 3.00f;
		float radioEsferas = radioAnillo + 0.05f;
		int numNaranjas = 16;
		for (int i = 0; i < numNaranjas; i++) {
			float angulo = i * (2.0f * 3.1416f / numNaranjas);

			if (angulo > 5.60f || angulo < 0.68f) continue;
			float x = cos(angulo) * radioAnillo;
			float z = sin(angulo) * radioAnillo;
			model = modelCuerpo;
			model = glm::translate(model, glm::vec3(x, -0.20f, z));
			model = glm::rotate(model, -angulo, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.15f, 0.35f, 0.21f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.95f, 0.55f, 0.05f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			sp.render();
		}

		// Ventanas de cubos azules de la nave
		float angulosVentanas[2] = { -0.15f, 0.10f };
		for (int i = 0; i < 2; i++) {
			float angulo = angulosVentanas[i];
			float x = cos(angulo) * (radioAnillo + 0.02f);
			float z = sin(angulo) * (radioAnillo + 0.02f);

			model = modelCuerpo;
			model = glm::translate(model, glm::vec3(x, 0.0f, z));
			model = glm::rotate(model, -angulo, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.28f, 0.42f, 0.15f));

			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.0f, 0.3f, 0.9f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[0]->RenderMesh();
		}

		// Accesorios de la nave de esferas azules
		float angulosEsferas[] = {
			-0.35f,
			 0.35f,
			 0.90f, 1.30f, 1.70f, 2.10f, 2.50f, 2.90f, 3.30f, 3.70f, 4.10f, 4.50f, 4.90f, 5.35f
		};

		for (float angulo : angulosEsferas) {
			float x = cos(angulo) * radioEsferas;
			float z = sin(angulo) * radioEsferas;
			model = modelCuerpo;
			model = glm::translate(model, glm::vec3(x, 0.0f, z));
			model = glm::rotate(model, -angulo, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.11f, 0.11f, 0.11f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.0f, 0.4f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			sp.render();
		}

		// Para accesorios naranjas de la nave cercanos a esferas de lado de ventanas de cubos
		
		float angulosNaranjasFrontales[2] = { -0.58f, 0.58f };
		for (float angulo : angulosNaranjasFrontales) {
			float x = cos(angulo) * radioAnillo;
			float z = sin(angulo) * radioAnillo;

			model = modelCuerpo;
			model = glm::translate(model, glm::vec3(x, -0.20f, z));
			model = glm::rotate(model, -angulo, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.15f, 0.35f, 0.21f));

			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(0.95f, 0.55f, 0.05f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			sp.render();
		}
		
		// Para patas de la nave
		int numPatas = 16;
		float radioBasePatas = 2.75f;

		for (int i = 0; i < numPatas; i++) {
			float angulo = i * (2.0f * 3.1416f / numPatas);
			float x = cos(angulo) * radioBasePatas;
			float z = sin(angulo) * radioBasePatas;
			color = glm::vec3(0.82f, 0.82f, 0.82f);
			model = modelCuerpo;
			model = glm::translate(model, glm::vec3(x, -0.42f, z));
			model = glm::rotate(model, -angulo, glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 modelArticulacion = model;
			glm::mat4 modelAnclaje = glm::scale(model, glm::vec3(0.18f, 0.2f, 0.18f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAnclaje));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[4]->RenderMesh(); 

			model = modelArticulacion;
			model = glm::translate(model, glm::vec3(0.0f, -0.15f, 0.0f));
			glm::mat4 modelMuslo = model;

			glm::mat4 modelCilindroMuslo = glm::scale(model, glm::vec3(0.09f, 0.2f, 0.09f)); 
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCilindroMuslo));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[2]->RenderMeshGeometry(); 

			model = modelMuslo;
			model = glm::translate(model, glm::vec3(0.0f, -0.15f, 0.0f));
			glm::mat4 modelPuntaConica = glm::scale(model, glm::vec3(0.12f, 0.18f, 0.12f)); 
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPuntaConica));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			meshList[3]->RenderMeshGeometry();
		}

		// Para 3 esferas de decoracion arriba de la nave
		float radioCúpulaTop = 2.5f; 

		float angulosEsferasSup[3] = { -0.025f, -0.35f, 0.30f };
		float escalasEsferasSup[3] = { 0.38f,   0.28f,  0.28f };

		for (int i = 0; i < 3; i++) {
			float angulo = angulosEsferasSup[i];
			float x = cos(angulo) * radioCúpulaTop;
			float z = sin(angulo) * radioCúpulaTop;

			model = modelCuerpo;
			model = glm::translate(model, glm::vec3(x, 0.20f, z)); 
			model = glm::rotate(model, -angulo, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(escalasEsferasSup[i]));

			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

			glm::vec3 colorAzul = glm::vec3(0.0f, 0.4f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(colorAzul));

			sp.render();
		}

		// Para paneles cafe de arriba de la nave
		int totalPaneles = 26;
		float radioCafes = 2.5f;
		float anguloMin = -2.9f;
		float anguloMax = 2.9f;

		for (int i = 0; i < totalPaneles; i++) {
			float t = (float)i / (totalPaneles - 1);
			float angulo = anguloMin + t * (anguloMax - anguloMin);

			if (angulo > -0.55f && angulo < 0.55f) continue;

			float x = cos(angulo) * radioCafes;
			float z = sin(angulo) * radioCafes;

			model = modelCuerpo;
			model = glm::translate(model, glm::vec3(x, 0.25f, z)); 
			model = glm::rotate(model, -angulo, glm::vec3(0.0f, 1.0f, 0.0f));

			model = glm::scale(model, glm::vec3(0.8f, 0.37f, 0.8f));

			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

			glm::vec3 colorCafé = glm::vec3(0.6f, 0.35f, 0.15f); 
			glUniform3fv(uniformColor, 1, glm::value_ptr(colorCafé));
			meshList[0]->RenderMesh();
		}

		// Para escotilla de arriba de la nave
		model = modelCuerpo; 
		model = glm::translate(model, glm::vec3(0.0f, 0.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.65f, 0.15f, 0.65f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glm::vec3 colorEscotilla = glm::vec3(0.70f, 0.70f, 0.70f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorEscotilla));
		sp.render();
		*/


		
		// ---------------------------------------------------------------------------------Figura de unión de 8 piramides ---------------------------------------------------------------------------------
		// Para dibujar cada cara de color diferente de la piramide
		auto renderPiramideMulticolor = [&](glm::mat4& baseModel) {
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

			// Cara 1 - Roja
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(baseModel));
			glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));
			meshList[5]->RenderMesh();

			// Cara 2 - Verde
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(baseModel));
			glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
			meshList[6]->RenderMesh();

			// Cara 3 - Amarilla
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(baseModel));
			glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.0f)));
			meshList[7]->RenderMesh();

			// Cara 4 - Magenta
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(baseModel));
			glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 1.0f)));
			meshList[8]->RenderMesh();

			// Cara cuadrada (Base) - Azul
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(baseModel));
			glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 1.0f)));
			meshList[9]->RenderMesh();
			};

		// Piramides de arriba---------------------------------------
		// Piramide inferior

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		renderPiramideMulticolor(model);

		// Piramide superior
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		renderPiramideMulticolor(model);

		// Piramides de abajo----------------------------------------
		// Piramide superior
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		renderPiramideMulticolor(model);

		// Piramide inferior
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, -4.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		renderPiramideMulticolor(model);

		// Piramides de izquierda----------------------------------------
		// Piramide inferior
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.5f, -0.5f, -4.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		renderPiramideMulticolor(model);
		
		// Piramide superior
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.5f, -0.5f, -4.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		renderPiramideMulticolor(model);

		// Piramides de derecha----------------------------------------
		// Piramide inferior
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.5f, -0.5f, -4.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		renderPiramideMulticolor(model);
		
		// Piramide superior
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.5f, -0.5f, -4.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		renderPiramideMulticolor(model);
		



		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

	
		