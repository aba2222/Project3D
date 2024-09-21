#pragma once
#include <math.h>
#include <DirectXMath.h>
#include <fstream>
#include "Math3D.h"
class SimObjectBase;

class ObjForces {
public:
	ObjForces(float S, int m, SimObjectBase* obj);
	ObjForces(const ObjForces&) = delete;
	~ObjForces();
	ObjForces& operator=(const ObjForces&) = delete;
	void Update();
	void SetMass(int mass);
	void SetThrust(float thrust);
	float GetGamma();
	float GetV();
	float GetV_Y();
	float GetVkmh();
	float GetLift();
	float GetP();
		
private:
	struct Forces {
		//Unit:N
		float thrust;
		double lift;
		double drag;
		float weight;
		double friction;
	};
	DirectX::XMFLOAT3 acceleration = { 0,0,0 };

	SimObjectBase* obj;
	Forces forces = { 0,0,0,0 };
	float thrustX = 0; // 推力在x轴上的分量
	float thrustY = 0; // 推力在y轴上的分量
	double CL = 0.6;
	double CD0 = 0.01;
	//m/s^2
	const float g = 9.81f;
	//m/s
	double V = 0;
	double V_Y = 0;
	//m^2
	float S;
	//kg/m^3
	//TODO:p is Variable
	float p;
	/// kg
	int m;
	//迎角
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

	std::ifstream file;
};