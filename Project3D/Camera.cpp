#include "Camera.h"

Camera::Camera() {
	pos = std::make_unique<EarthPos>(DSMTD(121.0f, 27.8f, 0.0f), DSMTD(29.0f, 49.6f, 00.0f), 100.0f, 0.0f, 0.0f, 0.0f);
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept {
	if (focus != nullptr) {
		pos->latitude = focus->GetEarthPos()->latitude;
		pos->longitude = focus->GetEarthPos()->longitude;
		pos->altitude = focus->GetEarthPos()->altitude;
	}

	// �������λ������
	const auto pos_t = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	// �������Ŀ��λ��
	const auto forward = DirectX::XMVector3Transform(
		DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
		DirectX::XMMatrixRotationRollPitchYaw(pos->pitch, pos->yaw, pos->roll)
	);
	const auto target = DirectX::XMVectorAdd(pos_t, forward);

	// ������ͼ���󣬽�����λ�ú���ת
	return DirectX::XMMatrixLookAtLH(
		pos_t,
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

void Camera::SetFocus(SimObjectBase* newF) {
	focus = newF;
}

void Camera::SpawnControlWindow() noexcept {
	if (ImGui::Begin("Camera")) {
		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &(pos->roll), -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &(pos->pitch), -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &(pos->yaw), -180.0f, 180.0f);
		if (ImGui::Button("Reset")) {
			Reset();
		}
	}
	ImGui::End();
}

void Camera::Update(float dt) noexcept {
}

void Camera::Reset() noexcept {
	pos->latitude = DSMTD(121.0f, 27.8f, 0.0f);
	pos->longitude = DSMTD(29.0f, 49.6f, 00.0f);
	pos->altitude = 100.0f;
	pos->pitch = 0.0f;
	pos->yaw = 0.0f;
	pos->roll = 0.0f;
}
