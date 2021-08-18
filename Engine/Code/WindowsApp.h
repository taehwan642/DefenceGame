#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

struct MyVertex
{
	XMFLOAT3 Pos;
};

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
	ID3D11VertexShader* vertexShader;
	ID3D11InputLayout* vertexLayout;
	ID3D11PixelShader* pixelShader;
	ID3D11Buffer* vertexBuffer;

public:
	__forceinline ~WindowsApp();
	__forceinline HRESULT CompileShaderFromFile(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** blobOut);

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

HRESULT WindowsApp::CompileShaderFromFile(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** blobOut)
{
	HRESULT hr = S_OK;
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* errorBlob = nullptr;
	if (FAILED(D3DX11CompileFromFile(fileName, 0, 0, entryPoint, shaderModel, shaderFlags, 0, 0, blobOut, &errorBlob, 0)))
	{
		if (nullptr != errorBlob)
		{
			OutputDebugStringA(static_cast<char*>(errorBlob->GetBufferPointer()));
			errorBlob->Release();
		}
		return E_FAIL;
	}
	if (nullptr != errorBlob)
		errorBlob->Release();

	return S_OK;
}

HRESULT WindowsApp::InitializeDevice(HWND hwnd)
{
	HRESULT hr = S_OK;

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

	ID3DBlob* vsBlob = nullptr;
	if (FAILED(CompileShaderFromFile(L"testShader.fx", "VS", "vs_4_0", &vsBlob)))
	{
		MessageBox(NULL, L"fx파일 컴파일 실패", L"error", MB_OK);
		return E_FAIL;
	}
	
	if (FAILED(device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &vertexShader)))
	{
		vsBlob->Release();
		return E_FAIL;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	UINT numElements = ARRAYSIZE(layout);

	if (FAILED(device->CreateInputLayout(layout, numElements, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &vertexLayout)))
	{
		vsBlob->Release();
		return E_FAIL;
	}

	vsBlob->Release();
	
	context->IASetInputLayout(vertexLayout);
	ID3DBlob* psBlob = nullptr;

	if (FAILED(CompileShaderFromFile(L"testShader.fx", "PS", "ps_4_0", &psBlob)))
	{
		MessageBox(NULL, L"fx 컴파일 실패", L"error", MB_OK);
		return E_FAIL;
	}

	if (FAILED(device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &pixelShader)))
	{
		psBlob->Release();
		return E_FAIL;
	}

	psBlob->Release();

	MyVertex vertices[] =
	{
		XMFLOAT3(0.0f, 0.5f, 0.5f),
		XMFLOAT3(0.5f, -0.5f, 0.5f),
		XMFLOAT3(-0.5f, -0.5f, 0.5f)
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(MyVertex) * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	
	if (FAILED(device->CreateBuffer(&bd, &InitData, &vertexBuffer)))
	{
		return E_FAIL;
	}

	UINT stride = sizeof(MyVertex);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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

	context->VSSetShader(vertexShader, 0, 0);
	context->PSSetShader(pixelShader, 0, 0);
	context->Draw(3, 0);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Hierarchy");
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	swapChain->Present(0, 0);
}
