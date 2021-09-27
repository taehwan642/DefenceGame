#pragma once
#include "framework.h"

class Camera
{
	static Matrix View;
public:
	static void Update();
	static Matrix GetViewMatrix();
};

