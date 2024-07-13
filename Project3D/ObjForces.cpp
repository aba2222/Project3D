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

	//只有寄生阻力
	forces.drag =  0.5 * p * V * V * S * CD0;
	forces.weight = m * g;

	//TODO:阻力推力

	// 计算加速度
	acceleration.x = (thrustX - forces.drag) / m;
	acceleration.y = (forces.lift - forces.weight) / m;
	acceleration.z = (thrustZ - forces.weight) / m;

	V += acceleration.y;  // 更新速度
	gamma -= acceleration.x / V * 180.0 / PI;  // 更新爬升角度
}

void ObjForces::SetMass(int mass) { m = mass; }
float ObjForces::GetGamma() { return gamma; }