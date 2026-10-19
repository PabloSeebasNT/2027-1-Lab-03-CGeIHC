/*
Práctica 7: Iluminación 1 
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
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
// Añadir Texturas del dado y del holocron
Texture dadoTexture;
Texture holocronTexture;


Model Blackhawk_M;
Model Rover_M; //Sustituir por sus propios Rovers Con jerarquía
Model Rover_cuerpo;
Model Rover_basebrazo;
Model Rover_partebrazo1;
Model Rover_partebrazo2;
Model Rover_partebrazo3;
Model Rover_partebrazo4;
Model Rover_pata1;
Model Rover_llanta1;
Model Rover_pata2;
Model Rover_llanta2;
Model Rover_pata3;
Model Rover_llanta3;
Model Rover_pata4;
Model Rover_llanta4;
Model Rover_pata5_parte1;
Model Rover_pata5_parte2;
Model Rover_llanta5;
Model Rover_pata6_parte1;
Model Rover_pata6_parte2;
Model Rover_llanta6;
//Añadir modelo del avión.
Model Avion_M;


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


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

	MeshModel *obj4 = new MeshModel();
	obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
	meshListModel.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

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
		-0.081432f, -2.513204f,  2.426005f,   0.715f, 0.202f,   0.0f, 0.0f, 0.0f,
		-2.534944f, -0.061146f,  2.580280f,   0.558f, 0.499f,   0.0f, 0.0f, 0.0f,
		-2.632201f, -2.553892f, -0.007226f,   0.442f, 0.198f,   0.0f, 0.0f, 0.0f,

		// --- Triángulo 2 ---
		 2.453980f, -0.063388f,  2.477451f,   0.831f, 0.085f,   0.0f, 0.0f, 0.0f,
		 2.646786f, -0.139047f, -2.493757f,   0.982f, 0.085f,   0.0f, 0.0f, 0.0f,
		 2.451372f,  2.421759f, -0.077538f,   0.924f, 0.224f,   0.0f, 0.0f, 0.0f,

		 // --- Triángulo 3 ---
		 -2.521206f,  2.448241f, -0.063388f,   0.26f, 1.0f,    0.0f, 0.0f, 0.0f,
		 -2.534944f, -0.061146f,  2.580280f,   0.46f, 0.724f,   0.0f, 0.0f, 0.0f,
		 -0.049649f,  2.440128f,  2.417217f,   0.678f, 1.0f,   0.0f, 0.0f, 0.0f,

		 // --- Triángulo 4 ---
		 -0.049649f,  2.440128f,  2.417217f,   0.0f, 1.0f,     0.0f, 0.0f, 0.0f,
		  2.453980f, -0.063388f,  2.477451f,   0.104f, 0.612f,   0.0f, 0.0f, 0.0f,
		  2.451372f,  2.421759f, -0.077538f,   0.271f, 1.0f,     0.0f, 0.0f, 0.0f,

		  // --- Triángulo 5 ---
		   0.140696f, -2.601934f, -2.558561f,   0.073f, 0.378f,  0.0f, 0.0f, 0.0f,
		  -0.051152f,  2.399781f, -2.545043f,   0.070f, 0.612f,  0.0f, 0.0f, 0.0f,
		   2.646786f, -0.139047f, -2.493757f,   -0.076f, 0.467f,  0.0f, 0.0f, 0.0f,

		   // --- Triángulo 6 ---
			2.451372f,  2.421759f, -0.077538f,   0.414f, 0.79f,   0.0f, 0.0f, 0.0f,
			2.646786f, -0.139047f, -2.493757f,   0.618f, 0.502f,   0.0f, 0.0f, 0.0f,
		   -0.051152f,  2.399781f, -2.545043f,   0.816f, 0.79f,   0.0f, 0.0f, 0.0f,

		   // --- Triángulo 7 ---
		   -0.051152f,  2.399781f, -2.545043f,   0.613f, 1.0f,   0.0f, 0.0f, 0.0f,
		   -2.593003f, -0.085222f, -2.505755f,   0.761f, 0.713f,   0.0f, 0.0f, 0.0f,
		   -2.521206f,  2.448241f, -0.063388f,   0.941f, 1.0f,   0.0f, 0.0f, 0.0f,

		   // --- Triángulo 8 ---
			0.140696f, -2.601934f, -2.558561f,   0.477f, 0.0f,   0.0f, 0.0f, 0.0f,
			2.646786f, -0.139047f, -2.493757f,   0.393f, 0.227f,   0.0f, 0.0f, 0.0f,
			2.517274f, -2.579095f, -0.002115f,   0.2f, 0.0f,     0.0f, 0.0f, 0.0f,

			// --- Triángulo 9 ---
			-2.632201f, -2.553892f, -0.007226f,   0.86f, 0.0f,   0.0f, 0.0f, 0.0f,
			-2.593003f, -0.085222f, -2.505755f,   0.783f, 0.239f,  0.0f, 0.0f, 0.0f,
			 0.140696f, -2.601934f, -2.558561f,   0.6f, 0.0f,     0.0f, 0.0f, 0.0f,

			 // --- Triángulo 10 ---
			 -0.081432f, -2.513204f,  2.426005f,   0.247f, 0.151f,  0.0f, 0.0f, 0.0f,
			 -0.049649f,  2.440128f,  2.417217f,   0.247f, 0.417f,  0.0f, 0.0f, 0.0f,
			 -2.534944f, -0.061146f,  2.580280f,   0.077f, 0.304f,  0.0f, 0.0f, 0.0f,

			 // --- Triángulo 11 ---
			 -2.632201f, -2.553892f, -0.007226f,   0.383f, 0.326f,  0.0f, 0.0f, 0.0f,
			 -2.521206f,  2.448241f, -0.063388f,   0.391f, 0.624f,  0.0f, 0.0f, 0.0f,
			 -2.593003f, -0.085222f, -2.505755f,   0.218f, 0.479f,  0.0f, 0.0f, 0.0f,

			 // --- Triángulo 12 ---
			 -0.049649f,  2.440128f,  2.417217f,   0.753f, 0.366f,  0.0f, 0.0f, 0.0f,
			 -0.051152f,  2.399781f, -2.545043f,   0.75f, 0.6f,     0.0f, 0.0f, 0.0f,
			 -2.521206f,  2.448241f, -0.063388f,   0.613f, 0.484f,  0.0f, 0.0f, 0.0f,

			 // --- Triángulo 13 ---
			 -0.049649f,  2.440128f,  2.417217f,   0.753f, 0.366f,  0.0f, 0.0f, 0.0f,
			  2.451372f,  2.421759f, -0.077538f,   0.889f, 0.49f,   0.0f, 0.0f, 0.0f,
			 -0.051152f,  2.399781f, -2.545043f,   0.75f, 0.6f,     0.0f, 0.0f, 0.0f,

			 // --- Triángulo 14 ---
			 -0.081432f, -2.513204f,  2.426005f,   0.247f, 0.151f,  0.0f, 0.0f, 0.0f,
			  2.453980f, -0.063388f,  2.477451f,   0.422f, 0.292f,  0.0f, 0.0f, 0.0f,
			 -0.049649f,  2.440128f,  2.417217f,   0.247f, 0.417f,  0.0f, 0.0f, 0.0f,

			 // --- Triángulo 15 ---
			 -2.632201f, -2.553892f, -0.007226f,   0.383f, 0.326f,  0.0f, 0.0f, 0.0f,
			 -2.534944f, -0.061146f,  2.580280f,   0.576f, 0.479f,  0.0f, 0.0f, 0.0f,
			 -2.521206f,  2.448241f, -0.063388f,   0.391f, 0.624f,  0.0f, 0.0f, 0.0f,

			 // --- Triángulo 16 ---
			  0.140696f, -2.601934f, -2.558561f,   0.073f, 0.378f,  0.0f, 0.0f, 0.0f,
			 -2.593003f, -0.085222f, -2.505755f,   0.228f, 0.465f,  0.0f, 0.0f, 0.0f,
			 -0.051152f,  2.399781f, -2.545043f,   0.070f, 0.612f,  0.0f, 0.0f, 0.0f,

			 // --- Triángulo 17 ---
			  2.453980f, -0.063388f,  2.477451f,   0.831f, 0.085f,  0.0f, 0.0f, 0.0f,
			  2.517274f, -2.579095f, -0.002115f,   0.904f, -0.078f, 0.0f, 0.0f, 0.0f,
			  2.646786f, -0.139047f, -2.493757f,   0.982f, 0.085f,  0.0f, 0.0f, 0.0f,

			  // --- Triángulo 18 ---
			   0.140696f, -2.601934f, -2.558561f,   0.26f, 0.531f,  0.0f, 0.0f, 0.0f,
			  -0.081432f, -2.513204f,  2.426005f,   0.266f, 0.8f,    0.0f, 0.0f, 0.0f,
			  -2.632201f, -2.553892f, -0.007226f,   0.111f, 0.71f,   0.0f, 0.0f, 0.0f,

			  // --- Triángulo 19 ---
			   0.140696f, -2.601934f, -2.558561f,   0.26f, 0.531f,  0.0f, 0.0f, 0.0f,
			   2.517274f, -2.579095f, -0.002115f,   0.419f, 0.698f, 0.0f, 0.0f, 0.0f,
			  -0.081432f, -2.513204f,  2.426005f,   0.266f, 0.8f,    0.0f, 0.0f, 0.0f,

			  // --- Triángulo 20 ---
			   2.517274f, -2.579095f, -0.002115f,   0.2f, 0.0f,     0.0f, 0.0f, 0.0f,
			   2.453980f, -0.063388f,  2.477451f,   0.1f, 0.207f,    0.0f, 0.0f, 0.0f,
			  -0.081432f, -2.513204f,  2.426005f,   0.0f, 0.0f,     0.0f, 0.0f, 0.0f
	};

	calcAverageNormals(holocron_indices, 60, holocron_vertices, 480, 8, 5);
	for (int i = 0; i < 480; i += 8) {
		holocron_vertices[i + 5] *= -1.0f; 
		holocron_vertices[i + 6] *= -1.0f; 
		holocron_vertices[i + 7] *= -1.0f; 
	}
	MeshModel* holocron = new MeshModel();
	holocron->CreateMeshModel(holocron_vertices, holocron_indices, 480, 60);
	meshListModel.push_back(holocron);
}




int main()
{
	mainWindow = Window(1366, 768); 
	mainWindow.Initialise();
	CreateObjects();
	CrearDado();
	CrearHolocron();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	dadoTexture = Texture("Textures/star wars logos_editada.png");
	dadoTexture.LoadTextureA();
	holocronTexture = Texture("Textures/star wars logos_editada_holocron.png");
	holocronTexture.LoadTextureA();

	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	
	Rover_M = Model();
	Rover_M.LoadModel("Models/rovermodificado.obj");
	Rover_cuerpo = Model();
	Rover_cuerpo.LoadModel("Models/Rover_cuerpo.obj");
	Rover_basebrazo = Model();
	Rover_basebrazo.LoadModel("Models/Rover_basebrazo.obj");
	Rover_partebrazo1 = Model();
	Rover_partebrazo1.LoadModel("Models/Rover_partebrazo1.obj");
	Rover_partebrazo2 = Model();
	Rover_partebrazo2.LoadModel("Models/Rover_partebrazo2.obj");
	Rover_partebrazo3 = Model();
	Rover_partebrazo3.LoadModel("Models/Rover_partebrazo3.obj");
	Rover_partebrazo4 = Model();
	Rover_partebrazo4.LoadModel("Models/Rover_partebrazo4.obj");
	Rover_pata1 = Model();
	Rover_pata1.LoadModel("Models/Rover_pata1.obj");
	Rover_llanta1 = Model();
	Rover_llanta1.LoadModel("Models/Rover_llanta1.obj");
	Rover_pata2 = Model();
	Rover_pata2.LoadModel("Models/Rover_pata2.obj");
	Rover_llanta2 = Model();
	Rover_llanta2.LoadModel("Models/Rover_llanta2.obj");
	Rover_llanta3 = Model();
	Rover_llanta3.LoadModel("Models/Rover_llanta3.obj");
	Rover_llanta4 = Model();
	Rover_llanta4.LoadModel("Models/Rover_llanta4.obj");
	Rover_pata3 = Model();
	Rover_pata3.LoadModel("Models/Rover_pata3.obj");
	Rover_pata4 = Model();
	Rover_pata4.LoadModel("Models/Rover_pata4.obj");
	Rover_pata5_parte1 = Model();
	Rover_pata5_parte1.LoadModel("Models/Rover_pata5_parte1.obj");
	Rover_pata5_parte2 = Model();
	Rover_pata5_parte2.LoadModel("Models/Rover_pata5_parte2.obj");
	Rover_llanta5 = Model();
	Rover_llanta5.LoadModel("Models/Rover_llanta5.obj");
	Rover_pata6_parte1 = Model();
	Rover_pata6_parte1.LoadModel("Models/Rover_pata6_parte1.obj");
	Rover_pata6_parte2 = Model();
	Rover_pata6_parte2.LoadModel("Models/Rover_pata6_parte2.obj");
	Rover_llanta6 = Model();
	Rover_llanta6.LoadModel("Models/Rover_llanta6.obj");
	
	//Añadir modelo del avión
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

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		1.0f, 0.0f, 0.0f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	
	//se crean mas luces puntuales y spotlight 
	// luz del rover (Faro) con mayor intensidad y un cono más amplio
	spotLights[2] = SpotLight(
		1.0f, 1.0f, 0.9f,     
		1.0f, 4.0f,           
		0.0f, 0.0f, 0.0f,     
		0.0f, 0.0f, 1.0f,    
		1.0f, 0.01f, 0.001f, 
		35.0f                 
	);
	spotLightCount++;

	spotLights[3] = SpotLight(
		1.0f, 1.0f, 0.9f,
		1.0f, 4.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.01f, 0.001f,
		35.0f
	);
	spotLightCount++;

	spotLights[4] = SpotLight(
		1.0f, 1.0f, 0.9f,
		1.0f, 4.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.01f, 0.001f,
		35.0f
	);
	spotLightCount++;





	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana

	glm::mat4 modelCuerpo(1.0);


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
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		/*
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		*/

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[2]->RenderMeshModel();
	
		/*
		//BLACKHAWK
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 6.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();
		*/

		//Rover: Sustituido
		// Cuerpo principal
		color = glm::vec3(0.549f, 0.412f, 0.275f); 
		modelCuerpo = glm::mat4(1.0);
		modelCuerpo = glm::translate(modelCuerpo, glm::vec3(0.0f + mainWindow.getmuevex(), 3.30f, -15.0));

		// Para faro
		glm::vec4 posicionLocalFaro = glm::vec4(1.5f, 0.5f, 0.0f, 1.0f);
		glm::vec3 posicionMundoFaro = glm::vec3(modelCuerpo * posicionLocalFaro);
		glm::vec4 direccionLocalFaro = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 direccionMundoFaro = glm::vec3(modelCuerpo * direccionLocalFaro);
		direccionMundoFaro = glm::normalize(direccionMundoFaro);

		spotLights[2].SetFlash(posicionMundoFaro, direccionMundoFaro);

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCuerpo));
		Rover_cuerpo.RenderModel();
		color = glm::vec3(0.0f, 0.0f, 0.0f);

		// Brazo -------------------------------------------------------------------------------------------------------------------------------------
		// Base del brazo
		glm::mat4 modelBaseBrazo = modelCuerpo;
		modelBaseBrazo = glm::translate(modelBaseBrazo, glm::vec3(1.64f, 0.0f, -1.11f));
		modelBaseBrazo = glm::rotate(modelBaseBrazo, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBaseBrazo));
		Rover_basebrazo.RenderModel();

		// Parte brazo 1
		color = glm::vec3(0.2f, 0.2f, 0.2f);
		glm::mat4 modelParteBrazo1 = modelBaseBrazo;
		modelParteBrazo1 = glm::translate(modelParteBrazo1, glm::vec3(0.0f, 2.0f, 0.11f));
		modelParteBrazo1 = glm::rotate(modelParteBrazo1, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelParteBrazo1));
		Rover_partebrazo1.RenderModel();

		// Parte brazo 2 
		color = glm::vec3(0.35f, 0.35f, 0.35f);
		glm::mat4 modelParteBrazo2 = modelParteBrazo1;
		modelParteBrazo2 = glm::translate(modelParteBrazo2, glm::vec3(2.57f, 2.5f, 0.00f));
		modelParteBrazo2 = glm::rotate(modelParteBrazo2, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelParteBrazo2));
		Rover_partebrazo2.RenderModel();

		// Parte brazo 3
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::mat4 modelParteBrazo3 = modelParteBrazo2;
		modelParteBrazo3 = glm::translate(modelParteBrazo3, glm::vec3(-2.4f, 2.8f, 0.055f));
		modelParteBrazo3 = glm::rotate(modelParteBrazo3, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelParteBrazo3 = glm::rotate(modelParteBrazo3, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelParteBrazo3));
		Rover_partebrazo3.RenderModel();

		// Parte brazo 4 
		color = glm::vec3(0.15f, 0.15f, 0.15f);
		glm::mat4 modelParteBrazo4 = modelParteBrazo3;
		modelParteBrazo4 = glm::translate(modelParteBrazo4, glm::vec3(0.0f, 0.1f, 0.02f));
		modelParteBrazo4 = glm::rotate(modelParteBrazo4, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelParteBrazo4));
		Rover_partebrazo4.RenderModel();
		// -------------------------------------------------------------------------------------------------------------------------------------------

		// Pata 1 -------------------------------------------------------------------------------------------------------------------------------------

		// Pata1
		glm::mat4 modelPata1 = modelCuerpo;
		modelPata1 = glm::translate(modelPata1, glm::vec3(-3.0f, 0.0f, 3.0f));
		modelPata1 = glm::rotate(modelPata1, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPata1));
		Rover_pata1.RenderModel();

		// Llanta 1
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::mat4 modelLlanta1 = modelPata1;
		modelLlanta1 = glm::translate(modelLlanta1, glm::vec3(-1.6f, -2.0f, 0.4f));
		modelLlanta1 = glm::rotate(modelLlanta1, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLlanta1));
		Rover_llanta1.RenderModel();
		// --------------------------------------------------------------------------------------------------------------------------------------------

		// Pata 2 -------------------------------------------------------------------------------------------------------------------------------------

		// Pata2
		color = glm::vec3(0.25f, 0.25f, 0.25f);
		glm::mat4 modelPata2 = modelCuerpo;
		modelPata2 = glm::translate(modelPata2, glm::vec3(-2.8f, -0.01f, 3.0f));
		modelPata2 = glm::rotate(modelPata2, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPata2));
		Rover_pata2.RenderModel();

		// Llanta 2
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::mat4 modelLlanta2 = modelPata2;
		modelLlanta2 = glm::translate(modelLlanta2, glm::vec3(1.6f, -2.0f, 0.4f));
		modelLlanta2 = glm::rotate(modelLlanta2, glm::radians(mainWindow.getarticulacion9()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLlanta2));
		Rover_llanta2.RenderModel();
		// --------------------------------------------------------------------------------------------------------------------------------------------

		// Pata 3 -------------------------------------------------------------------------------------------------------------------------------------

		// Pata3
		color = glm::vec3(0.25f, 0.25f, 0.25f);
		glm::mat4 modelPata3 = modelCuerpo;
		modelPata3 = glm::translate(modelPata3, glm::vec3(-3.0f, -0.00f, -3.0f));
		modelPata3 = glm::rotate(modelPata3, glm::radians(mainWindow.getarticulacion10()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPata3));
		Rover_pata3.RenderModel();

		// Llanta 3
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::mat4 modelLlanta3 = modelPata3;
		modelLlanta3 = glm::translate(modelLlanta3, glm::vec3(-1.6f, -2.0f, 0.4f));
		modelLlanta3 = glm::rotate(modelLlanta3, glm::radians(mainWindow.getarticulacion11()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLlanta3));
		Rover_llanta3.RenderModel();
		// --------------------------------------------------------------------------------------------------------------------------------------------

		// Pata 4 -------------------------------------------------------------------------------------------------------------------------------------

		// Pata4
		color = glm::vec3(0.25f, 0.25f, 0.25f);
		glm::mat4 modelPata4 = modelCuerpo;
		modelPata4 = glm::translate(modelPata4, glm::vec3(-2.8f, 0.0f, -3.0f));
		modelPata4 = glm::rotate(modelPata4, glm::radians(mainWindow.getarticulacion12()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPata4));
		Rover_pata4.RenderModel();

		// Llanta 4
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::mat4 modelLlanta4 = modelPata4;
		modelLlanta4 = glm::translate(modelLlanta4, glm::vec3(1.6f, -2.0f, 0.4f));
		modelLlanta4 = glm::rotate(modelLlanta4, glm::radians(mainWindow.getarticulacion13()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLlanta4));
		Rover_llanta4.RenderModel();
		// --------------------------------------------------------------------------------------------------------------------------------------------

		// Pata 5 -------------------------------------------------------------------------------------------------------------------------------------

		// Pata5 Parte 1
		color = glm::vec3(0.25f, 0.25f, 0.25f);
		glm::mat4 modelPata5_Parte1 = modelCuerpo;
		modelPata5_Parte1 = glm::translate(modelPata5_Parte1, glm::vec3(2.0f, 1.0f, -3.0f));
		modelPata5_Parte1 = glm::rotate(modelPata5_Parte1, glm::radians(mainWindow.getarticulacion14()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPata5_Parte1));
		Rover_pata5_parte1.RenderModel();

		// Pata5 Parte 2
		color = glm::vec3(0.35f, 0.35f, 0.35f);
		glm::mat4 modelPata5_Parte2 = modelPata5_Parte1;
		modelPata5_Parte2 = glm::translate(modelPata5_Parte2, glm::vec3(3.5f, -0.9f, 0.0f));
		modelPata5_Parte2 = glm::rotate(modelPata5_Parte2, glm::radians(mainWindow.getarticulacion15()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPata5_Parte2));
		Rover_pata5_parte2.RenderModel();

		// Llanta 5
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::mat4 modelLlanta5 = modelPata5_Parte2;
		modelLlanta5 = glm::translate(modelLlanta5, glm::vec3(-0.0f, -1.6f, 1.0f));
		modelLlanta5 = glm::rotate(modelLlanta5, glm::radians(mainWindow.getarticulacion16()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLlanta5));
		Rover_llanta5.RenderModel();

		// Pata 6 -------------------------------------------------------------------------------------------------------------------------------------

		// Pata6 Parte 1
		color = glm::vec3(0.25f, 0.25f, 0.25f);
		glm::mat4 modelPata6_Parte1 = modelCuerpo;
		modelPata6_Parte1 = glm::translate(modelPata6_Parte1, glm::vec3(2.0f, 1.0f, 3.0f));
		modelPata6_Parte1 = glm::rotate(modelPata6_Parte1, glm::radians(mainWindow.getarticulacion17()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPata6_Parte1));
		Rover_pata6_parte1.RenderModel();

		// Pata6 Parte 2
		color = glm::vec3(0.35f, 0.35f, 0.35f);
		glm::mat4 modelPata6_Parte2 = modelPata6_Parte1;
		modelPata6_Parte2 = glm::translate(modelPata6_Parte2, glm::vec3(3.5f, -0.9f, 0.0f));
		modelPata6_Parte2 = glm::rotate(modelPata6_Parte2, glm::radians(mainWindow.getarticulacion18()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPata6_Parte2));
		Rover_pata6_parte2.RenderModel();

		// Llanta 6
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::mat4 modelLlanta6 = modelPata6_Parte2;
		modelLlanta6 = glm::translate(modelLlanta6, glm::vec3(0.2f, -1.5f, -1.0f));
		modelLlanta6 = glm::rotate(modelLlanta6, glm::radians(mainWindow.getarticulacion19()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelLlanta6));
		Rover_llanta6.RenderModel();
		// --------------------------------------------------------------------------------------------------------------------------------------------
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//Dado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 5.0f, 6.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshListModel[4]->RenderMeshModel();

		//Holocron
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.0f, 7.0f, 6.0));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		holocronTexture.UseTexture();
		meshListModel[5]->RenderMeshModel();


		//Añadir modelo del avión
		// Avion------------------------------------------------------------------------------------------------------------------------------------------------
		// Faro 1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-40.0f, 5.0f + mainWindow.getmueveyavion(), 6.0f + mainWindow.getmuevexavion()));
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		glm::vec4 posicionLocalFaroAvion = glm::vec4(0.0f, -0.06f, 0.0f, 1.0f);
		glm::vec3 posicionMundoFaroAvion = glm::vec3(model * posicionLocalFaroAvion);
		glm::vec4 direccionLocalFaroAvion = glm::vec4(0.0f, -1.0, 0.0f, 0.0f);
		glm::vec3 direccionMundoFaroAvion = glm::vec3(model * direccionLocalFaroAvion);
		direccionMundoFaroAvion = glm::normalize(direccionMundoFaroAvion);
		spotLights[3].SetFlash(posicionMundoFaroAvion, direccionMundoFaroAvion);

		// Faro 2 
		glm::vec4 posicionLocalFaroFrontal = glm::vec4(0.0f, 0.0f, -2.5f, 1.0f);
		glm::vec3 posicionMundoFaroFrontal = glm::vec3(model * posicionLocalFaroFrontal);
		glm::vec4 direccionLocalFaroFrontal = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
		glm::vec3 direccionMundoFaroFrontal = glm::vec3(model * direccionLocalFaroFrontal);
		direccionMundoFaroFrontal = glm::normalize(direccionMundoFaroFrontal);
		spotLights[4].SetFlash(posicionMundoFaroFrontal, direccionMundoFaroFrontal);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Avion_M.RenderModel();
		//------------------------------------------------------------------------------------------------------------------------------------------------------


		//Agave ¿qué sucede si lo renderizan antes del helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[3]->RenderMeshModel();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
