#pragma once
#include "framework.h"

class Transform;
class MyShader;
class Texture;

class MyRectangle
{
private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;

	ID3D11InputLayout* vertexLayout;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	std::unique_ptr<MyShader> shader;
	std::shared_ptr<Transform> transform;
	std::shared_ptr<Texture> texture;

	UINT stride;
	UINT offset;
	
public:
	MyRectangle(ID3D11Device* dev, ID3D11DeviceContext* con);
	~MyRectangle();

	void Render();

	__forceinline ID3D11InputLayout* GetVertexLayout();
	__forceinline ID3D11Buffer* const* GetVertexBuffer();
	ID3D11VertexShader* GetVertexShader();
	ID3D11PixelShader* GetPixelShader();
	__forceinline const UINT* GetStride() const;
	__forceinline const UINT* GetOffset() const;

	std::shared_ptr<Transform> GetTransform();
};

ID3D11InputLayout* MyRectangle::GetVertexLayout()
{
	return vertexLayout;
}

ID3D11Buffer* const* MyRectangle::GetVertexBuffer()
{
	return &vertexBuffer;
}

const UINT* MyRectangle::GetStride() const
{
	return &stride;
}

const UINT* MyRectangle::GetOffset() const
{
	return &offset;
}