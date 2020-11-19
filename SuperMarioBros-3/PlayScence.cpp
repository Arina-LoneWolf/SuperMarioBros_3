#include <iostream>
#include <fstream>

#include "PlayScence.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/


void CPlayScene::_ParseSection_TEXTURES(string line)
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

void CPlayScene::_ParseSection_SPRITES(string line)
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

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
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
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case Type::MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		player = (CMario*)obj;

		DebugOut(L"[INFO] Player object created!\n");
		break;

	case Type::PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
		break;
	}

	case Type::FLOOR:
	{
		float width = atof(tokens[4].c_str());
		float height = atof(tokens[5].c_str());
		obj = new CFloor(width, height);
		break;
	}

	case Type::COLOR_BOX:
	{
		float width = atof(tokens[4].c_str());
		float height = atof(tokens[5].c_str());
		obj = new CColorBox(width, height);
		break;
	}

	case Type::BRICK_CONTAINS_ITEM:
	{
		int brickType = atof(tokens[4].c_str());
		int itemType = atof(tokens[5].c_str());
		obj = new CBrickContainsItem(brickType, itemType, y);
		break;
	}

	case Type::FIRE_PIRANHA:
	{
		int piranhaType = atof(tokens[4].c_str());
		obj = new CFirePiranha(player, piranhaType);
		break;
	}

	case Type::PIPE:
	{
		int pipeType = atof(tokens[4].c_str());
		obj = new CPipe(pipeType);
		break;
	}

	case Type::KOOPA:
	{
		int startingPos = atof(tokens[4].c_str());
		obj = new CRedKoopa(player, startingPos);
		break;
	}

	case Type::GREEN_PIRANHA: obj = new CGreenPiranha(player); break;
	case Type::BRONZE_BRICK: obj = new CBronzeBrick(); break;
	case Type::GOOMBA: obj = new CGoomba(); break;

	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}

void CPlayScene::_ParseSection_TileMap(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 9) return;
	int ID = atoi(tokens[0].c_str());
	wstring file_texture = ToWSTR(tokens[1]);
	wstring file_path = ToWSTR(tokens[2]);
	int row_on_textures = atoi(tokens[3].c_str());
	int col_on_textures = atoi(tokens[4].c_str());
	int row_on_tile_map = atoi(tokens[5].c_str());
	int col_on_tile_map = atoi(tokens[6].c_str());
	int tile_width = atoi(tokens[7].c_str());
	int tile_height = atoi(tokens[8].c_str());
	//int texID = atoi(tokens[0].c_str());
	map = new TileMap(ID, file_texture.c_str(), file_path.c_str(), row_on_textures, col_on_textures, row_on_tile_map, col_on_tile_map, tile_width, tile_height);
}

void CPlayScene::Load()
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
		if (line == "[TILEMAP]")
		{
			section = SCENE_SECTION_TILEMAP;
			continue;
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
		case SCENE_SECTION_TILEMAP: _ParseSection_TileMap(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);

		if (objects[i]->type == Type::BRICK_CONTAINS_ITEM)
		{
			CBrickContainsItem* brick = dynamic_cast<CBrickContainsItem*>(objects[i]);
			if (brick->isAboutToDropItem && !brick->dropped)
			{
				DropItem(brick->itemType, brick->x, brick->y);
				brick->dropped = true;
			}
		}
	}

	player->CheckCollisionWithItems(&listItem);

	for (LPGAMEOBJECT item : listItem)
		item->Update(dt, &coObjects);

	// remove
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->isFinishedUsing)
			objects.erase(objects.begin() + i);
	}

	for (size_t i = 0; i < listItem.size(); i++)
	{
		if (listItem[i]->isFinishedUsing)
			listItem.erase(listItem.begin() + i);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;

	CGame::GetInstance()->cam_y = 200;
	if (player->x > (SCREEN_WIDTH / 4) && player->x + (SCREEN_WIDTH / 4) < map->GetWidthTileMap())
	{
		cx = player->x - (SCREEN_WIDTH / 4);
		CGame::GetInstance()->cam_x = cx;
	}
}

