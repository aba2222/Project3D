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
	void SetThrust(float thrust);
	float GetGamma();
	float GetV();
	float GetVkmh();
	float GetLift();
		
private:
	struct Forces {
		//Unit:N
		float thrust;
		float lift;
		float drag;
		float weight;
		float friction;
	};
	DirectX::XMFLOAT3 acceleration = { 0,0,0 };

	Forces forces = { 0,0,0,0 };
	float thrustX = 0; // 推力在x轴上的分量
	float thrustY = 0; // 推力在y轴上的分量
	float CL = 2;
	//TODO:
	float CD0 = 0.01;
	//m/s^2
	const float g = 9.81f;
	//m/s
	float V = 0;
	float V_Y = 0;
	//m^2
	float S;
	//kg/m^3
	//TODO:p is Variable
	float p;
	/// kg
	int m;
	//爬升角度
	float gamma;
	bool OnGround;
	//时间步长
	float dt;
	//机翼效率因子
	float e;
	//展弦比
	float AR;
	//滚动摩擦系数
	float ru;
};