#include "Camera.h"
Matrix Camera::View;
Matrix Camera::Projection;

void Camera::Update()
{
	Vector Eye = DirectX::XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f);
	Vector At = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	Vector Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	View = DirectX::XMMatrixLookAtLH(Eye, At, Up);
}

void Camera::ProjectionUpdate()
{
	Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, 1280 / (FLOAT)720, 0.01f, 100.0f);
}

Matrix Camera::GetViewMatrix()
{
	return View;
}

Matrix Camera::GetProjectionMatrix()
{

	return Projection;
}
