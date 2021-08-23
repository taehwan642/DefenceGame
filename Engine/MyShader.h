#pragma once

class MyShader
{
private:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	ID3DBlob* vsBlob;

public:
	ID3D11Device* device;

	__forceinline MyShader(ID3D11Device* dev);
	__forceinline ~MyShader();

	__forceinline HRESULT CompileShaderFromFile(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** blobOut);

	__forceinline ID3D11VertexShader* GetVertexShader();
	__forceinline ID3D11PixelShader* GetPixelShader();
	__forceinline ID3DBlob* GetVertexShaderBlob();

};

MyShader::MyShader(ID3D11Device* dev) : device(dev)
{
	device->AddRef();
	vsBlob = nullptr;
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
}

MyShader::~MyShader()
{
	if (vertexShader) vertexShader->Release();
	if (vsBlob) vsBlob->Release();
	if (pixelShader) pixelShader->Release();

}

HRESULT MyShader::CompileShaderFromFile(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** blobOut)
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

ID3D11VertexShader* MyShader::GetVertexShader()
{
	return vertexShader;
}

ID3D11PixelShader* MyShader::GetPixelShader()
{
	return pixelShader;
}

ID3DBlob* MyShader::GetVertexShaderBlob()
{
	return vsBlob;
}