#include "IncludePCH.h"
#include "FoliageClass.h"
#include "Locator.h"
#include "Geometry.h"
#include "TextureFactory.h"
#include "Camera3D_2.h"
using namespace DirectX;
FoliageClass::FoliageClass(std::string texFilename, std::vector<DirectX::XMFLOAT3>& positions)
	:m_numInstances((UINT)positions.size())
{
	std::vector<VertexType> verts;

	verts.emplace_back(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));
	verts.emplace_back(XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f));
	verts.emplace_back(XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f));

	verts.emplace_back(XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f));
	verts.emplace_back(XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f));
	verts.emplace_back(XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f));
	
	m_aabb = Utilities::BoundingBox(XMFLOAT3(0.5f, 0.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 0.0f));
	HRESULT hr = Geometry::CreateVertexBufferData(verts.data(), sizeof(VertexType)*(UINT)verts.size(), &m_vertexBuffer);
	
	assert(hr == S_OK);

	m_data.resize(m_numInstances);
	m_Instances.resize(m_numInstances);

	for (int i = 0; i < m_numInstances; i++)
	{
		m_data[i].position = positions[i];
		m_data[i].color = XMFLOAT3(0.5f, 1.0f, 0.0f);
		m_Instances[i].matrix = XMMatrixIdentity();
		m_Instances[i].color = m_data[i].color;
	}

	
	D3D11_BUFFER_DESC constantBufferDesc = { 0 };
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.ByteWidth = sizeof(InstanceType)*(UINT)m_Instances.size();
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	
	D3D11_SUBRESOURCE_DATA subData = { 0 };
	subData.pSysMem = m_Instances.data();
	hr = Locator::GetDevice()->CreateBuffer(&constantBufferDesc, &subData, &m_InstanceBuffer);
	assert(hr == S_OK);
	hr = TextureFactory::CreateSRV(&m_texture, texFilename);
	assert(hr == S_OK);
}

FoliageClass::~FoliageClass()
{
	SafeRelease(m_vertexBuffer);
	SafeRelease(m_texture);
	SafeRelease(m_InstanceBuffer);
}

ID3D11Buffer * FoliageClass::GetVertexBuffer()
{
	return m_vertexBuffer;
}

ID3D11Buffer * FoliageClass::GetInstanceBuffer()
{
	return m_InstanceBuffer;
}

ID3D11ShaderResourceView * FoliageClass::GetTexture()
{
	return m_texture;
}

UINT FoliageClass::GetVisibleCount()
{
	return m_VisibleCount;
}

void FoliageClass::CheckInView( CFirstPersonCamera & cam)
{
	m_VisibleCount = 0;
	std::vector<InstanceType> inst;
	inst.resize(m_numInstances);
	XMFLOAT3 camPos = cam.GetEyePt2();
	for (size_t i = 0; i < m_numInstances; i++)
	{
		XMVECTOR result = XMLoadFloat3(&m_data[i].position) - cam.GetEyePt();
		// get square length
		float lenSq = XMVectorGetX(XMVector3LengthSq(result));
		float minDistance = 50.0f;
		// cheezy Level of detail
		if (lenSq > sqr(minDistance))
			continue;//too far away
		
		float scale = 0.15f;// __min(1.0f - sqrt(lenSq), 0.15f);
		//scale = __max(0.09f, scale);
		XMMATRIX m = XMMatrixTranslation(m_data[i].position.x, m_data[i].position.y*scale, m_data[i].position.z);
	
		float angle = atan2(m_data[i].position.x - camPos.x, m_data[i].position.z - camPos.z) * (180.0 / XM_PI);

		//float angle = Utilities::RadToDeg(-XMVectorGetX(result));
		float rotation = (float)angle * 0.0174532925f;
		m = XMMatrixScaling(scale, scale, scale) * XMMatrixRotationY(Utilities::DegToRad(angle)) * m;
		
		Utilities::BoundingBox box = m_aabb.TransformCoords(m);
	
		if (cam.InFrustumView(box.minPt, box.maxPt))
		{
			inst[m_VisibleCount].matrix = m; 
			inst[m_VisibleCount].color = m_Instances[i].color;
			m_VisibleCount++;
		}
	}
	Locator::GetContext()->UpdateSubresource(m_InstanceBuffer, 0, nullptr, inst.data(), 0, 0);
}
