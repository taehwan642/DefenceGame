#include "Camera.h"
Matrix Camera::View;

void Camera::Update()
{
	Vector Eye = DirectX::XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
	Vector At = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	Vector Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	View = DirectX::XMMatrixLookAtLH(Eye, At, Up);
}

Matrix Camera::GetViewMatrix()
{
	return View;
}
