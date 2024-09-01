#pragma once
#include <DirectXMath.h>
#include <memory>
#define DSMTD(D, M, S) ((D) + ((M) / 60.0) + ((S) / 3600.0))

class EarthPos {
public:
    EarthPos(float lon, float lat, float alt, float yaw, float pch, float rll)
        : longitude(lon), latitude(lat), altitude(alt),
        yaw(yaw), pitch(pch), roll(rll) {}

    float longitude;
    float latitude;
    float altitude;

    float roll;
    float pitch;
    float yaw;

    // ��������ڵ�ǰ EarthPos �ľֲ�����
    DirectX::XMFLOAT3 ToLocal(EarthPos* other) const {
        const float R = 6371000.0f;  // ����뾶����λ���ף�
        const float feetToMeters = 0.3048f;  // Ӣ��ת��Ϊ��

        // ���㾭γ�Ȳ���
        float latDiff = DirectX::XMConvertToRadians(other->latitude - latitude);
        float lonDiff = DirectX::XMConvertToRadians(other->longitude - longitude);
        float altDiff = (other->altitude - altitude) * feetToMeters;

        // ����ֲ�����
        float x = R * cos(DirectX::XMConvertToRadians(latitude)) * lonDiff;
        float y = altDiff;  // �߶Ȳ����Ϊ��λ��
        float z = R * latDiff;

        return DirectX::XMFLOAT3(x, y, z);
    }

    DirectX::XMFLOAT3 ToLocal(float lon, float lat, float alt) const {
        const float R = 6371000.0f;  // ����뾶����λ���ף�
        const float feetToMeters = 0.3048f;  // Ӣ��ת��Ϊ��

        // ���㾭γ�Ȳ���
        float latDiff = DirectX::XMConvertToRadians(lat - latitude);
        float lonDiff = DirectX::XMConvertToRadians(lon - longitude);
        float altDiff = (alt - altitude) * feetToMeters;

        // ����ֲ�����
        float x = R * cos(DirectX::XMConvertToRadians(latitude)) * lonDiff;
        float y = altDiff;  // �߶Ȳ����Ϊ��λ��
        float z = R * latDiff;

        return DirectX::XMFLOAT3(x, y, z);
    }
};
