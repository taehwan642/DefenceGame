#include "Camera.h"
#include "MyShader.h"
#include "Transform.h"
#include "Texture.h"
#include "MyRectangle.h"

MyRectangle::MyRectangle(ID3D11Device* dev, ID3D11DeviceContext* con) :
	device(dev),
	context(con),
	shader(std::make_unique<MyShader>(dev)),
	transform(std::make_shared<Transform>()),
	texture(std::make_unique<Texture>())
{
	device->AddRef();
	context->AddRef();
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	if (FAILED(texture->Initialize(device, L"Resources/seafloor.dds")))
	{
		MessageBox(NULL, L"텍스처 생성 실패", L"error", MB_OK);
		return;
	}

	if (FAILED(device->CreateInputLayout(layout, numElements, shader->GetVertexShaderBlob()->GetBufferPointer(), shader->GetVertexShaderBlob()->GetBufferSize(), &vertexLayout)))
	{
		MessageBox(NULL, L"레이아웃 인풋 생성 실패", L"error", MB_OK);
		return;
	}

	context->IASetInputLayout(vertexLayout);

	MyVertex vertices[] =
	{
		{ Float3(-1.0f, -1.0f, 1.0f), Float2(0.f, 1.f) },
		{ Float3(-1.0f, 1.0f, 1.0f), Float2(0.f, 0.f) },
		{ Float3(1.0f, 1.0f, 1.0f),Float2(1.f, 0.f) },
		{ Float3(1.0f, -1.0f, 1.0f), Float2(1.f, 1.f) },
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(MyVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;

	if (FAILED(device->CreateBuffer(&bd, &InitData, &vertexBuffer)))
	{
		return;
	}

	stride = sizeof(MyVertex);
	offset = 0;

	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	WORD indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	// 위에서 쓴 bd의 정보를 덮어쓰고 돌려쓰기
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 6;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;
	if (FAILED(device->CreateBuffer(&bd, &InitData, &indexBuffer)))
		return;

	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	Camera::ProjectionUpdate();
}

MyRectangle::~MyRectangle()
{
	device->Release();
	context->Release();
	if (vertexLayout) vertexLayout->Release();
	if (vertexBuffer) vertexBuffer->Release();
	if (indexBuffer) indexBuffer->Release();
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

std::shared_ptr<Transform> MyRectangle::GetTransform()
{
	return transform;
}