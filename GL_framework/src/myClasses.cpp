
#include "myClasses.h"


int NumPrticles = 0;
GLfloat numWaves = 2;

float* partVerts = new float[LilSpheres::maxParticles * 3];

Particle::Particle(vec3 pos) {

	inicalPos = pos;
	position = pos;
	time = 0;
	
	ks.push_back({ 1,0,0 });
	ks.push_back({ 0,0,-3 });
	As.push_back(0.5);
	As.push_back(0.25);
	frequencies.push_back(1/3.141516);
	frequencies.push_back(1/(2*3.141516));
	phis.push_back(0);
	phis.push_back(0.25);
	
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
/*
void Particle::DetectWall(vec3 n, int d, float dt) {

	//calculem quina seria la seva seguent posicio
	vec3 posCreuada = position + dt*velocity;

	//si estan una a cada banda del pla fem el rebot
	if ((dot(n, position) + d) * (dot(n, posCreuada) + d) <= 0) {

		//calculem la nova posicio
		position = posCreuada - (1 + elasticCoef) * (dot(n, posCreuada) + d)*n;

		//calculem la nova velocitat
		float VperN = dot(velocity, n); // v*n
		//elasticidad
		velocity += -(1 + elasticCoef)*(n*VperN);
		//friccion
		vec3 vN = dot(velocity, n)*n;
		velocity += -frictionCoef * (velocity - vN); //-u*vT	

	}
}

void Particle::DetectSphere(vec3 centreEsfera, float radius, float dt) {
	//calculem quina seria la seva seguent posicio
	vec3 posCreuada = position + dt*velocity;;

	vec3 distVector = posCreuada - centreEsfera;

	float dist = length(distVector);

	if (dist < radius) {
		//trobem el punt d'interseccio
		vec3 l = normalize(velocity); //normalitzem velocitat per fer la recta que surt de PosActual i va en dir de la velocitat
		float distIntersec = -dot(l, (position - centreEsfera)) - sqrt((dot(l, (position - centreEsfera)))*(dot(l, (position - centreEsfera))) - ((length(position - centreEsfera))*(length(position - centreEsfera))) + (radius*radius));
		vec3 intersectionPoint = position + l*distIntersec;
		//comprovem si el punt d'interseccio que hem calculat esta entre pos i posCreuada
		if (length(position - intersectionPoint) + length(intersectionPoint - posCreuada) != length(position - posCreuada))
			distIntersec = -dot(l, (position - centreEsfera)) + sqrt((dot(l, (position - centreEsfera)))*(dot(l, (position - centreEsfera))) - ((length(position - centreEsfera))*(length(position - centreEsfera))) + (radius*radius));

		//vector interseccio-centre sera la normal del pla
		vec3 n = normalize(intersectionPoint - centreEsfera);

		//calcular d del pla i pos de rebot
		float d = -dot(n, intersectionPoint);
		position = posCreuada - 2 * (dot(n, posCreuada) + d)*n;

		//elasticitat			
		velocity += -(1 + elasticCoef)*(n*dot(n, velocity));

		//friccion
		vec3 vN = dot(n, velocity)*n;
		velocity += -frictionCoef * (velocity - vN); //velocity = velocity -u*vT

	}
}*/



//MANAGER
void particleManager::Update(GLfloat dt) {

	//actualitzar el array de vertexs
	for (int i = 0; i < particles.size(); ++i) {
		//colisio murs
		//for (int j = 0; j < 6;j++) {
		//	particles[i].DetectWall(wallNormals[j], wallDs[j], dt);
		//}		

		particles[i].Move(dt);
		

		partVerts[i * 3 + 0] = particles[i].position.x;
		partVerts[i * 3 + 1] = particles[i].position.y;
		partVerts[i * 3 + 2] = particles[i].position.z;

	}
}







