#pragma once
#include <DirectXMath.h>
#include <memory>
#include "Math3D.h"
#define DSMTD(D, M, S) ((D) + ((M) / 60.0) + ((S) / 3600.0))

class EarthPos {
public:
    EarthPos(float lon, float lat, float alt, float yaw, float pch, float rll)
        : longitude(lon), latitude(lat), altitude(alt),
        yaw(yaw), pitch(pch), roll(rll) {}

    double longitude;
    double latitude;
    double altitude;

    float roll;
    float pitch;
    float yaw;

    // 计算相对于当前 EarthPos 的局部坐标
    DirectX::XMFLOAT3 ToLocal(EarthPos* other) const {
        const float R = 6371000.0f;  // 地球半径（单位：米）
        const float feetToMeters = 0.3048f;  // 英尺转换为米

        // 计算经纬度差异
        float latDiff = DirectX::XMConvertToRadians(other->latitude - latitude);
        float lonDiff = DirectX::XMConvertToRadians(other->longitude - longitude);
        float altDiff = (other->altitude - altitude) * feetToMeters;

        // 计算局部坐标
        float x = R * cos(DirectX::XMConvertToRadians(latitude)) * lonDiff;
        float y = altDiff;  // 高度差（以米为单位）
        float z = R * latDiff;

        return DirectX::XMFLOAT3(x, y, z);
    }

    DirectX::XMFLOAT3 ToLocal(float lon, float lat, float alt) const {
        const float R = 6371000.0f;
        const float feetToMeters = 0.3048f;

        float latDiff = DirectX::XMConvertToRadians(lat - latitude);
        float lonDiff = DirectX::XMConvertToRadians(lon - longitude);
        float altDiff = (alt - altitude) * feetToMeters;

        float x = R * cos(DirectX::XMConvertToRadians(latitude)) * lonDiff;
        float y = altDiff;
        float z = R * latDiff;

        return DirectX::XMFLOAT3(x, y, z);
    }

    void UpdateLatLon(double vx_local, double vy_local, double vz_local, float dt) {
        double vx_global, vy_global, vz_global;

        double cosPitch = cos(pitch);
        double sinPitch = sin(pitch);
        double cosRoll = cos(roll);
        double sinRoll = sin(roll);
        double cosYaw = cos(yaw);
        double sinYaw = sin(yaw);

        vx_global = (cosYaw * cosPitch) * vx_local +
            (cosYaw * sinPitch * sinRoll - sinYaw * cosRoll) * vy_local +
            (cosYaw * sinPitch * cosRoll + sinYaw * sinRoll) * vz_local;

        vy_global = (sinYaw * cosPitch) * vx_local +
            (sinYaw * sinPitch * sinRoll + cosYaw * cosRoll) * vy_local +
            (sinYaw * sinPitch * cosRoll - cosYaw * sinRoll) * vz_local;

        vz_global = (-sinPitch) * vx_local +
            (cosPitch * sinRoll) * vy_local +
            (cosPitch * cosRoll) * vz_local;

        double dx = vx_global * dt;
        double dy = vy_global *dt;
        double dz = vz_global * dt;

        // 计算纬度和经度变化
        double dLat = dz / EARTH_RADIUS;  // 纬度变化
        double dLon = dx / (EARTH_RADIUS * cos(latitude));  // 经度变化

        latitude += dLat;
        longitude += dLon;
        altitude += dy * 3.28084;
    }
};