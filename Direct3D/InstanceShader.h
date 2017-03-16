#pragma once
#include "ShaderFactory.h"

#include "Model.h"
#include "Camera3D_2.h"
class InstanceShader :
	public ShaderFactory::Shader
{
	std::vector<DirectX::XMFLOAT3> m_instancePositions;
	ID3D11Buffer* m_instanceBuffer;
	WavefrontModel* m_model;
	int m_VisibleCount = 0;
	const UINT numElements = sizeof(ShaderFactory::InputLayoutDesc::InstancedPosition32) /
		sizeof(ShaderFactory::InputLayoutDesc::InstancedPosition32[0]);
public:
	InstanceShader(ShaderFactory::InitShaderDesc desc,WavefrontModel* model, std::vector<DirectX::XMFLOAT3> positions);
	~InstanceShader();
	void CheckInViewFrustum(CFirstPersonCamera& cam);
	void SetModel(WavefrontModel* model) { m_model = model; }
	// Inherited via Shader
	virtual HRESULT Render(ID3D11DeviceContext * context, ID3D11Buffer * VBuffer, ID3D11Buffer * IBuffer, const UINT vertStructInBytes, const int indexCount, const std::vector<ID3D11ShaderResourceView*> textures) override;
};

