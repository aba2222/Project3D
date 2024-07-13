#include "ObjForces.h"

ObjForces::ObjForces(float S, int m) 
	: S(S),
	  m(m),
	  p(1.225),
      gamma(0) {
}

void ObjForces::Update() {
	thrustX = forces.thrust * cos(gamma);
	thrustZ = forces.thrust * sin(gamma);

	//ֻ�м�������
	forces.drag =  0.5 * p * V * V * S * CD0;
	forces.weight = m * g;

	//TODO:��������

	// ������ٶ�
	acceleration.x = (thrustX - forces.drag) / m;
	acceleration.y = (forces.lift - forces.weight) / m;
	acceleration.z = (thrustZ - forces.weight) / m;

	V += acceleration.y;  // �����ٶ�
	gamma -= acceleration.x / V * 180.0 / PI;  // ���������Ƕ�
}

void ObjForces::SetMass(int mass) { m = mass; }
float ObjForces::GetGamma() { return gamma; }