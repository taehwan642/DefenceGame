#pragma once

struct MyVertex
{
	XMFLOAT3 Pos;
};

class MyRectangle
{
private:
	ID3D11VertexShader* vertexShader;
	ID3D11InputLayout* vertexLayout;
	ID3D11PixelShader* pixelShader;
	ID3D11Buffer* vertexBuffer;
	UINT stride;
	UINT offset;

public:
	ID3D11Device* device;
	__forceinline MyRectangle(ID3D11Device* dev);
	__forceinline ~MyRectangle();

	__forceinline HRESULT CompileShaderFromFile(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** blobOut);

	__forceinline ID3D11InputLayout* GetVertexLayout();
	__forceinline ID3D11Buffer* const* GetVertexBuffer();
	__forceinline ID3D11VertexShader* GetVertexShader();
	__forceinline ID3D11PixelShader* GetPixelShader();
	__forceinline const UINT* GetStride();
	__forceinline const UINT* GetOffset();
};


MyRectangle::MyRectangle(ID3D11Device* dev) : device(dev)
{
	device->AddRef();
	ID3DBlob* vsBlob = nullptr;
	if (FAILED(CompileShaderFromFile(L"testShader.fx", "VS", "vs_5_0", &vsBlob)))
	{
		MessageBox(NULL, L"fx파일 컴파일 실패", L"error", MB_OK);
		return;
	}

	if (FAILED(device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &vertexShader)))
	{
		vsBlob->Release();
		return;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	UINT numElements = ARRAYSIZE(layout);

	if (FAILED(device->CreateInputLayout(layout, numElements, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &vertexLayout)))
	{
		vsBlob->Release();
		return;
	}

	vsBlob->Release();

	ID3DBlob* psBlob = nullptr;

	if (FAILED(CompileShaderFromFile(L"testShader.fx", "PS", "ps_5_0", &psBlob)))
	{
		MessageBox(NULL, L"fx 컴파일 실패", L"error", MB_OK);
		return;
	}

	if (FAILED(device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &pixelShader)))
	{
		psBlob->Release();
		return;
	}

	psBlob->Release();

	MyVertex vertices[] =
	{
		XMFLOAT3(0.0f, 0.5f, 0.5f),
		XMFLOAT3(0.5f, -0.5f, 0.5f),
		XMFLOAT3(-0.5f, -0.5f, 0.5f)
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(MyVertex) * 3;
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
}

MyRectangle::~MyRectangle()
{
	device->Release();
	if (vertexShader) vertexShader->Release();
	if (vertexLayout) vertexLayout->Release();
	if (pixelShader) pixelShader->Release();
	if (vertexBuffer) vertexBuffer->Release();
}

inline HRESULT MyRectangle::CompileShaderFromFile(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** blobOut)
{
	HRESULT hr = S_OK;
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* errorBlob = nullptr;
	if (FAILED(D3DX11CompileFromFile(fileName, 0, 0, entryPoint, shaderModel, shaderFlags, 0, 0, blobOut, &errorBlob, 0)))
	{
		if (nullptr != errorBlob)
		{
			OutputDebugStringA(static_cast<char*>(errorBlob->GetBufferPointer()));
			errorBlob->Release();
		}
		return E_FAIL;
	}
	if (nullptr != errorBlob)
		errorBlob->Release();

	return S_OK;
}

ID3D11InputLayout* MyRectangle::GetVertexLayout()
{
	return vertexLayout;
}

ID3D11Buffer* const* MyRectangle::GetVertexBuffer()
{
	return &vertexBuffer;
}

ID3D11VertexShader* MyRectangle::GetVertexShader()
{
	return vertexShader;
}

ID3D11PixelShader* MyRectangle::GetPixelShader()
{
	return pixelShader;
}

const UINT* MyRectangle::GetStride()
{
	return &stride;
}

const UINT* MyRectangle::GetOffset()
{
	return &offset;
}
