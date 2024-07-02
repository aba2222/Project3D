#pragma once
#include "ExceptionHandle.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include <random>
#include <Windows.h>
#include <wrl.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

// 使用模板别名(C++11)简化类型名
template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

class Graphics {
	friend class Bindable;
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
	void EndFrame();
	void DrawIndexed(UINT indexCount);
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	void ImguiSwitch() noexcept;
	void ImguiSwitch(bool status) noexcept;
	bool ImguiStatus() noexcept;
	void BeginFrame(float r, float g, float b);
	DirectX::XMMATRIX GetProjection() const noexcept;

private:
	bool imguiStatus = 1;
	DirectX::XMMATRIX projection;

	ComPtr<ID3D11Device> pDevice = nullptr;
	ComPtr<IDXGISwapChain> pSwap = nullptr;
	ComPtr<ID3D11DeviceContext> pContext = nullptr;
	ComPtr<ID3D11RenderTargetView> pTarget = nullptr;
	ComPtr<ID3D11DepthStencilView> pDSV;
	D3D_FEATURE_LEVEL featureLevel;
};

