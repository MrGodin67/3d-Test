#include "IncludePCH.h"
#include "Direct3DWindow.h"
#include "Meteor.h"
#include "ShaderFactory.h"
#include "StringHandler.h"
#include "Object3D.h"
#include "InstanceShader.h"
#include "FoliageShader.h"
#include "Collision.h"
#include <DirectXCollision.h>
#include "LineShader.h"
#include <DirectXMath.h>
using namespace DirectX;
Meteor::Meteor(Direct3DWindow & window)
	:Game(window)
{
	
	  
	m_test = make_unique<Model2>();
	m_test->LoadObjectFile("Obj\\room02.obj");
	ObjParser<VertexPTN>* parser = m_test->getParser();
	m_roomWorld =  std::make_unique<RoomWorld>(parser->AABB(), 4.01f);

	std::vector<Utilities::Plane> planes = parser->allTriangle();
	for (size_t i = 0; i < planes.size(); i++)
	{
		m_roomWorld->AddPlane(&planes[i]);
	}

	m_defaultFog.start = 3.0f;
	m_defaultFog.range = 15.0f;
	m_defaultLight = ShaderFactory::ConstBuffers::DefaultAmbientLight;
	m_defaultLight.fogColor = DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 0.5f);
	m_defaultLight.ambient = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	ShaderFactory::CreateConstMatrixBuffer();
	ShaderFactory::CreateFogLightBuffer(m_defaultLight,m_defaultFog);
	SetupCamera();
	LoadTerrain();
	InitShaders();
	InitD2DImages();
	SetupHeadsUpDisplay();
	SetupStaticObjects();
	RandomGenerator<std::mt19937> randG;
	
	m_player = std::make_unique<Player>(m_Camera);
	HRESULT hr;
	m_ammo = std::make_unique<WavefrontModel>();
	hr = m_ammo->LoadFromFile("Obj\\ammo01.obj");
	assert(hr == S_OK);


}

Meteor::~Meteor()
{
	for (int c = 0; c < ShaderFactory::numConstBufferMatrixType; c++)
		SafeRelease(ShaderFactory::ConstMatrixBuffers[c]);

	for (int c = 0; c < ShaderFactory::numConstBufferFogLightType; c++)
		SafeRelease(ShaderFactory::ConstFogLightBuffers[c]);
	
	
}

HRESULT Meteor::OnUpdate(UpdateEventArgs & e)
{
	Locator::SetViewMatrix(m_Camera.GetViewMatrix());
	Locator::GetContext()->UpdateSubresource(ShaderFactory::ConstMatrixBuffers[ShaderFactory::CB_frame], 0, nullptr, &XMMatrixTranspose(Locator::GetViewMatrix()), 0, 0);
	switch (m_gameState)
	{
	case running:
		HandlePlayer(e.ElapsedTime);
		HandleAmmo(e.ElapsedTime);
		m_hud->Update(e,MiniMap::PlayerData( m_Camera.GetEyePt(), m_Camera.GetWorldAhead(),40.0f));
		break;
	}
	return S_OK;
}

HRESULT Meteor::OnRender(RenderEventArgs & e)
{
	HRESULT hr = S_OK;

	if (SUCCEEDED(gfx.BeginScene(m_defaultLight.fogColor)))
	{
		switch (m_gameState)
		{
		case  menu:
		break;
		case running:
		{
			//gfx.EnableWireframe();
			for (auto& it : m_playerAmmo)
			     gfx.RenderModel(*m_ammo.get(), DirectX::XMMatrixTranslation(it.GetPosition().x, it.GetPosition().y, it.GetPosition().z), &m_shaderManager->GetShader<FogLightShader>());
			
			gfx.RenderModel(*m_test.get(), DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f), &m_shaderManager->GetShader<FogLightShader>());
			dbTri->Rasterize(&m_shaderManager->GetShader<LineShader>(),m_ammo->GetMesh(0)->GetTexture(0));
			
			m_hud->Render(gfx);
		break;
		}
		}
		gfx.EndScene();
	}
	return hr;
}

