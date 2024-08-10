#include "Camera.h"

DirectX::XMMATRIX Camera::GetMatrix() const noexcept {
	// �������λ������
	const auto pos = DirectX::XMVectorSet(x, y, z, 1.0f);

	// �������Ŀ��λ��
	const auto forward = DirectX::XMVector3Transform(
		DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
		DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll)
	);
	const auto target = DirectX::XMVectorAdd(pos, forward);

	// ������ͼ���󣬽�����λ�ú���ת
	return DirectX::XMMatrixLookAtLH(
		pos,
		target,
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	);
}

DirectX::XMMATRIX Camera::GetProjectionMatrix(float aspectRatio) const noexcept {
	return DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(60.0f), // �ӳ���
		aspectRatio, // ��߱�
		0.1f, // ��ƽ��
		1000.0f // Զƽ�棨ȷ�������㹻��
	);
	DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f);
}

DirectX::XMFLOAT3 Camera::GetPos() const noexcept {
	return DirectX::XMFLOAT3(x, y, z);
}

void Camera::SpawnControlWindow() noexcept {
	if (ImGui::Begin("Camera")) {
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &x, -100, 100);
		ImGui::SliderFloat("Y", &y, -100, 100);
		ImGui::SliderFloat("Z", &z, -100, 100);
		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);
		if (ImGui::Button("Reset")) {
			Reset();
		}
	}
	ImGui::End();
}

void Camera::Reset() noexcept {
	x = 0.0f;
	y = 0.0f;
	z = -30.0f;
	pitch = 0.0f;
	yaw = 0.0f;
	roll = 0.0f;
}
