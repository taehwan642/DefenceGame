#pragma once
#include "framework.h"

class Camera
{
	static XMMATRIX View;
public:
	static void Update();
	static XMMATRIX GetViewMatrix();
};

