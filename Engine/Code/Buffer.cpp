#include "MyShader.h"
#include "Buffer.h"

Buffer::Buffer(ID3D11Device* dev, ID3D11DeviceContext* con, std::shared_ptr<MyShader> shader) :
	device(dev), context(con)
{
	device->AddRef();
	context->AddRef();
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

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
}

Buffer::~Buffer()
{
	if (vertexLayout) vertexLayout->Release();
	if (vertexBuffer) vertexBuffer->Release();
	if (indexBuffer) indexBuffer->Release();
	context->Release();
	device->Release();
}

void Buffer::Update()
{
}

void Buffer::Render(ID3D11DeviceContext* context)
{
}
