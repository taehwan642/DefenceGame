#pragma once
#include "framework.h"

class MyShader;

class Buffer
{
public:
	ID3D11InputLayout* vertexLayout;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	UINT stride;
	UINT offset;

	ID3D11Device* device;
	ID3D11DeviceContext* context;

	Buffer(ID3D11Device* dev, ID3D11DeviceContext* con, std::shared_ptr<MyShader> shader);
	~Buffer();

	HRESULT Initialize();

	void Update();
	void Render(ID3D11DeviceContext* context);

	__forceinline ID3D11InputLayout** GetVertexLayout();
	__forceinline ID3D11Buffer** GetVertexBuffer();
	__forceinline ID3D11Buffer** GetIndexBuffer();

	__forceinline const UINT* GetStride() const;
	__forceinline const UINT* GetOffset() const;
};

ID3D11InputLayout** Buffer::GetVertexLayout()
{
	return &vertexLayout;
}

ID3D11Buffer** Buffer::GetVertexBuffer()
{
	return &vertexBuffer;
}

ID3D11Buffer** Buffer::GetIndexBuffer()
{
	return &indexBuffer;
}

const UINT* Buffer::GetStride() const
{
	return &stride;
}

const UINT* Buffer::GetOffset() const
{
	return &offset;
}