void Meteor::OnKeyPressed(KeyEventArgs & e)
{
	if (e.Key == KeyCode::Escape)
		PostQuitMessage(0);

	if (e.Key == KeyCode::L)
	{
		RoomCell* cell = m_roomWorld->getCell(m_Camera.GetEyePt2());
		if (cell)
		{
			int x = 0;
		}
	}

}

void Meteor::OnKeyReleased(KeyEventArgs & e)
{
}

void Meteor::OnMouseMoved(MouseMotionEventArgs & e)
{
	m_hud->UpdateMouse(e.X, e.Y);
	
}

void Meteor::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
	switch (m_gameState)
	{
		case  menu:
		{
			break;
		}
		case running:
		{
			if (e.State == MouseButtonEventArgs::ButtonState::Pressed && e.LeftButton && !isFiring)
			{
				m_player->IsFiring(true);
			}
			
		}
	}
}

void Meteor::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
	if (m_gameState != running) 
	{}

	if (e.State == MouseButtonEventArgs::ButtonState::Released && !e.LeftButton)
	{
		m_player->IsFiring(false);
	}
		
}

void Meteor::OnMouseWheel(MouseWheelEventArgs & e)
{
}

void Meteor::OnWndMessage(WndMsgEventArgs & e)
{
	
}

void Meteor::SetupHeadsUpDisplay()
{
	m_hud = std::make_unique<HeadsUpDisplay>(gfx.GetViewport());
	FontFactory::FontDesc camDesc(L"Ordinates",gfx.D2DFonts()->GetFormat("Tahoma18"),D2D1::RectF(10.0f,100.0f,500.0f,125.0f),D2D1::ColorF(0.0f,1.0f,0.0f,1.0f));
	m_hud->AddText("ordinates", camDesc);
	FontFactory::FontDesc Desc(L"Mouse", gfx.D2DFonts()->GetFormat("Tahoma18"), D2D1::RectF(10.0f, 125.0f, 500.0f, 150.0f), D2D1::ColorF(0.0f, 1.0f, 0.0f, 1.0f));
	m_hud->AddText("mouse", Desc);
}

void Meteor::SetupCamera()
{
	float aspect = (float)(gfx.GetViewport().Width / gfx.GetViewport().Height);
	m_Camera.SetProjParams(0.25f*DirectX::XM_PI, aspect, 0.1f, 1000.0f);
	m_Camera.SetViewParams(DirectX::XMVectorSet(3.0f, -0.75f, 3.0f, 1.0f), DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f));
	m_Camera.SetRotateButtons(false, false, true);
	m_Camera.SetEnablePositionMovement(true);
	m_Camera.SetEnableYAxisMovement(true);
	m_Camera.SetNumberOfFramesToSmoothMouseData(12);
	m_Camera.SetScalers(0.004f, 2.0f);
	m_Camera.SetViewPortCenter(DirectX::XMFLOAT2((float)(gfx.GetViewport().Width / 2), (float)(gfx.GetViewport().Height / 2)));
	m_Camera.SetDrag(true);
	m_Camera.SetResetCursorAfterMove(true);
	m_Camera.SetClipToBoundary(true, &DirectX::XMFLOAT3(2.5f, -0.75f, 2.5f), &DirectX::XMFLOAT3(57.5f, 50.0f, 57.5f));
	m_Camera.FrameMove(0.016f);
	// update const buffer for shaders
	Locator::SetProjectionMatrix(m_Camera.GetProjMatrix());
	Locator::GetContext()->UpdateSubresource(ShaderFactory::ConstMatrixBuffers[ShaderFactory::CB_projection], 0, nullptr, &XMMatrixTranspose(m_Camera.GetProjMatrix()), 0, 0);
}

HRESULT Meteor::LoadTerrain()
{
	std::vector<std::string> fn;
	fn.push_back("dirt01d.tga");
	fn.push_back("dirt01n.tga");
	//m_terrain = std::make_unique<Terrain>(Terrain::TerrainDesc(XMFLOAT3(0.0f, 0.0f, 0.0f), "output.r16",
		//"colorm02.bmp", 257, 257, 582.0f, fn));
	
	//m_grid = std::make_unique<WorldGrid>(256.0f, 256.0f, 8.0f, 8.0f);
	
	

	return S_OK;
	
}

