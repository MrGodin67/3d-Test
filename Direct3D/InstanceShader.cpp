#include "IncludePCH.h"
#include "InstanceShader.h"
using namespace DirectX;

InstanceShader::InstanceShader(ShaderFactory::InitShaderDesc desc,WavefrontModel* model, std::vector<DirectX::XMFLOAT3> positions)
	:m_model(model), m_instancePositions(positions)
{
	ID3DBlob* vblob;
	ID3DBlob* pblob;
	m_vertexShader = ShaderFactory::LoadShader<ID3D11VertexShader>(&vblob, desc.first);
	assert(m_vertexShader);// check pointer
	m_pixelShader = ShaderFactory::LoadShader<ID3D11PixelShader>(&pblob, desc.second);
	assert(m_pixelShader);
	ShaderFactory::SamplerStates::CreateSamplerState(ShaderFactory::SamplerStates::BasicTexture, &m_sampler);
	assert(m_sampler);
	ShaderFactory::InputLayoutDesc::CreateLayout(ShaderFactory::InputLayoutDesc::InstancedPosition32, numElements, vblob, &m_inputLayout);
	assert(m_inputLayout);
	
	// clean up
	SafeRelease(vblob);
	SafeRelease(pblob);

	HRESULT hr;
	D3D11_BUFFER_DESC constantBufferDesc = { 0 };
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.ByteWidth = sizeof(XMFLOAT3)*m_instancePositions.size();
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	D3D11_SUBRESOURCE_DATA subData = { 0 };
	subData.pSysMem = m_instancePositions.data();
	hr = Locator::GetDevice()->CreateBuffer(&constantBufferDesc, &subData, &m_instanceBuffer);
	assert(hr == S_OK);
	
}


InstanceShader::~InstanceShader()
{
	SafeRelease(m_instanceBuffer);
}

void InstanceShader::CheckInViewFrustum(CFirstPersonCamera & cam)
{
	m_VisibleCount = 0;
	Utilities::BoundingBox model_box = m_model->BoundingBox();
	std::vector<XMFLOAT3> inst(m_instancePositions.size());
	for (size_t i = 0; i < m_instancePositions.size(); i++)
	{
		Utilities::BoundingBox box;
		// transform from model to world space
		box = model_box.TransformCoords(DirectX::XMMatrixTranslation(m_instancePositions[i].x, m_instancePositions[i].y, m_instancePositions[i].z));

		// Check if in view frustun
		if (cam.InFrustumView(box.minPt, box.maxPt))
		{
			inst[m_VisibleCount] = m_instancePositions[i];
			m_VisibleCount++;
		}


	}
	Locator::GetContext()->UpdateSubresource(m_instanceBuffer, 0, nullptr, inst.data(), 0, 0);
}

HRESULT InstanceShader::Render(ID3D11DeviceContext * context, ID3D11Buffer * VBuffer, ID3D11Buffer * IBuffer, const UINT vertStructInBytes, const int indexCount, const std::vector<ID3D11ShaderResourceView*> textures)
{
	for (int c = 0; c < m_model->MeshCount(); c++)
	{
		context->PSSetConstantBuffers(0, 1, &ShaderFactory::ConstFogLightBuffers[ShaderFactory::CB_light]);
		UINT stride[2] = { sizeof(m_model->GetMesh(c)->VertType), sizeof(DirectX::XMFLOAT3) };
		UINT offset[2] = { 0,0 };
		ID3D11Buffer* vbs[2] = { m_model->GetMesh(c)->GetVertexBuffer(), m_instanceBuffer };
		//// set buffers into pipeline
		context->IASetVertexBuffers(0, 2, vbs, stride, offset);
		context->IASetIndexBuffer(m_model->GetMesh(c)->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
		//UINT offset = 0;
		//context->IASetVertexBuffers(0, 1, &VBuffer, &vertStructInBytes, &offset);
		//context->IASetIndexBuffer(IBuffer, DXGI_FORMAT_R32_UINT, 0);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		int bufferNumber = 0;

		context->VSSetConstantBuffers(bufferNumber, 3, ShaderFactory::ConstMatrixBuffers);
		size_t index = 0;
		bufferNumber = 3;
		//for (UINT i = 0; i < (UINT)textures.size(); i++)
		ID3D11ShaderResourceView* tex = m_model->GetMesh(c)->GetTexture(index);
		context->PSSetShaderResources(0, 1, &tex);// &textures[i]);
		context->VSSetConstantBuffers(bufferNumber, 1, &ShaderFactory::ConstFogLightBuffers[ShaderFactory::CB_fog]);
		context->IASetInputLayout(m_inputLayout);
		context->VSSetShader(m_vertexShader, NULL, 0);
		context->PSSetShader(m_pixelShader, NULL, 0);
		context->PSSetSamplers(0, 1, &m_sampler);
		context->DrawIndexedInstanced(m_model->GetMesh(c)->IndicesCount(), m_VisibleCount, 0, 0, 0);
	}
	//context->DrawIndexed(indexCount, 0, 0);

	return S_OK;
	
}
