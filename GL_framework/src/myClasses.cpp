
#include "myClasses.h"


int NumPrticles = 0;
GLfloat numWaves = 2;

float* partVerts = new float[LilSpheres::maxParticles * 3];
float gravity;



Particle::Particle(vec3 pos) {

	inicalPos = pos;
	position = pos;
	time = 0;
	
	ks.push_back({ 1,0,0 });
	ks.push_back({ 0,0,-2});
	As.push_back(0.5);
	As.push_back(0.25);
	frequencies.push_back(1/3.141516);
	frequencies.push_back(1/(2*3.141516));
	phis.push_back(0.25);
	phis.push_back(0.5);
	
};
void Particle::Move(float dt) {

	time += dt;	
	vec3 sumatoriXZ;
	GLfloat sumatoriY = 0;

	//Fem el sumatori
	for (int i = 0; i < 2;i++) {
		GLfloat w = 2 * 3.141516*frequencies[i];
		sumatoriXZ += normalize(ks[i])*As[i] * sin(dot(ks[i], inicalPos) - w*time + phis[i]);
		sumatoriY += As[i] * cos(dot(ks[i], inicalPos) - w*time + phis[i]);
	}

	position.x = (inicalPos - sumatoriXZ).x;
	position.y = inicalPos.y + sumatoriY;
	position.z = (inicalPos - sumatoriXZ).z;
}
//MANAGER
void particleManager::Update(GLfloat dt) {

	//actualitzar el array de vertexs
	for (int i = 0; i < particles.size(); ++i) {
	
		particles[i].Move(dt);

		partVerts[i * 3 + 0] = particles[i].position.x;
		partVerts[i * 3 + 1] = particles[i].position.y;
		partVerts[i * 3 + 2] = particles[i].position.z;
	}
}

void Esfera::SetVars(vec3 pos, GLfloat rad, GLfloat massa, GLfloat wH) {
	position = pos;
	velocity = vec3(0);
	radius = rad;
	mass = massa;
	waterHeight = wH;
}

void Esfera::Update(GLfloat deltaTime) {
	velocity += deltaTime * (force / mass);
	position += velocity*deltaTime;
	force.y = gravity*mass;
	
}

void Esfera::CalculateBuoyancy() {
	if (position.y - radius < waterHeight) {		
		GLfloat volumeUnderwater = pow(radius*2,3) * clamp(((waterHeight-(position.y-radius)/(radius*2))), 0.f, 1.f);
		vec3 Fbuoyancy = 5.f * 9.8f * volumeUnderwater * vec3(0,1,0);
		force += Fbuoyancy;
	}
}

vec3 Esfera::GetPos() {
	return position;
}


GLfloat Esfera::GetRadius() {
	return radius;
}

void Esfera::SetMass(GLfloat m) {
	mass = m;
}

void Esfera::CalculateWaterHeight(vector <Particle> p) {
	//les alçades de les 4 particules
	Particle nearestXP = p[0];
	Particle nearestX2P = p[0];
	Particle nearestZP = p[0];
	Particle nearestZ2P = p[0];

	//trobar les alçades de les 4 mes properes
	for (int i = 0; i < p.size();i++) {
		//Comprovacions en X
		if (abs(p[i].position.x - position.x) < abs(nearestX2P.position.x - position.x)) { //si esta mes a prop que la segona mes propera
			nearestX2P = p[i];
			if (abs(p[i].position.x - position.x) < abs(nearestXP.position.x - position.x)) { //si esta mes a prop que la mes propera
				nearestX2P = nearestXP;
				nearestXP = p[i];
			}
		}
		//Comprovacions en Z
		if (abs(p[i].position.z - position.z) < abs(nearestZ2P.position.z - position.z)) { //si esta mes a prop que la segona mes propera
			nearestZ2P = p[i];
			if (abs(p[i].position.z - position.z) < abs(nearestZP.position.z - position.z)) { //si esta mes a prop que la mes propera
				nearestZ2P = nearestZP;
				nearestZP = p[i];
			}
		}
	}

	//fem la mitja per obtenir una alçada de l'aigua més precisa
	waterHeight = (nearestXP.position.y + nearestX2P.position.y + nearestZP.position.y + nearestZ2P.position.y) / 4;	
}








