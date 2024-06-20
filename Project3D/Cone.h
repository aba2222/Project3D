#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>
#include "Math3D.h"

class Cone {
public:
	template<class V>
	static IndexedTriangleList<V> MakeTesselated(int longDiv) {
		assert(longDiv >= 3);

		const auto base = DirectX::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
		const float longitubeAngle = 2.0f * PI / longDiv;

		std::vector<V> vertices;
		for (int iLong = 0; iLong < longDiv; iLong++) {
			vertices.emplace_back();
			auto v = DirectX::XMVector3Transform(base, DirectX::XMMatrixRotationZ(longitubeAngle * iLong));
			DirectX::XMStoreFloat3(&vertices.back().pos, v);
		}
		vertices.emplace_back();
		vertices.back().pos = { 0.0f,0.0f,-1.0f };
		const auto iCenter = (unsigned short)(vertices.size() - 1);
		vertices.emplace_back();
		vertices.back().pos = { 0.0f,0.0f,1.0f };
		const auto iTip = (unsigned short)(vertices.size() - 1);

		std::vector<unsigned short> indices;
		for (unsigned short iLong = 0; iLong < longDiv; iLong++) {
			indices.push_back(iCenter);
			indices.push_back((iLong + 1) % longDiv);
			indices.push_back(iLong);
		}
		for (unsigned short iLong = 0; iLong < longDiv; iLong++) {
			indices.push_back(iLong);
			indices.push_back((iLong + 1) % longDiv);
			indices.push_back(iTip);
		}
		return { std::move(vertices),std::move(indices) };
	}

	template<class V>
	static IndexedTriangleList<V> Make() {
		return MakeTesselated<V>(24);
	}
};