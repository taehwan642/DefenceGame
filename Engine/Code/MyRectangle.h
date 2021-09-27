#pragma once
#include "framework.h"
#include "MyShader.h"
#include "Transform.h"

class MyShader;

class MyRectangle
{
private:
	static Matrix Projection;

	ID3D11Device* device;
	ID3D11DeviceContext* context;

	ID3D11InputLayout* vertexLayout;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	std::unique_ptr<MyShader> shader;
	std::shared_ptr<Transform> transform;

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
	__forceinline const UINT* GetStride() const;
	__forceinline const UINT* GetOffset() const;

	__forceinline std::shared_ptr<Transform> GetTransform();
};


MyRectangle::MyRectangle(ID3D11Device* dev, ID3D11DeviceContext* con) : 
	device(dev), 
	context(con), 
	shader(std::make_unique<MyShader>(dev)), 
	transform(std::make_shared<Transform>())
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
		{ Float3(-1.0f, -1.0f, 1.0f), Float4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ Float3(-1.0f, 1.0f, 1.0f), Float4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ Float3(1.0f, 1.0f, 1.0f), Float4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ Float3(1.0f, -1.0f, 1.0f), Float4(1.0f, 0.0f, 0.0f, 1.0f) },
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

	Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, 1280 / (FLOAT)720, 0.01f, 100.0f);
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

const UINT* MyRectangle::GetStride() const
{
	return &stride;
}

const UINT* MyRectangle::GetOffset() const
{
	return &offset;
}

std::shared_ptr<Transform> MyRectangle::GetTransform()
{
	return transform;
}