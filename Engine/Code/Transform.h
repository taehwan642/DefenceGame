#pragma once
#include "framework.h"

class Transform
{

public:
	float position[2];
	float scale[2];
	float rotation;
	
	__forceinline explicit Transform();

	__forceinline float* GetPosition();
	__forceinline float* GetScale();
	__forceinline float* GetRotation();

	__forceinline XMMATRIX GetMatrix() const;
};

Transform::Transform() : 
	position{ 0,0 }, scale{ 1,1 }, rotation(0)
{

}

float* Transform::GetPosition()
{
	return position;
}

float* Transform::GetScale()
{
	return scale;
}

float* Transform::GetRotation()
{
	return &rotation;
}

XMMATRIX Transform::GetMatrix() const
{
	XMMATRIX world, s, r, t;
	s = XMMatrixScaling(scale[0], scale[1], 1);
	r = XMMatrixRotationZ(XMConvertToRadians(rotation));
	t = XMMatrixTranslation(position[0], position[1], 1);
	world = s * r * t;
	return world;
}
