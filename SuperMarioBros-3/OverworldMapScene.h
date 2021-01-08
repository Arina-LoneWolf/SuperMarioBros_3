#pragma once
#include "Scence.h"
#include "Game.h"
#include "Textures.h"
#include "Mario.h"
#include "TileMap.h"
#include "StatusBar.h"
#include "MapObject.h"

class COverworldMapScene : public CScene
{
protected:
	CMario* player = nullptr;					// A play scene has to have player, right? 
	TileMap* map = nullptr;
	CStatusBar* CRUD;
	vector<LPGAMEOBJECT> listMapObj;
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TILE_MAP(string line);

public:
	COverworldMapScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(ULONGLONG dt);
	virtual void Render();
	virtual void Unload();

	CMario* GetPlayer() { return player; }

	//friend class CPlayScenceKeyHandler;
};

class COverworldMapSceneKeyHandler : public CSceneKeyHandler
{
public:

	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	COverworldMapSceneKeyHandler(CScene* s) : CSceneKeyHandler(s) {};
};