void Meteor::InitShaders()
{
	std::vector<std::string> data;
	if (DXStringHandler::DXLoadTextFile("shaderdata.txt", data))
	{
		std::string str;
		for (size_t i = 0; i < data.size(); i++)
		{
			std::vector<std::string>tokens;
			str = data[i];
			DXStringHandler::Tokenize(&tokens, str, " ");
			std::wstring wStrV;
			std::wstring wStrP;
			DXStringHandler::DXConvertFromStrToWStr(tokens[1], wStrV);
			DXStringHandler::DXConvertFromStrToWStr(tokens[4], wStrP);
			ShaderFactory::InitShaderDesc desc({ wStrV,tokens[2],tokens[3] },
			{ wStrP,tokens[5],tokens[6] });
			m_shaderInits[tokens[0]] = desc;
			
		}
		m_shaderManager = std::make_unique<ShaderFactory::ShaderManager>();
		m_shaderManager->AddShader<FogLightShader>(m_shaderInits["FOG_LIGHT"]);
		m_shaderManager->AddShader<LineShader>(m_shaderInits["LINE_COLOR"]);
	}
	else
	{
		throw std::exception("failed to load and create shaders");
	}
	
	
}

void Meteor::InitD2DImages()
{
	m_D2DImageManager = std::make_unique<TextureFactory::D2D1ImageManager>();
	m_D2DImageManager->AddImage(L"menu.png","menu", { 128,128 });
	
}

void Meteor::SetupStaticObjects()
{
	//std::vector<XMFLOAT3> foliagePositions;
	//
	//RandomGenerator<std::mt19937> randG;
	//for (int c = 0; c < 10; c++)
	//{
	//	float height = 0.0f;
	//	XMFLOAT3 p(randG.Get<float>(60.0f, 82.0f), 8.75f, randG.Get<float>(80.0f, 102.0f));
	//	//m_terrain->GetHeight(XMLoadFloat3(&p), height);
	//	p.y = height + 0.5f;
	//	foliagePositions.push_back(p);
	//}
	//m_foliageA = std::make_unique<FoliageClass>("grass.dds", foliagePositions);
	
	Utilities::BoundingBox box(DirectX::XMFLOAT3(2.0f, 1.0f, 2.0f),
		DirectX::XMFLOAT3(7.5f, 0.5f, 7.5f), DirectX::XMFLOAT3(8.5f, 1.5f, 8.5f));
	Utilities::Plane plane(DirectX::XMFLOAT3(8.0f, 1.5f, 8.5f),
		DirectX::XMFLOAT3(8.25f, 1.0f, 7.0f), DirectX::XMFLOAT3(7.70f, 0.4f, 7.85f));

	dbTri = std::make_unique<Utilities::DebugTri>( plane , box);


}

