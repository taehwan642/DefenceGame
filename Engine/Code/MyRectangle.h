#pragma once
#include "framework.h"
#include "MyShader.h"

class MyShader;

class MyRectangle
{
private:
	static XMMATRIX World;
	static XMMATRIX View;
	static XMMATRIX Projection;

	ID3D11Device* device;
	ID3D11DeviceContext* context;

	ID3D11InputLayout* vertexLayout;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	std::unique_ptr<MyShader> shader;

	UINT stride;
	UINT offset;
	
public:

	__forceinline MyRectangle(ID3D11Device* dev, ID3D11DeviceContext* con);
	__forceinline ~MyRectangle();

	void Render();

	__forceinline ID3D11InputLayout* GetVertexLayout();
	__forceinline ID3D11Buffer* const* GetVertexBuffer();
	__forceinline ID3D11VertexShader* GetVertexShader();
	__forceinline ID3D11PixelShader* GetPixelShader();
	__forceinline const UINT* GetStride();
	__forceinline const UINT* GetOffset();
};


MyRectangle::MyRectangle(ID3D11Device* dev, ID3D11DeviceContext* con) : device(dev), context(con), shader(std::make_unique<MyShader>(dev))
{
	device->AddRef();
	context->AddRef();
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};
	UINT numElements = ARRAYSIZE(layout);

	if (FAILED(device->CreateInputLayout(layout, numElements, shader->GetVertexShaderBlob()->GetBufferPointer(), shader->GetVertexShaderBlob()->GetBufferSize(), &vertexLayout)))
	{
		MessageBox(NULL, L"레이아웃 인풋 생성 실패", L"error", MB_OK);
		return;
	}

	context->IASetInputLayout(vertexLayout);

	MyVertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(MyVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;

	if (FAILED(device->CreateBuffer(&bd, &InitData, &vertexBuffer)))
	{
		return;
	}

	stride = sizeof(MyVertex);
	offset = 0;

	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	WORD indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	// 위에서 쓴 bd의 정보를 덮어쓰고 돌려쓰기
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 6;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;
	if (FAILED(device->CreateBuffer(&bd, &InitData, &indexBuffer)))
		return;

	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	World = XMMatrixIdentity();
	
	XMVECTOR Eye = XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	View = XMMatrixLookAtLH(Eye, At, Up);

	Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, 1280 / (FLOAT)720, 0.01f, 100.0f);
}

MyRectangle::~MyRectangle()
{
	device->Release();
	context->Release();
	if (vertexLayout) vertexLayout->Release();
	if (vertexBuffer) vertexBuffer->Release();
	if (indexBuffer) indexBuffer->Release();
}



ID3D11InputLayout* MyRectangle::GetVertexLayout()
{
	return vertexLayout;
}

ID3D11Buffer* const* MyRectangle::GetVertexBuffer()
{
	return &vertexBuffer;
}

ID3D11VertexShader* MyRectangle::GetVertexShader()
{
	return shader->GetVertexShader();
}

ID3D11PixelShader* MyRectangle::GetPixelShader()
{
	return shader->GetPixelShader();
}

const UINT* MyRectangle::GetStride()
{
	return &stride;
}

const UINT* MyRectangle::GetOffset()
{
	return &offset;
}
