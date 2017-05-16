#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include "myClasses.h"
#include <time.h>
#include <stdio.h>

using namespace std;
particleManager pM;
Esfera esfera;

bool show_test_window = false;
static bool Play_simulation = true;
static bool Reset = false;
static float resertTime=20;
float Second;
//Gravity
static float GravityAccel[3] = { 0.0f, -9.81f,0.0f };
//k
static float linkDistance[2] = { 0.8, 0.6 };
//collisions
static bool collisions = true;
static bool sphereCollisions = false;
static float ElasticCoeff=0.1;
static float FrictionCoeff=0.1;

void GUI() {
	{	//FrameRate
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		
		Second += 1 / ImGui::GetIO().Framerate;
		
		//TODO
		//for play & stop
		ImGui::Checkbox("Play simulation", &Play_simulation);
		//for resert
		if (ImGui::Button("Reset simulation")) {
			Reset ^= 1;
		}
		//Resert Time
		ImGui::DragFloat("Resert Time", &resertTime, 20); 
		ImGui::InputFloat3("Gravity Accel", GravityAccel);

		//Link distance
		ImGui::InputFloat2("Particle distance", linkDistance);
			
		//Collisions
		if (ImGui::CollapsingHeader("Collisions")) {
			//Use Sphere collider
			ImGui::Checkbox("Use Sphere Collider", &sphereCollisions);
			//Elastic Coefficion
			ImGui::InputFloat("Elastic Coefficion", &ElasticCoeff, 0.3);
			//Friction Coefficion
			ImGui::InputFloat("Friction Coefficion", &FrictionCoeff, 0.3);
		}
	}
	// ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if(show_test_window) {
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
}

float RandomFloat(float min, float max) {
	return  (max - min) * ((((float)rand()) / (float)RAND_MAX)) + min;
}

void PhysicsInit() {
	//actualitzar variables del GUI
	pM.lHorizontal = linkDistance[0];
	pM.lVertical = linkDistance[1];
	pM.gravity = GravityAccel[1];

	//crear malla
	for (int i = 0; i < 18;i++) {
		for (int j = 0; j < 14;j++) {
				Particle temp({ -5 + pM.lVertical*j, 2, -5 + i*pM.lHorizontal }, ElasticCoeff, FrictionCoeff);
				pM.particles.push_back(temp);
						
		}			
	}
	
	//La esfera
	srand(time(NULL));

	float min = 2;
	float max = 4;

	esfera.radius = rand() % (int)(max - min) + min;

	min = -4 + esfera.radius;
	max = 4 - esfera.radius;

	esfera.position.x = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
	esfera.position.y = (min+4) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
	esfera.position.z = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
	

	//els murs
	/*pM.wallNormals[0] = { 0,1,0 };
	pM.wallNormals[1] = { 0,-1,0 };
	pM.wallNormals[2] = { 1,0,0 };
	pM.wallNormals[3] = { -1,0,0 };
	pM.wallNormals[4] = { 0,0,1 };
	pM.wallNormals[5] = { 0,0,-1 };
	pM.wallDs[0] = 0;
	pM.wallDs[1] = 10;
	pM.wallDs[2] = -5;
	pM.wallDs[3] = 5;
	pM.wallDs[4] = 5;
	pM.wallDs[5] = -5;*/

	//Sphere::updateSphere(esfera.position, esfera.radius);

}
void PhysicsUpdate(float dt) {
	if (Play_simulation) {
		for (int i = 0; i < pM.particles.size();i++) {
			pM.elasticCoef = ElasticCoeff;
			pM.frictionCoef = FrictionCoeff;
			//if (sphereCollisions)
				//pM.particles[i].DetectSphere(esfera.position, esfera.radius, dt);
		}
		for (int i = 0; i < 10;i++) {
			pM.Update(dt/10);
		}
		//Pintar
		ClothMesh::updateClothMesh(partVerts);
		if (sphereCollisions)
			renderSphere = true;
		else
			renderSphere = false;

		//Si s'acaba el temps reset
		if (Second >= resertTime) {
			Second = 0;
			Reset = true;
		}
	}

	if (Reset) {
		Second = 0;
		Second += 1 / ImGui::GetIO().Framerate;
		for (int i = 0; i < pM.particles.size(); i++) {
			pM.particles.clear(); //netejem l'array i com que el dels vertex nomes fem update tenint en compte el tamany d'aquest dons no tindrem problemes.
		}
		PhysicsInit();
		Reset = false;
		Play_simulation = true;
	}

	
}
void PhysicsCleanup() {
	//TODO
}