bool Meteor::HandlePlayer(float dt)
{
	m_player->Update(dt, nullptr);// m_terrain.get());
	DirectX::XMVECTOR targetP;
	targetP = m_Camera.GetEyePt() + m_Camera.GetWorldAhead() * 20.75f;
	Utilities::BoundingBox box = Utilities::BoundingBox::Make(m_Camera.GetEyePt2(),
		DirectX::XMFLOAT3(-0.15f, -0.15f, -0.15f), DirectX::XMFLOAT3(0.15f, 0.15f, 0.15f));
	RoomCell* cell = m_roomWorld->getCell(box.center);
	DirectX::XMFLOAT3 intersectPt;
	float intersectDistance;
	static DirectX::XMFLOAT3 oldPos = m_Camera.GetEyePt2();
	bool foundCollision = false;
	if (cell)
	{
		std::vector<DirectX::XMVECTOR> vecs;
		std::vector<float> dist;
		float p;
		DirectX::XMVECTOR n;
			for (size_t i = 0; i < cell->m_planes.size(); i++)
			{
				if (Collision::AABBtoAABB(box, cell->m_planes[i].box))
				{
					float nx = cell->m_planes[i].normal.x;
					float nz = cell->m_planes[i].normal.z;
					float ny = cell->m_planes[i].normal.y;
					float vx = m_Camera.GetVelocity().x;
					float vz = m_Camera.GetVelocity().z;
					ny = 0.0f;
					if (nx != 0.0f)
					{
						if (nx > 0.0f)
						{
							p = abs(cell->m_planes[i].box.maxPt.x - box.minPt.x);
						}
						else
						{
							p = abs(cell->m_planes[i].box.minPt.x - box.maxPt.x);
						}
					}
					if (nz != 0.0f)
					{
						if (nz > 0.0f)
						{
							p = abs(cell->m_planes[i].box.maxPt.z - box.minPt.z);
						}
						else
						{
							p = abs(cell->m_planes[i].box.minPt.z - box.maxPt.z);
						}
					}
					foundCollision = true;
					n = DirectX::XMVectorSet(nx, ny, nz,1.0f);// DirectX::XMLoadFloat3(&cell->m_planes[i].normal);
						auto& it = std::find_if(vecs.begin(), vecs.end(),[&n](DirectX::XMVECTOR& a)
					{
						return DirectX::XMVector3Equal(n , a);
					});
					if (it == vecs.end())
					{
						vecs.push_back(n);
						dist.push_back(p);
						foundCollision = true;
					}
										
				}
		
		}
		if (foundCollision)
		{
			float d = 0.0f;
			XMVECTOR resultN = { 0 };
			if (vecs.size() > 1)
			{
				
				for (size_t i=0;i < vecs.size();i++)
				{
					resultN += vecs[i];
					d += dist[i];
					
				}
				resultN /= vecs.size();
				resultN = XMVector3Normalize(resultN);
			}
			DirectX::XMVECTOR pt = m_Camera.GetEyePt();
		
			if (vecs.size() > 1)
				pt += resultN * d;
			else
			   pt += n * p;
			DirectX::XMStoreFloat3(&oldPos, pt);
			m_Camera.SetEye(oldPos.x, oldPos.y, oldPos.z);
			foundCollision = false;
		}
	}

	if (m_player->IsFiring())
	{
		Ammo a;
		if (m_player->FireGun(targetP, a))
		{
			m_playerAmmo.push_back(a);
		}
	}
	
	return true;
}

void Meteor::HandleAmmo(float& dt)
{
	for (size_t i = 0; i < m_playerAmmo.size(); i++)
	{
		RoomCell* cell = m_roomWorld->getCell(m_playerAmmo[i].GetPosition());
		bool intersect_result = false;
		if (cell)
		{
			for (int c = 0; c < cell->m_planes.size(); c++)
			{
				DirectX::XMFLOAT3 vel = m_playerAmmo[i].GetVeclocity();
				DirectX::XMVECTOR pi = DirectX::XMLoadFloat3(&m_playerAmmo[i].GetPosition()) + DirectX::XMLoadFloat3(&m_playerAmmo[i].GetVeclocity()) *m_playerAmmo[i].speed*dt;
				DirectX::XMStoreFloat3(&vel, pi);
				DirectX::XMFLOAT3 intersect_pt;
				float resultDot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&m_playerAmmo[i].GetPosition()),
					DirectX::XMLoadFloat3(&cell->m_planes[c].normal)));

				float distance_from_origin;
				Utilities::LineSegment line(m_playerAmmo[i].GetPosition(), vel);
				intersect_result = Collision::LineIntersectPlane(line, cell->m_planes[c],
					intersect_pt, distance_from_origin);


				if (intersect_result)
				{
					m_playerAmmo[i].position = intersect_pt;
					m_playerAmmo[i].speed = 0.0f;
					m_playerAmmo[i].timeToDie = 10.0f;
					break;
				}
			}
		}
		if (!m_playerAmmo[i].Update(dt))
		{
			m_playerAmmo.erase(m_playerAmmo.begin() + i);
			i--;
		};
	}
}

