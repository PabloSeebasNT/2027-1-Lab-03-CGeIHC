/*
Práctica 6: Texturizado
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION


#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList; //solo recibe xyz
std::vector<MeshColor*> meshListColor; // recibe xyz rgb
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz

std::vector<Shader> shaderList;

Camera camera;

Texture plainTexture;
Texture pisoTexture;
Texture dadoTexture;
Texture holocronTexture;

Model Kitt_M;
Model Llanta_M;
Model Dado_M;
Model Holocron_M;
Model Avion_M;

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_texture.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_texture.frag";





void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};


	
	MeshModel *obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj1);

	MeshModel *obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj2);

	MeshModel *obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshListModel.push_back(obj3);

	MeshModel* obj4 = new MeshModel();
	obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
	meshListModel.push_back(obj4);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.17f,  0.0f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.29f,	0.0f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.29f,	0.19f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.17f,	0.19f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.85f,  0.0f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.96f,	0.0f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.96f,	0.19f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.85f,	0.19f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.96f,  0.82f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.85f,	0.82f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.85f,	1.0f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.96f,	1.0f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.68f,  0.41f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.82f,	0.41f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.82f,	0.6f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.68f,	0.6f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.48f,  0.59f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.67f,	0.59f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.67f,	0.41f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.48f,	0.41f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.35f,  0.61f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.52f,	0.61f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.52f,	0.79f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.35f,	0.79f,		0.0f,	-1.0f,	0.0f,

	};

	MeshModel* dado = new MeshModel();
	dado->CreateMeshModel(cubo_vertices, cubo_indices, 192, 36);
	meshListModel.push_back(dado);

}

void CrearHolocron()
{
	unsigned int holocron_indices[] = {
		0, 1, 2,    // Triángulo 1
		3, 4, 5,    // Triángulo 2
		6, 7, 8,    // Triángulo 3
		9, 10, 11,  // Triángulo 4
		12, 13, 14, // Triángulo 5
		15, 16, 17, // Triángulo 6
		18, 19, 20, // Triángulo 7
		21, 22, 23, // Triángulo 8
		24, 25, 26, // Triángulo 9
		27, 28, 29, // Triángulo 10
		30, 31, 32, // Triángulo 11
		33, 34, 35, // Triángulo 12
		36, 37, 38, // Triángulo 13
		39, 40, 41, // Triángulo 14
		42, 43, 44, // Triángulo 15
		45, 46, 47, // Triángulo 16
		48, 49, 50, // Triángulo 17
		51, 52, 53, // Triángulo 18
		54, 55, 56, // Triángulo 19
		57, 58, 59  // Triángulo 20
	};

	GLfloat holocron_vertices[] = {
		// --- Triángulo 1 ---
			-0.081432f, -2.513204f,  2.426005f,   0.715f, 0.202f,   -0.5588f, -0.5875f,  0.5853f,
			-2.534944f, -0.061146f,  2.580280f,   0.558f, 0.499f,   -0.5588f, -0.5875f,  0.5853f,
			-2.632201f, -2.553892f, -0.007226f,   0.442f, 0.198f,   -0.5587f, -0.5875f,  0.5853f,

			// --- Triángulo 2 ---
			 2.453980f, -0.063388f,  2.477451f,   0.831f, 0.085f,    0.9999f,  0.0012f, -0.0165f,
			 2.646786f, -0.139047f, -2.493757f,   0.982f, 0.085f,    0.9999f,  0.0019f, -0.0170f,
			 2.451372f,  2.421759f, -0.077538f,   0.924f, 0.224f,    0.9999f,  0.0019f, -0.0169f,

			 // --- Triángulo 3 ---
			 -2.521206f,  2.448241f, -0.063388f,   0.26f, 1.0f,   -0.5667f,  0.5876f,  0.5776f,
			 -2.534944f, -0.061146f,  2.580280f,   0.46f, 0.724f,   -0.5666f,  0.5876f,  0.5776f,
			 -0.049649f,  2.440128f,  2.417217f,   0.678f, 1.0f,   -0.5666f,  0.5877f,  0.5775f,

			 // --- Triángulo 4 ---
			 -0.049649f,  2.440128f,  2.417217f,   0.0f, 1.0f,    0.5776f,  0.5876f,  0.5666f,
			  2.453980f, -0.063388f,  2.477451f,   0.104f, 0.612f,    0.5776f,  0.5876f,  0.5667f,
			  2.451372f,  2.421759f, -0.077538f,   0.271f, 1.0f,    0.5776f,  0.5877f,  0.5666f,

			  // --- Triángulo 5 ---
			   0.140696f, -2.601934f, -2.558561f,   0.073f, 0.378f,   -0.0006f,  0.0170f, -0.9999f,
			  -0.051152f,  2.399781f, -2.545043f,   0.070f, 0.612f,   -0.0008f,  0.0171f, -0.9999f,
			   2.646786f, -0.139047f, -2.493757f,   -0.076f, 0.467f,   -0.0009f,  0.0171f, -0.9999f,

			   // --- Triángulo 6 ---
				2.451372f,  2.421759f, -0.077538f,   0.414f, 0.79f,    0.5667f,  0.5874f, -0.5777f,
				2.646786f, -0.139047f, -2.493757f,   0.618f, 0.502f,    0.5664f,  0.5877f, -0.5778f,
			   -0.051152f,  2.399781f, -2.545043f,   0.816f, 0.79f,    0.5667f,  0.5874f, -0.5777f,

			   // --- Triángulo 7 ---
			   -0.051152f,  2.399781f, -2.545043f,   0.613f, 1.0f,   -0.5788f,  0.5849f, -0.5682f,
			   -2.593003f, -0.085222f, -2.505755f,   0.761f, 0.713f,   -0.5789f,  0.5850f, -0.5681f,
			   -2.521206f,  2.448241f, -0.063388f,   0.941f, 1.0f,   -0.5788f,  0.5849f, -0.5682f,

			   // --- Triángulo 8 ---
				0.140696f, -2.601934f, -2.558561f,   0.477f, 0.0f,    0.6175f, -0.5880f, -0.5225f,
				2.646786f, -0.139047f, -2.493757f,   0.393f, 0.227f,    0.6176f, -0.5877f, -0.5227f,
				2.517274f, -2.579095f, -0.002115f,   0.2f, 0.0f,    0.6176f, -0.5877f, -0.5227f,

				// --- Triángulo 9 ---
				-2.632201f, -2.553892f, -0.007226f,   0.86f, 0.0f,   -0.5558f, -0.5648f, -0.6100f,
				-2.593003f, -0.085222f, -2.505755f,   0.783f, 0.239f,   -0.5557f, -0.5649f, -0.6100f,
				 0.140696f, -2.601934f, -2.558561f,   0.6f, 0.0f,   -0.5557f, -0.5649f, -0.6100f,

				 // --- Triángulo 10 ---
				 -0.081432f, -2.513204f,  2.426005f,   0.247f, 0.151f,    0.0011f,  0.0174f,  0.9998f,
				 -0.049649f,  2.440128f,  2.417217f,   0.247f, 0.417f,    0.0011f,  0.0174f,  0.9998f,
				 -2.534944f, -0.061146f,  2.580280f,   0.077f, 0.304f,    0.0011f,  0.0175f,  0.9998f,

				 // --- Triángulo 11 ---
				 -2.632201f, -2.553892f, -0.007226f,   0.383f, 0.326f,   -0.9992f,  0.0358f, -0.0171f,
				 -2.521206f,  2.448241f, -0.063388f,   0.391f, 0.624f,   -0.9992f,  0.0358f, -0.0171f,
				 -2.593003f, -0.085222f, -2.505755f,   0.218f, 0.479f,   -0.9992f,  0.0359f, -0.0171f,

				 // --- Triángulo 12 ---
				 -0.049649f,  2.440128f,  2.417217f,   0.753f, 0.366f,    0.0012f,  0.9999f, -0.0170f,
				 -0.051152f,  2.399781f, -2.545043f,   0.75f, 0.6f,    0.0010f,  0.9999f, -0.0170f,
				 -2.521206f,  2.448241f, -0.063388f,   0.613f, 0.484f,    0.0015f,  0.9999f, -0.0168f,

				 // --- Triángulo 13 ---
				 -0.049649f,  2.440128f,  2.417217f,   0.753f, 0.366f,    0.0012f,  0.9999f, -0.0170f,
				  2.451372f,  2.421759f, -0.077538f,   0.889f, 0.49f,    0.0015f,  0.9999f, -0.0170f,
				 -0.051152f,  2.399781f, -2.545043f,   0.75f, 0.6f,    0.0010f,  0.9999f, -0.0170f,

				 // --- Triángulo 14 ---
				 -0.081432f, -2.513204f,  2.426005f,   0.247f, 0.151f,    0.0011f,  0.0174f,  0.9998f,
				  2.453980f, -0.063388f,  2.477451f,   0.422f, 0.292f,    0.5056f, -0.6013f,  0.6187f,
				 -0.049649f,  2.440128f,  2.417217f,   0.247f, 0.417f,    0.0011f,  0.0174f,  0.9998f,

				 // --- Triángulo 15 ---
				 -2.632201f, -2.553892f, -0.007226f,   0.383f, 0.326f,   -0.9992f,  0.0358f, -0.0171f,
				 -2.534944f, -0.061146f,  2.580280f,   0.576f, 0.479f,   -0.9992f,  0.0364f, -0.0167f,
				 -2.521206f,  2.448241f, -0.063388f,   0.391f, 0.624f,   -0.9992f,  0.0358f, -0.0171f,

				 // --- Triángulo 16 ---
				  0.140696f, -2.601934f, -2.558561f,   0.073f, 0.378f,   -0.0006f,  0.0170f, -0.9999f,
				 -2.593003f, -0.085222f, -2.505755f,   0.228f, 0.465f,   -0.0008f,  0.0170f, -0.9999f,
				 -0.051152f,  2.399781f, -2.545043f,   0.070f, 0.612f,   -0.0008f,  0.0171f, -0.9999f,

				 // --- Triángulo 17 ---
				  2.453980f, -0.063388f,  2.477451f,   0.831f, 0.085f,    0.9999f,  0.0012f, -0.0165f,
				  2.517274f, -2.579095f, -0.002115f,   0.904f, -0.078f,    0.9999f,  0.0018f, -0.0169f,
				  2.646786f, -0.139047f, -2.493757f,   0.982f, 0.085f,    0.9999f,  0.0019f, -0.0170f,

				  // --- Triángulo 18 ---
				   0.140696f, -2.601934f, -2.558561f,   0.26f, 0.531f,   -0.0078f, -1.0000f, -0.0019f,
				  -0.081432f, -2.513204f,  2.426005f,   0.266f, 0.8f,    0.0082f, -0.9999f, -0.0113f,
				  -2.632201f, -2.553892f, -0.007226f,   0.111f, 0.71f,    0.0092f, -0.9996f, -0.0283f,

				  // --- Triángulo 19 ---
				   0.140696f, -2.601934f, -2.558561f,   0.26f, 0.531f,   -0.0078f, -1.0000f, -0.0019f,
				   2.517274f, -2.579095f, -0.002115f,   0.419f, 0.698f,   -0.0172f, -0.9998f, -0.0128f,
				  -0.081432f, -2.513204f,  2.426005f,   0.266f, 0.8f,    0.0082f, -0.9999f, -0.0113f,

				  // --- Triángulo 20 ---
				   2.517274f, -2.579095f, -0.002115f,   0.2f, 0.0f,    0.5056f, -0.6013f,  0.6187f,
				   2.453980f, -0.063388f,  2.477451f,   0.1f, 0.207f,    0.5056f, -0.6014f,  0.6187f,
				  -0.081432f, -2.513204f,  2.426005f,   0.0f, 0.0f,    0.5056f, -0.6013f,  0.6187f
	};

	MeshModel* holocron = new MeshModel();
	holocron->CreateMeshModel(holocron_vertices, holocron_indices, 480, 60);
	meshListModel.push_back(holocron);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearDado();
	CrearHolocron();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	dadoTexture = Texture("Textures/star wars logos_editada.jpg");
	dadoTexture.LoadTextureA();
	holocronTexture = Texture("Textures/star wars logos_editada_holocron.png");
	holocronTexture.LoadTextureA();
	
	Holocron_M = Model();
	Holocron_M.LoadModel("Models/Holocron_starwars.obj");
	Dado_M = Model();
	Dado_M.LoadModel("Models/Dadostarwarsoptimizado.obj");
	Avion_M = Model();
	Avion_M.LoadModel("Models/avion.obj");
	
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		meshListModel[2]->RenderMeshModel();


		

		//Dado de Opengl
		//Ejercicio 1: Texturizar su dado con la imagen ya optimizada por ustedes con logos de star wars
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.5f, 4.5f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshListModel[4]->RenderMeshModel();
		
		//Ejercicio 2:Importar el cubo texturizado en el programa de modelado con 
		//la imagen ya optimizada por ustedes
		
		
		//Dado importado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.0f, 3.0f, -2.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dado_M.RenderModel();
		
	



		/*Reporte de práctica :
		
		Ejercicio 1: Crear o modificar el holocron y texturizarlo por medio de código
		Ejercicio 2: Importar el modelo del holocron texturizardo en el programa de modelado
		Ejercicio 3: Importar un modelo de avión con con la textura de la cara del personaje de la imagen del previo:
		Vidrio fonrtal: OJOS
		Frente del avión: Nariz y Sonrisa
		Alas: Logos del universo del personaje
		
		*/


		//Holocrones
		color = glm::vec3(1.0f, 1.0f, 1.0f);//color que multiplica a la información de color de la textura
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-8.5f, 4.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		holocronTexture.UseTexture();
		meshListModel[5]->RenderMeshModel();
		

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.5f, 2.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Holocron_M.RenderModel();

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-8.0f, 8.0f, -7.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Avion_M.RenderModel();

		


	
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
/*
//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		logofiTexture.UseTexture(); //textura con transparencia o traslucidez
		FIGURA A RENDERIZAR de OpenGL, si es modelo importado no se declara UseTexture
		glDisable(GL_BLEND);
*/