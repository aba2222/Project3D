#pragma once
#include <math.h>
#include <DirectXMath.h>
#include "Math3D.h"

class ObjForces {
public:
	ObjForces(float S, int m);
	ObjForces(const ObjForces&) = delete;
	ObjForces& operator=(const ObjForces&) = delete;
	void Update();
	void SetMass(int mass);
	float GetGamma();

private:
	struct Forces {
		//Unit:N
		float thrust;
		float lift;
		float drag;
		float weight;
	};
	DirectX::XMFLOAT3 acceleration = { 0,0,0 };

	Forces forces = { 0,0,0,0 };
	float thrustX = 0; // 推力在x轴上的分量
	float thrustZ = 0; // 推力在z轴上的分量
	float CL = 2;
	//TODO:
	float CD0;
	//m/s^2
	const float g = 9.81f;
	//m/s
	float V = 0;
	//m^2
	float S;
	//kg/m^3
	//TODO:p is Variable
	float p;
	/// kg
	int m;
	//爬升角度
	float gamma;
};