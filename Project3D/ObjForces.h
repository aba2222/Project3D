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
	float thrustX = 0; // ������x���ϵķ���
	float thrustY = 0; // ������y���ϵķ���
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
	//ӭ��
	float gamma;
	bool OnGround;
	//ʱ�䲽��
	float dt;
	//����Ч������
	float e;
	//չ�ұ�
	float AR;
	//����Ħ��ϵ��
	float ru;

	std::ifstream file;
};