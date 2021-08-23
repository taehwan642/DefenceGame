#pragma once

struct MyVertex
{
	XMFLOAT3 Pos;
};

class MyRectangle
{
private:
	ID3D11InputLayout* vertexLayout;
	ID3D11Buffer* vertexBuffer;

	std::unique_ptr<MyShader> shader;

	UINT stride;
	UINT offset;

public:
	ID3D11Device* device;
	__forceinline MyRectangle(ID3D11Device* dev);
	__forceinline ~MyRectangle();

	__forceinline ID3D11InputLayout* GetVertexLayout();
	__forceinline ID3D11Buffer* const* GetVertexBuffer();
	__forceinline ID3D11VertexShader* GetVertexShader();
	__forceinline ID3D11PixelShader* GetPixelShader();
	__forceinline const UINT* GetStride();
	__forceinline const UINT* GetOffset();
};


MyRectangle::MyRectangle(ID3D11Device* dev) : device(dev), shader(std::make_unique<MyShader>(dev))
{

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	UINT numElements = ARRAYSIZE(layout);

	if (FAILED(device->CreateInputLayout(layout, numElements, shader->GetVertexShaderBlob()->GetBufferPointer(), shader->GetVertexShaderBlob()->GetBufferSize(), &vertexLayout)))
	{
		MessageBox(NULL, L"·¹ÀÌ¾Æ¿ô ÀÎÇ² »ý¼º ½ÇÆÐ", L"error", MB_OK);
		return;
	}

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
		return;
	}

	stride = sizeof(MyVertex);
	offset = 0;
}

MyRectangle::~MyRectangle()
{
	device->Release();
	if (vertexLayout) vertexLayout->Release();
	if (vertexBuffer) vertexBuffer->Release();
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
