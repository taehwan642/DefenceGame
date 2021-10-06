#pragma once
#include "framework.h"

class Camera
{
	static Matrix View;
	static Matrix Projection;
	static float screenWidth;
	static float screenHeight;
public:
	static void Update();
	static void ProjectionUpdate();
	static Matrix GetViewMatrix();
	static Matrix GetProjectionMatrix();
};

