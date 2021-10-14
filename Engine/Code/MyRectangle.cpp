#include "Camera.h"
#include "MyShader.h"
#include "Transform.h"
#include "Texture.h"
#include "Buffer.h"
#include "MyRectangle.h"

MyRectangle::MyRectangle(ID3D11Device* dev, ID3D11DeviceContext* con) :
	device(dev),
	context(con),
	shader(std::make_shared<MyShader>(dev)),
	transform(std::make_shared<Transform>()),
	texture(std::make_shared<Texture>()),
	buffer(std::make_shared<Buffer>(dev, con, shader))
{
	device->AddRef();
	context->AddRef();

	if (FAILED(texture->Initialize(device, L"Resources/seafloor.dds")))
	{
		MessageBox(NULL, L"텍스처 생성 실패", L"error", MB_OK);
		return;
	}

	Camera::ProjectionUpdate();
}

MyRectangle::~MyRectangle()
{
	device->Release();
	context->Release();
}

void MyRectangle::Render()
{
	ConstantBuffer cb;

	cb.mWorld = DirectX::XMMatrixTranspose(transform->GetMatrix());
	cb.mView = DirectX::XMMatrixTranspose(Camera::GetViewMatrix());
	cb.mProjection = DirectX::XMMatrixTranspose(Camera::GetProjectionMatrix());

	context->UpdateSubresource(shader->GetConstantBuffer(), 0, NULL, &cb, 0, 0);

	context->VSSetShader(shader->GetVertexShader(), 0, 0);
	context->VSSetConstantBuffers(0, 1, shader->GetConstantBuffer2());

	context->PSSetShader(shader->GetPixelShader(), 0, 0);
	context->PSSetConstantBuffers(0, 1, shader->GetConstantBuffer2());
	texture->Render(context);
	context->DrawIndexed(6, 0, 0);
}

ID3D11VertexShader* MyRectangle::GetVertexShader()
{
	return shader->GetVertexShader();
}

ID3D11PixelShader* MyRectangle::GetPixelShader()
{
	return shader->GetPixelShader();
}

std::shared_ptr<Buffer> MyRectangle::GetBuffer()
{
	return buffer;
}

std::shared_ptr<Transform> MyRectangle::GetTransform()
{
	return transform;
}