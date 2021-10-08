#pragma once
#include "framework.h"

class Buffer
{
public:
	ID3D11InputLayout* vertexLayout;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	~Buffer();

	void Update();
	void Render(ID3D11DeviceContext* context);
};

