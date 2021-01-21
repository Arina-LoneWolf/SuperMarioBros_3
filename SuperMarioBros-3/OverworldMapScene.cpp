#include "OverworldMapScene.h"

COverworldMapScene::COverworldMapScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new COverworldMapSceneKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/


void COverworldMapScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void COverworldMapScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void COverworldMapScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (size_t i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void COverworldMapScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (size_t i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS]
*/
void COverworldMapScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	Type object_type = static_cast<Type>(atoi(tokens[0].c_str()));
	float x = strtof(tokens[1].c_str(), NULL);
	float y = strtof(tokens[2].c_str(), NULL);

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;
	CMapPoint* point = NULL;

	switch (object_type)
	{
	case Type::MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = CMario::GetInstance();
		player = (CMario*)obj;
		//player->RefreshAtOverworldMap();
		//player->SetPosition(x, y);
		player->SetPositionAtCurrentPoint(player->currentPoint->x, player->currentPoint->y + MAP_POINT_Y_OFFSET);
		HUD = new CStatusBar(player);

		DebugOut(L"[INFO] Player object created!\n");
		break;

	case Type::MAP_POINT:
	{
		int sceneID = atoi(tokens[4].c_str());
		int l = atoi(tokens[5].c_str());
		int r = atoi(tokens[6].c_str());
		int a = atoi(tokens[7].c_str());
		int u = atoi(tokens[8].c_str());
		int id = atoi(tokens[9].c_str());
		point = new CMapPoint(x, y, sceneID, l, r, a, u, id);
		break;
	}
	
	case Type::TUSSOCK:
	case Type::HELP_BUBBLE:
	case Type::GATEKEEPER:
	{
		obj = new CMapObject();
		break;
	}

	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	if (point)
	{
		point->SetType(object_type);
		point->SetPosition(x, y);
		point->SetAnimationSet(ani_set);
		listPoints.push_back(point);
	}
	else
	{
		obj->SetType(object_type);
		obj->SetAnimationSet(ani_set);
		if (obj->type != Type::MARIO)
		{
			obj->SetPosition(x, y);
			listMapObj.push_back(obj);
		}
	}

	

	if (CGame::GetInstance()->GetCurrentSceneID() == INTRO_SCENE_ID)
		intro = new CIntroDisplay();
}

/*
	Parse a line in section [TILE_MAP]
*/
void COverworldMapScene::_ParseSection_TILE_MAP(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 9) return; // skip invalid lines - an object set must have at least id, x, y

	int ID = atoi(tokens[0].c_str());

	wstring FilePath_tex = ToWSTR(tokens[1]);
	wstring FilePath_data = ToWSTR(tokens[2]);

	int Map_width = atoi(tokens[3].c_str());
	int Map_height = atoi(tokens[4].c_str());
	int Num_row_read = atoi(tokens[5].c_str());
	int Num_col_read = atoi(tokens[6].c_str());
	int Tile_width = atoi(tokens[7].c_str());
	int Tile_height = atoi(tokens[8].c_str());

	map = new TileMap(ID, FilePath_tex.c_str(), FilePath_data.c_str(), Map_width, Map_height, Num_row_read, Num_col_read, Tile_width, Tile_height);
}

//load object/ textures/ sprites -> animations
void COverworldMapScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[TILEMAP]") {
			section = SCENE_SECTION_TILEMAP; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_TILEMAP: _ParseSection_TILE_MAP(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	CTextures::GetInstance()->Add(ID_TEX_DARKEN, L"textures\\darken-the-screen.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void COverworldMapScene::Update(ULONGLONG dt)
{
	for (size_t i = 0; i < listPoints.size(); i++)
	{
		listMapObj[i]->Update(dt, &listPoints);
	}

	for (size_t i = 0; i < listMapObj.size(); i++)
	{
		listMapObj[i]->Update(dt, &listMapObj);
	}

	if (CGame::GetInstance()->GetCurrentSceneID() != INTRO_SCENE_ID)
		player->UpdateAtOverworldMap(dt, &listPoints);

	if (intro)
		intro->Update(dt);
	
	//skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

}

void COverworldMapScene::Render()
{
	map->Draw();

	for (size_t i = 0; i < listPoints.size(); i++)
	{
		listMapObj[i]->Render();
	}

	for (size_t i = 0; i < listMapObj.size(); i++)
	{
		listMapObj[i]->Render();
	}

	if (CGame::GetInstance()->GetCurrentSceneID() != INTRO_SCENE_ID)
	{
		player->RenderAtOverworldMap();
		HUD->Render(CGame::GetInstance()->GetCamPosX(), CGame::GetInstance()->GetCamPosY());
	}

	if (intro)
		intro->Render();
}

/*
	Unload current scene
*/
void COverworldMapScene::Unload()
{
	for (int i = 0; i < listMapObj.size(); i++)
		delete listMapObj[i];

	for (int i = 0; i < listPoints.size(); i++)
		delete listPoints[i];

	listMapObj.clear();
	listPoints.clear();
	player = NULL;
	delete HUD;
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void COverworldMapSceneKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario* mario = ((COverworldMapScene*)scene)->GetPlayer();

	CIntroDisplay* intro = ((COverworldMapScene*)scene)->GetIntro();
	
	if (mario->vx != 0 || mario->vy != 0)
		return;
	
	switch (KeyCode)
	{
	case DIK_LEFT:
		if (mario->movementPermission.at(ALLOWED_TO_GO_LEFT))
			mario->vx = -MARIO_SPEED_ON_MAP;
		break;
	case DIK_RIGHT:
		if (mario->movementPermission.at(ALLOWED_TO_GO_RIGHT))
			mario->vx = MARIO_SPEED_ON_MAP;
		break;
	case DIK_UP:
		if (CGame::GetInstance()->GetCurrentSceneID() == INTRO_SCENE_ID)
		{
			if (intro->GetCursorPosition() == CURSOR_AT_2_PLAYER)
				intro->SetCursorPosition(CURSOR_AT_1_PLAYER);
		}
		else if (mario->movementPermission.at(ALLOWED_TO_GO_UP))
			mario->vy = -MARIO_SPEED_ON_MAP;
		break;
	case DIK_DOWN:
		if (CGame::GetInstance()->GetCurrentSceneID() == INTRO_SCENE_ID)
		{
			if (intro->GetCursorPosition() == CURSOR_AT_1_PLAYER)
				intro->SetCursorPosition(CURSOR_AT_2_PLAYER);
		}
		else if (mario->movementPermission.at(ALLOWED_TO_GO_DOWN))
			mario->vy = MARIO_SPEED_ON_MAP;
		break;
	case DIK_S:
		if (CGame::GetInstance()->GetCurrentSceneID() == OVERWORLD_MAP_SCENE_ID)
		{
			mario->onOverworldMap = false;
			CGame::GetInstance()->SwitchScene(mario->currentPoint->sceneID);
		}
		break;
	case DIK_W:
		if (CGame::GetInstance()->GetCurrentSceneID() == INTRO_SCENE_ID)
		{
			CGame::GetInstance()->SwitchScene(OVERWORLD_MAP_SCENE_ID);
		}
		break;
	case DIK_Q:
		if (CGame::GetInstance()->GetCurrentSceneID() == OVERWORLD_MAP_SCENE_ID)
			CGame::GetInstance()->SwitchScene(INTRO_SCENE_ID);
	}
}

void COverworldMapSceneKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = ((COverworldMapScene*)scene)->GetPlayer();
}