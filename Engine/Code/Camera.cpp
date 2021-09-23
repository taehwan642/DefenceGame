#include "Camera.h"
XMMATRIX Camera::View;

void Camera::Update()
{
	XMVECTOR Eye = XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	View = XMMatrixLookAtLH(Eye, At, Up);
}

XMMATRIX Camera::GetViewMatrix()
{
	return View;
}
