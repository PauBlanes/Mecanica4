#pragma once

#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <vector>
#include <iostream>

#include "imgui\imgui.h"
#include <time.h>

//Boolean variables allow to show/hide the primitives
extern bool renderSphere;
extern bool renderParticles;
using namespace glm;
using namespace std;
struct coords { float x; float y; float z; };


namespace LilSpheres {
	extern const int maxParticles;
	extern void setupParticles(int numTotalParticles, float radius = 0.05f);
	extern void cleanupParticles();
	extern void updateParticles(int startIdx, int count, float* array_data);
	extern void drawParticles(int startIdx, int count);	

}
namespace Sphere {
	extern void setupSphere(glm::vec3 pos = glm::vec3(0.f, 1.f, 0.f), float radius = 1.f);
	extern void cleanupSphere();
	extern void updateSphere(glm::vec3 pos, float radius = 1.f);
	extern void drawSphere();
}

namespace ClothMesh {
	extern void setupClothMesh();
	extern void cleanupClothMesh();
	extern void updateClothMesh(float* array_data);
	extern void drawClothMesh();
}

extern float* partVerts;
extern GLfloat numWaves;
extern float gravity;

class Particle {
	friend class particleManager;
	friend class Esfera;

	vec3 position;
	vec3 inicalPos;
	vector <vec3> ks; //wave vector
	vector <GLfloat> frequencies;
	vector <GLfloat> As; //Amplitud
	vector <GLfloat> phis;
	GLfloat time;

public:
	Particle(vec3 pos);	
	void Move(float dt);	
};


class particleManager {
			
public:	
	GLfloat elasticCoef;
	GLfloat frictionCoef;
	
	GLfloat lHorizontal;
	GLfloat lVertical;
	
	std::vector<Particle> particles;
	void Update(GLfloat dt);
	
};

class Esfera {

	vec3 position;	
	vec3 velocity;
	vec3 force;
	GLfloat radius;
	GLfloat mass;
	GLfloat waterHeight;	
public:
	
	void SetVars(vec3 pos, GLfloat rad, GLfloat massa, GLfloat wH);
	void Update(GLfloat deltaTime);
	void CalculateBuoyancy();
	vec3 GetPos();
	GLfloat GetRadius();
	void SetMass(GLfloat m);
	void CalculateWaterHeight(vector <Particle> p);
};


