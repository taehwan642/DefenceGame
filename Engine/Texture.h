#pragma once
#include "framework.h"

class Texture
{
	ID3D11ShaderResourceView* textureRV;
	ID3D11SamplerState* samplerState;
};

