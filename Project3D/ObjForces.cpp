#include "ObjForces.h"
#include "SimObjectBase.h"

ObjForces::ObjForces(float S, int m, SimObjectBase* obj)
	: S(S),
	  m(m),
	  p(1.225f),
      gamma(0),
      OnGround(1),
	  dt(0.01667f),
	  e(0.825f),
	  AR(pow(34.1,2)/S),
	  ru(0.02f),
	  obj(obj) {
}

ObjForces::~ObjForces() {
	file.close();
}

void ObjForces::Update() {
	file = std::ifstream("Models\\a320_jh_DegenGeom.polar", std::ios::in);
	if (!file.is_open()) {
		throw std::runtime_error("OpenFile Error");
	}
	int targetLine = 11 * 10 + 1;
	targetLine += (int)(V / 340 * 10) * 11;
	targetLine += 5;
	std::string result;
	for (int i = 1; i <= targetLine + 1; i++) {
		std::getline(file, result);
	}
	std::string subStr = result.substr(69, 15);
	CL =  std::stof(subStr);
	subStr = result.substr(86, 15);
	CD0 = std::stof(subStr);
	file.close();

	thrustX = forces.thrust * cos(gamma);
	thrustY = forces.thrust * sin(gamma);
	forces.lift = 0.5 * p * pow(V, 2) * S * CL;

	forces.weight = m * g;

	// 计算阻力
	if (V != 0) {
		double drag_induced = (forces.lift * forces.lift) / (0.5 * p * V * V * S * PI * e * AR);
		double drag_parasite = 0.5 * p * V * V * S * CD0;
		forces.drag = drag_induced + drag_parasite;
	}
	else forces.drag = 0;

	// 计算加速度
	if (OnGround) {
		// 计算起落架摩擦力
		if (V > 0 && forces.weight > forces.lift)	forces.friction = ru * (forces.weight - forces.lift);
		else forces.friction = 0;

		acceleration.x = (thrustX - forces.drag - forces.friction) / m;
		acceleration.y = (thrustY + forces.lift - forces.weight) / m;
		acceleration.z = 0;

		if (acceleration.y < 0)	V_Y = 0;
		else V_Y += acceleration.y * dt;
	}
	else {
		acceleration.x = (thrustX - forces.drag) / m;
		acceleration.y = (thrustY + forces.lift - forces.weight) / m;
		acceleration.z = 0;

		if (V != 0) { gamma -= acceleration.x / V * 180.0 / PI;  /*更新爬升角度*/ };

		V_Y += acceleration.y * dt;
	}

	V += acceleration.x * dt;  // 更新速度

	obj->GetEarthPos()->UpdateLatLon(V, V_Y, 0, dt);
}

void ObjForces::SetMass(int mass) { m = mass; }
void ObjForces::SetThrust(float thrust) { forces.thrust = thrust; }
float ObjForces::GetGamma() { return gamma; }
float ObjForces::GetV() { return V; }
float ObjForces::GetV_Y() { return V_Y; }
float ObjForces::GetVkmh() { return V * 3.6; }
float ObjForces::GetLift() { return forces.lift; }
float ObjForces::GetP() { return p; }