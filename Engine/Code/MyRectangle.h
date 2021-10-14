#pragma once
#include "framework.h"

class Transform;
class MyShader;
class Texture;
class Buffer;

class MyRectangle
{
private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;

	// Buffer를 가지는 클래스(컴포넌트)를 하나 만들기.
	std::shared_ptr<MyShader> shader;
	std::shared_ptr<Buffer> buffer;
	std::shared_ptr<Transform> transform;
	std::shared_ptr<Texture> texture;
	
public:
	MyRectangle(ID3D11Device* dev, ID3D11DeviceContext* con);
	~MyRectangle();

	void Render();

	ID3D11VertexShader* GetVertexShader();
	ID3D11PixelShader* GetPixelShader();

	std::shared_ptr<Buffer> GetBuffer();
	std::shared_ptr<Transform> GetTransform();
};

