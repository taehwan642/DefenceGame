#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

class WindowsApp
{
private:
	D3D_DRIVER_TYPE	driverType = D3D_DRIVER_TYPE_NULL;
	IDXGISwapChain* swapChain = nullptr;
	// device, featurelevel, context
	ID3D11Device* device = nullptr;
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11DeviceContext* context = nullptr;
	ID3D11RenderTargetView* renderTargetView;
	HWND handleWindow;

public:
	__forceinline ~WindowsApp();

	__forceinline HRESULT InitializeDevice(HWND hwnd);
	void Update();
	void Render();
};

WindowsApp::~WindowsApp()
{
	if (context) context->ClearState();

	if (renderTargetView) renderTargetView->Release();
	if (swapChain) swapChain->Release();
	if (context) context->Release();
	if (device) device->Release();
}

HRESULT WindowsApp::InitializeDevice(HWND hwnd)
{
	HRESULT hr = S_OK;

	handleWindow = hwnd;

	RECT rc;
	GetClientRect(hwnd, &rc);

	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT deviceFlags = 0;
#ifdef _DEBUG
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for (unsigned int driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex)
	{
		driverType = driverTypes[driverTypeIndex];

		hr = D3D11CreateDeviceAndSwapChain(NULL, driverType, NULL, deviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &swapChain, &device, &featureLevel, &context);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	ID3D11Texture2D* backBuffer = nullptr;
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
		return E_FAIL;

	hr = device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
	backBuffer->Release();
	if (FAILED(hr))
		return E_FAIL;

	context->OMSetRenderTargets(1, &renderTargetView, NULL);

	D3D11_VIEWPORT vp;
	vp.Width = static_cast<float>(width);
	vp.Height = static_cast<float>(height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	context->RSSetViewports(1, &vp);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device, context);
	ImGui::StyleColorsDark();
}

void WindowsApp::Update()
{
}

void WindowsApp::Render()
{
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	context->ClearRenderTargetView(renderTargetView, ClearColor);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("test");
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	swapChain->Present(0, 0);
}