void CPlayScene::Render()
{
	map->Draw();
	for (int i = objects.size() - 1; i >= 0; i--)
		objects[i]->Render();

	for (LPGAMEOBJECT item : listItem)
		item->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScene::DropItem(int itemType, float x, float y)
{
	switch (itemType)
	{
	case ITEM_RANDOM:
	{
		//if (player->GetLevel() == MARIO_LEVEL_SMALL)
		//{
		//	// mushroom appears
		//}
		//else if ()
		CSuperLeaf* leaf = new CSuperLeaf(x, y);
		listItem.push_back(leaf);
		break;
	}
	case ITEM_MONEY:
		break;
	case ITEM_UP_MUSHROOM:
		break;
	case ITEM_P_SWITCH:
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_S: // high jump
		if (mario->canFly)
		{
			//DebugOut(L"can fly\n");
			if (mario->GetLevel() == MARIO_RACCOON)
			{
				mario->Fly();
			}
		}
		else
		{
			if (!mario->isOnGround && mario->GetLevel() == MARIO_RACCOON)
				mario->isWaggingTail = true;
			mario->SetState(MARIO_STATE_JUMP_HIGH);
			mario->waggingTailStartTime = GetTickCount();
		}
		break;

	case DIK_A: // attack enemies while in Fire or Raccoon transformation
		if (mario->GetLevel() == MARIO_FIRE || mario->GetLevel() == MARIO_RACCOON)
			mario->Attack();
		break;

		/*case DIK_X:
			if (mario->canFly)
			{
				if (mario->GetLevel() == MARIO_RACCOON)
					mario->Fly();
			}*/

	case DIK_R: // turn into raccoon mario
		mario->TurnIntoRaccoon();
		break;

	case DIK_F: // turn into fire mario
		mario->TurnIntoFire();
		break;

	case DIK_B: // turn into big mario
		mario->TurnIntoSuper();
		break;

	case DIK_SPACE: // reset the game starting as small mario
		mario->Reset();
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_S:
		if (mario->canFly)
			return;
		if (!(mario->isOnGround || mario->isFalling))
		{
			if (mario->y > MARIO_TOO_HIGH_ABOVE)
				mario->vy += MARIO_GRAVITY * 15 * mario->dt;
			else
				mario->vy += MARIO_GRAVITY * 11 * mario->dt;
		}
		break;

	case DIK_X:
		mario->vy += MARIO_GRAVITY * 7 * mario->dt;
		break;

	case DIK_A:
		mario->isRunning = false;
		mario->isHoldingShell = false;
		break;

	case DIK_DOWN:
		if (mario->isOnGround)
			mario->isSitting = false;
		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();

	if (mario->GetState() == MARIO_STATE_DIE)
		return;
	if (mario->isWaitingForAni)
		return;

	else if ((game->IsKeyDown(DIK_LEFT) && game->IsKeyDown(DIK_RIGHT))
		|| (game->IsKeyDown(DIK_DOWN) && game->IsKeyDown(DIK_UP)))
	{
		mario->Idle();
	}
	else if (game->IsKeyDown(DIK_A) && game->IsKeyDown(DIK_RIGHT))
	{
		mario->isRunning = true;
		if (!mario->immovable)
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		else
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_A) && game->IsKeyDown(DIK_LEFT))
	{
		mario->isRunning = true;
		if (!mario->immovable)
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
		else
			mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (mario->vx > 0 && mario->isOnGround)
			mario->SetState(MARIO_STATE_STOP);
		else
		{
			mario->SetState(MARIO_STATE_WALKING_LEFT);
		}
	}
	else if (game->IsKeyDown(DIK_RIGHT))
	{
		if (mario->vx < 0 && mario->isOnGround)
			mario->SetState(MARIO_STATE_STOP);
		else
		{
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
		}
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		if (mario->GetLevel() != MARIO_LEVEL_SMALL && mario->isOnGround)
		{
			mario->Sit();
		}
	}
	else if (game->IsKeyDown(DIK_X))
	{
		mario->SetState(MARIO_STATE_JUMP_LOW);
	}
	else
	{
		if (mario->isOnGround)
			mario->Idle();
	}
}