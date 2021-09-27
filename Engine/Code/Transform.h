#pragma once
#include "framework.h"

class Transform
{
public:
	float position[2];
	float scale[2];
	float rotation;
	
	__forceinline explicit Transform();

	__forceinline float* GetPositionPointer();
	__forceinline float* GetScalePointer();
	__forceinline float* GetRotationPointer();

	__forceinline Matrix GetMatrix() const;
};

Transform::Transform() : 
	position{ 0,0 }, scale{ 1,1 }, rotation(0)
{

}

float* Transform::GetPositionPointer()
{
	return position;
}

float* Transform::GetScalePointer()
{
	return scale;
}

float* Transform::GetRotationPointer()
{
	return &rotation;
}

Matrix Transform::GetMatrix() const
{
	Matrix world, s, r, t;
	s = DirectX::XMMatrixScaling(scale[0], scale[1], 1);
	r = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotation));
	t = DirectX::XMMatrixTranslation(position[0], position[1], 1);
	world = s * r * t;
	return world;
}
