#pragma once

#include "Game.h"
#include "FogLightShader.h"
#include "MeshFactory.h"
#include "TextureFactory.h"
#include "Camera3D_2.h"
#include "Model.h"
#include "Utilities.h"
#include "MiniMap.h"
#include "randomGenerator.h"
#include "HeadsUpDisplay.h"
#include "DualGun.h"
#include "Ammo.h"
#include "Player.h"
#include "ObjParser.h"
#include "Model2.h"
#include "RoomWorld.h"
class Direct3DWindow;
class Meteor : public Game
{

private:
	GAME_STATE m_gameState = running;
	std::unordered_map<std::string,ShaderFactory::InitShaderDesc> m_shaderInits;
	std::unique_ptr<WavefrontModel> m_ammo;
	std::unique_ptr<ShaderFactory::ShaderManager> m_shaderManager;
	std::unique_ptr<HeadsUpDisplay> m_hud;
	std::unique_ptr<TextureFactory::D2D1ImageManager> m_D2DImageManager;
	std::unique_ptr<RoomWorld> m_roomWorld;
	std::unique_ptr<Model2> m_test;
	// player
	std::unique_ptr<Player> m_player;
	std::vector<Ammo> m_playerAmmo;
	//~
	ShaderFactory::ConstBuffers::DirectionalLight m_defaultLight;
	ShaderFactory::ConstBuffers::Fog m_defaultFog;
	std::unique_ptr<Utilities::DebugTri> dbTri;
	ObjParser<VertexPTN> m_parser;
	bool isFiring = false;
public:
	Meteor(Direct3DWindow& window);
	~Meteor();
	virtual HRESULT OnUpdate(UpdateEventArgs& e);
	virtual HRESULT OnRender(RenderEventArgs& e);
	virtual void OnKeyPressed(KeyEventArgs& e);
	virtual void OnKeyReleased(KeyEventArgs& e);
	virtual void OnMouseMoved(MouseMotionEventArgs& e);
	virtual void OnMouseButtonPressed(MouseButtonEventArgs& e);
	virtual void OnMouseButtonReleased(MouseButtonEventArgs& e);
	virtual void OnMouseWheel(MouseWheelEventArgs& e);
	virtual void OnWndMessage(WndMsgEventArgs& e);
private:
	// Initialization
	void SetupHeadsUpDisplay();
	void SetupCamera();
	HRESULT LoadTerrain();
	void InitShaders();
	void InitD2DImages();
	void SetupStaticObjects();
	// Logic
	bool HandlePlayer(float dt);
	void HandleAmmo(float& dt);
	

	
};


