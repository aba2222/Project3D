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
	float thrustX = 0; // ������x���ϵķ���
	float thrustY = 0; // ������y���ϵķ���
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
	//�����Ƕ�
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
};