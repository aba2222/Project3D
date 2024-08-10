#include "Camera.h"

DirectX::XMMATRIX Camera::GetMatrix() const noexcept {
	// 定义相机位置向量
	const auto pos = DirectX::XMVectorSet(x, y, z, 1.0f);

	// 计算相机目标位置
	const auto forward = DirectX::XMVector3Transform(
		DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
		DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll)
	);
	const auto target = DirectX::XMVectorAdd(pos, forward);

	// 返回视图矩阵，结合相机位置和旋转
	return DirectX::XMMatrixLookAtLH(
		pos,
		target,
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	);
}

DirectX::XMMATRIX Camera::GetProjectionMatrix(float aspectRatio) const noexcept {
	return DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(60.0f), // 视场角
		aspectRatio, // 宽高比
		0.1f, // 近平面
		1000.0f // 远平面（确保设置足够大）
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
