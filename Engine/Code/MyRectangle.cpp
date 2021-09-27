#include "MyShader.h"
#include "Camera.h"
#include "MyRectangle.h"
Matrix MyRectangle::Projection;

void MyRectangle::Render()
{
	ConstantBuffer cb;

	cb.mWorld = DirectX::XMMatrixTranspose(transform->GetMatrix());
	cb.mView = DirectX::XMMatrixTranspose(Camera::GetViewMatrix());
	cb.mProjection = DirectX::XMMatrixTranspose(Projection);

	context->UpdateSubresource(shader->GetConstantBuffer(), 0, NULL, &cb, 0, 0);

	context->VSSetShader(shader->GetVertexShader(), 0, 0);
	context->VSSetConstantBuffers(0, 1, shader->GetConstantBuffer2());

	context->PSSetShader(shader->GetPixelShader(), 0, 0);
	context->DrawIndexed(6, 0, 0);
}