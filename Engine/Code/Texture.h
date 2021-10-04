#pragma once
#include "framework.h"

class Texture
{
	ID3D11ShaderResourceView* textureRV;
	ID3D11SamplerState* samplerState;
public:
	__forceinline HRESULT Initialize(ID3D11Device* device, const wchar_t* filename);
	void Render();
};

HRESULT Texture::Initialize(ID3D11Device* device, const wchar_t* filename)
{
	if (FAILED(DirectX::CreateDDSTextureFromFile(device, filename, nullptr, &textureRV)))
		return E_FAIL;

    // Create the sample state
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
   
    if (FAILED(device->CreateSamplerState(&sampDesc, &samplerState)))
        return E_FAIL;

	return S_OK;
}

