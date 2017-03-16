#pragma once

#include "ShaderFactory.h"

class LineShader : public ShaderFactory::Shader
{
private:
	const UINT numElements = sizeof(ShaderFactory::InputLayoutDesc::PositionColor32) /
		sizeof(ShaderFactory::InputLayoutDesc::PositionColor32[0]);
public:
	LineShader(ShaderFactory::InitShaderDesc desc);
	~LineShader();
	HRESULT Render(ID3D11DeviceContext * context, ID3D11Buffer* VBuffer, ID3D11Buffer* IBuffer, const UINT vertStructInBytes,
		const int indexCount, const std::vector<ID3D11ShaderResourceView*> textures)override;

};