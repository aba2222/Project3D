#pragma once
#include <string>
#include "Graphics.h"
#include "TransformCbuf.h"
#include "DrawableBase.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class AssModel : public DrawableBase<AssModel> {
public:
	AssModel(Graphics& gfx, std::string fileName, DirectX::XMFLOAT3 material, float scale);
	DirectX::XMMATRIX GetTransformXM() const noexcept;
	void Update(float dt) noexcept;

private:

};