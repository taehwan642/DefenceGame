#pragma once
#include "framework.h"

class Transform
{

public:
	XMFLOAT2 position;
	XMFLOAT2 scale;
	float rotation;
	
	__forceinline explicit Transform();

	__forceinline XMMATRIX GetMatrix() const;
};

Transform::Transform() : position(0, 0), scale(1, 1), rotation(0)
{

}

XMMATRIX Transform::GetMatrix() const
{
	XMMATRIX world, s, r, t;
	s = XMMatrixScaling(scale.x, scale.y, 1);
	r = XMMatrixRotationZ(rotation);
	t = XMMatrixTranslation(position.x, position.y, 1);
	world = s * r * t;
	return world;
}
