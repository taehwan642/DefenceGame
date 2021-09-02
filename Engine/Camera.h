#pragma once
#include "framework.h"

class Camera
{
	XMMATRIX View;
public:
	void Update();
	void GetMatrix();
};

