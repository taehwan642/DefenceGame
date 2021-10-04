#pragma once
#include "framework.h"

class Camera
{
	static Matrix View;
	static Matrix Projection;
public:
	static void Update();
	static void ProjectionUpdate();
	static Matrix GetViewMatrix();
	static Matrix GetProjectionMatrix();
};

