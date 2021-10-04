#include "Texture.h"

void Texture::Render(ID3D11DeviceContext* context)
{
	context->PSSetShaderResources(0, 1, &textureRV);
	context->PSSetSamplers(0, 1, &samplerState);
}
