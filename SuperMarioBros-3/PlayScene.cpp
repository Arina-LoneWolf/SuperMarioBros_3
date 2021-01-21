#include <iostream>
#include <fstream>

#include "PlayScene.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) : CScene(id, filePath)
{
	key_handler = new CPlaySceneKeyHandler(this);
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

	Type object_type = static_cast<Type>(atoi(tokens[0].c_str()));
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	CGameObject* obj = NULL;
	CBronzeBrick* brick = NULL;

	switch (object_type)
	{
	case Type::MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}

		player = CMario::GetInstance();
		player->justPickedReward = false;
		player->RefreshAtPlayScene();
		player->SetPosition(x, y);
		player->SetAnimationSet(ani_set);
		HUD = new CStatusBar(player);
		cam = new Camera(player);

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
		int width = atoi(tokens[4].c_str());
		int height = atoi(tokens[5].c_str());
		obj = new CFloor(width, height);
		break;
	}

	case Type::COLOR_BOX:
	{
		int width = atoi(tokens[4].c_str()); // Why float?
		int height = atoi(tokens[5].c_str());
		obj = new CColorBox(width, height);
		break;
	}

	case Type::BRICK_CONTAINS_ITEM:
	{
		int brickType = atoi(tokens[4].c_str());
		int itemType = atoi(tokens[5].c_str());
		obj = new CBrickContainsItem(brickType, itemType, y);
		break;
	}

	case Type::FIRE_PIRANHA:
	{
		int piranhaType = atoi(tokens[4].c_str());
		obj = new CFirePiranha(player, piranhaType);
		break;
	}

	case Type::GREEN_PIRANHA: obj = new CGreenPiranha(player); break;
	case Type::RANDOM_ITEM_BOX: obj = new CRandomItemBox(); break;
	case Type::FLOATING_WOOD: obj = new CFloatingWood(); break;

	case Type::PIPE:
	{
		int pipeType = atoi(tokens[4].c_str());
		obj = new CPipe(pipeType);
		break;
	}

	case Type::RED_KOOPA:
	case Type::GREEN_KOOPA:
	case Type::RED_PARAKOOPA:
	case Type::GREEN_PARAKOOPA:
	{
		obj = new CKoopa(player, x, y, object_type);
		break;
	}

	case Type::COIN:
	case Type::BRONZE_BRICK:
	{
		int transformation = atoi(tokens[4].c_str());
		brick = new CBronzeBrick(transformation);
		break;
	}

	case Type::YELLOW_GOOMBA:
	case Type::RED_PARAGOOMBA:
		obj = new CGoomba(player, object_type); break;

	case Type::BOOMERANG_BROTHER: obj = CBoomerangBrother::GetInstance(); break;

	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup


	if (brick)
	{
		brick->SetType(object_type);
		brick->SetPosition(x, y);
		brick->SetAnimationSet(ani_set);
		listBronzeBricks.push_back(brick);
	}
	else if (obj)
	{
		obj->SetType(object_type);
		obj->SetPosition(x, y);
		obj->SetAnimationSet(ani_set);
		objects.push_back(obj);
	}
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
	CTextures::GetInstance()->Add(ID_TEX_DARKEN, L"textures\\darken-the-screen.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(ULONGLONG dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;

	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < listBronzeBricks.size(); i++)
	{
		coObjects.push_back(listBronzeBricks[i]);
	}

	if (player)
		player->Update(dt, &coObjects);

	cam->Update(dt);

	for (LPGAMEOBJECT item : priorityListItems)
		item->Update(dt, &coObjects);

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
		LPGAMEOBJECT e = objects[i];

		if (e->type == Type::BRICK_CONTAINS_ITEM)
		{
			CBrickContainsItem* brick = dynamic_cast<CBrickContainsItem*>(e);
			if (brick->isAboutToDropItem && !brick->dropped)
			{
				DropItem(brick->itemType, brick->x, brick->y);
				if (brick->itemType == ItemOfBrick::ITEM_COIN_X10 && brick->coin > 0)
				{
					brick->isAboutToDropItem = false;
				}
				else
					brick->dropped = true;
			}
		}
		else if (e->type == Type::P_SWITCH)
		{
			CP_Switch* p_switch = dynamic_cast<CP_Switch*>(e);
			if (p_switch->GetState() == STATE_PRESSED && p_switch->readyToPerform)
			{
				p_switch->PerformBricksTransformation(&listBronzeBricks);
				p_switch->readyToPerform = false;
			}
		}
	}

	player->CheckCollisionWithItems(&listItems);
	player->CheckCollisionWithItems(&priorityListItems);

	for (size_t i = 0; i < listBronzeBricks.size(); i++)
	{
		listBronzeBricks[i]->Update(dt, &coObjects);
	}

	for (LPGAMEOBJECT item : listItems)
		item->Update(dt, &coObjects);

	// remove objects
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->isFinishedUsing)
			objects.erase(objects.begin() + i);
	}

	for (size_t i = 0; i < listBronzeBricks.size(); i++)
	{
		if (listBronzeBricks[i]->isFinishedUsing)
		{
			listBronzeBricks.erase(listBronzeBricks.begin() + i);
		}
	}

	// remove items
	for (size_t i = 0; i < listItems.size(); i++)
	{
		if (listItems[i]->isFinishedUsing)
			listItems.erase(listItems.begin() + i);
	}


	for (size_t i = 0; i < priorityListItems.size(); i++)
	{
		if (priorityListItems[i]->isFinishedUsing)
			priorityListItems.erase(priorityListItems.begin() + i);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;

	//CGame::GetInstance()->cam_y = 200;
	float playerLeft = player->x + 11;

	//if (CGame::GetInstance()->GetCurrentSceneID() == MAP_4_SCENE_ID && CGame::GetInstance()->GetCamPosX() + SCREEN_WIDTH / SCREEN_DIVISOR < 2079) // thêm 1 dk là đang ở trong vùng autocam
	//{
	//	if (!player->pauseCam && CGame::GetInstance()->GetCamPosX() + SCREEN_WIDTH / SCREEN_DIVISOR < 2063)
	//		CGame::GetInstance()->cam_x += 0.03f * dt;
	//}
	/*else*/ if (playerLeft > (5 * SCREEN_WIDTH / 28) && playerLeft + (5 * SCREEN_WIDTH / 28) < map->GetWidthTileMap())
	{
		cx = playerLeft - (5 * SCREEN_WIDTH / 28);
		CGame::GetInstance()->cam_x = cx;
	}

	HUD->Update();

	if (alpha == ALPHA_MAX_VALUE && player->screenDim)
	{
		player->screenDim = false;
		player->inStartOfPipe = false;
		player->goBackyard = true;

		if (player->goHiddenArea)
		{
			cam->goToHiddenArea = true;
			player->goHiddenArea = false;
		}
		else if (player->leaveHiddenArea)
		{
			cam->backFromHiddenArea = true;
			player->leaveHiddenArea = false;
		}
		else if (player->goBackyard)
		{
			cam->goToBackyard = true;
			player->goBackyard = false;
		}
		lighteningIsDone = false;
	}

	if (cam->inHiddenArea && !lighteningIsDone && alpha == ALPHA_MIN_VALUE)
	{
		player->readyToOutOfPipe = true;
		lighteningIsDone = true;
	}
}

void CPlayScene::Render()
{
	map->Draw();

	for (LPGAMEOBJECT item : priorityListItems)
		item->Render();

	for (int i = listBronzeBricks.size() - 1; i >= 0; i--)
		listBronzeBricks[i]->Render();

	if (player)
		player->Render();

	for (int i = objects.size() - 1; i >= 0; i--)
		objects[i]->Render();

	for (LPGAMEOBJECT item : listItems)
		item->Render();

	HUD->Render(CGame::GetInstance()->GetCamPosX(), CGame::GetInstance()->GetCamPosY());

	if (player->screenDim)
		DarkenTheScreen();

	if (cam->inHiddenArea && !lighteningIsDone)
		LightenTheScreen();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	for (int i = 0; i < listBronzeBricks.size(); i++)
		delete listBronzeBricks[i];

	for (int i = 0; i < listItems.size(); i++)
		delete listItems[i];

	for (int i = 0; i < priorityListItems.size(); i++)
		delete priorityListItems[i];

	objects.clear();
	listBronzeBricks.clear();
	listItems.clear();
	priorityListItems.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScene::DropItem(int itemType, float x, float y)
{
	switch (itemType)
	{
	case ITEM_RANDOM:
	{
		if (player->GetLevel() == MARIO_LEVEL_SMALL)
		{
			CMushroom* mushroom = new CMushroom(x, y, TYPE_SUPER_MUSHROOM);
			priorityListItems.push_back(mushroom);
		}
		else if (player->GetLevel() == MARIO_RACCOON)
		{
			CIceFlower* flower = new CIceFlower(x, y);
			priorityListItems.push_back(flower);
		}
		else
		{
			CSuperLeaf* leaf = new CSuperLeaf(x, y);
			listItems.push_back(leaf);
		}
		break;
	}
	case ITEM_COIN:
	{
		CCoinEffect* effect = new CCoinEffect(x, y);
		listItems.push_back(effect);
		break;
	}
	case ITEM_1_UP_MUSHROOM:
	{
		CMushroom* mushroom = new CMushroom(x, y, TYPE_UP_MUSHROOM);
		priorityListItems.push_back(mushroom);
		break;
	}
	case ITEM_P_SWITCH:
	{
		CP_Switch* p_switch = new CP_Switch();
		objects.push_back(p_switch);
		break;
	}
	case ITEM_COIN_X10:
	{
		CCoinEffect* effect = new CCoinEffect(x, y);
		listItems.push_back(effect);
		break;
	}
	}
}

void CPlayScene::DarkenTheScreen()
{
	colorSubtrahend += COLOR_ADDEND_LEVEL_UP;
	alpha = floor(alpha + colorSubtrahend);
	if (alpha > ALPHA_MAX_VALUE)
	{
		alpha = ALPHA_MAX_VALUE;
		colorSubtrahend = 0;
	}

	CSprites::GetInstance()->Get(TRANSITION_SPRITE_ID)->Draw(CGame::GetInstance()->GetCamPosX(), CGame::GetInstance()->GetCamPosY(), alpha);
}

void CPlayScene::LightenTheScreen()
{
	colorSubtrahend += COLOR_ADDEND_LEVEL_UP;
	alpha = floor(alpha - colorSubtrahend);
	if (alpha < ALPHA_MIN_VALUE)
	{
		alpha = ALPHA_MIN_VALUE;
		colorSubtrahend = 0;
	}

	CSprites::GetInstance()->Get(TRANSITION_SPRITE_ID)->Draw(CGame::GetInstance()->GetCamPosX(), CGame::GetInstance()->GetCamPosY(), alpha);
}

void CPlaySceneKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario* mario = ((CPlayScene*)scene)->GetPlayer();

	if (mario->autoGoRight)
		return;

	switch (KeyCode)
	{
	case DIK_SPACE: // reset the game starting as small mario
		mario->Reset();
		break;

		if (mario->GetState() == MARIO_STATE_DIE)
			return;

	case DIK_S: // high jump
		if (mario->onPressUp)
			return;

		if (mario->canFly && mario->GetLevel() == MARIO_RACCOON)
		{
			mario->Fly();
		}
		else
		{
			if (!mario->isOnGround && mario->GetLevel() == MARIO_RACCOON)
				mario->isWaggingTail = true;
			mario->SetState(MARIO_STATE_JUMP_HIGH);
			mario->waggingTailStartTime = GetTickCount64();
		}
		break;

	case DIK_A: // attack enemies while in Fire or Raccoon transformation
		if ((mario->GetLevel() == MARIO_FIRE || mario->GetLevel() == MARIO_RACCOON) && !mario->isSitting)
			mario->Attack();
		break;

	case DIK_DOWN:
		mario->unpressDown = false;
		if ((int)mario->GetBottom() == MARIO_AT_HIDDEN_AREA_ENTRANCE && mario->GetLevel() == MARIO_RACCOON)
		{
			mario->goHiddenArea = true;
			mario->inStartOfPipe = true;
			mario->SetState(MARIO_STATE_GO_INTO_PIPE);
		}

		if ((int)mario->GetBottom() == 95 && (int)mario->GetLeft() > 1939 && CGame::GetInstance()->GetCurrentSceneID() == MAP_4_SCENE_ID)
		{
			mario->inStartOfPipe = true;
			mario->SetState(MARIO_STATE_GO_INTO_PIPE);
		}
		break;

	case DIK_UP:
		if (mario->GetTop() > 495 && mario->GetLeft() > 2321 && mario->GetLeft() < 2333)
			mario->onPressUp = true;
		break;

	case DIK_R: // turn into raccoon mario
		mario->SetLevel(MARIO_RACCOON);
		break;

	case DIK_F: // turn into fire mario
		mario->SetLevel(MARIO_FIRE);
		break;

	case DIK_B: // turn into big mario
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;

	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;

	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;

	case DIK_3:
		mario->SetLevel(MARIO_RACCOON);
		break;

	case DIK_4:
		mario->SetLevel(MARIO_FIRE);
		break;

	case DIK_5:
		mario->renderBBOX ^= true; // x ^= a <=>  x = x ^ a and '^' <=> XOR
		//mario->renderBBOX = !mario->renderBBOX;
		//mario->renderBBOX = mario->renderBBOX == false;
		//mario->renderBBOX = abs(mario->renderBBOX - 1);
		break;

	case DIK_P:
		mario->pauseCam ^= true;
		break;

	case DIK_Q:
		mario->onOverworldMap = true;
		CGame::GetInstance()->SwitchScene(OVERWORLD_MAP_SCENE_ID);

		break;

	case DIK_E:
		if (CGame::GetInstance()->GetCurrentSceneID() == MAP_1_SCENE_ID)
			CGame::GetInstance()->SwitchScene(MAP_4_SCENE_ID);
		else
			CGame::GetInstance()->SwitchScene(MAP_1_SCENE_ID);
		break;
	}
}

void CPlaySceneKeyHandler::OnKeyUp(int KeyCode)
{
	CMario* mario = ((CPlayScene*)scene)->GetPlayer();

	if (mario->GetState() == MARIO_STATE_DIE || mario->autoGoRight)
		return;

	switch (KeyCode)
	{
	case DIK_S:
		if (mario->canFly)
			return;
		if (mario->onPressUp)
			return;
		if (mario->vy < 0 && !mario->isWaggingTail)
		{
			float factor = 15 - ((mario->y_when_started_to_jump - mario->y) / 10) * 2;
			mario->vy += MARIO_GRAVITY * factor * mario->dt;
		}
		break;

	case DIK_X:
		mario->vy += MARIO_GRAVITY * 7 * mario->dt;
		break;

	case DIK_UP:
		mario->onPressUp = false;
		break;

	case DIK_A:
		mario->isRunning = false;
		mario->isHoldingShell = false;
		break;

	case DIK_DOWN:
		if (mario->isOnGround)
			mario->isSitting = false;
		mario->unpressDown = true;
		break;
	}
}

void CPlaySceneKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = ((CPlayScene*)scene)->GetPlayer();

	if (mario->GetState() == MARIO_STATE_DIE || mario->autoGoRight)
		return;
	if (mario->GetState() == MARIO_STATE_GO_INTO_PIPE || mario->GetState() == MARIO_STATE_OUT_OF_PIPE)
		return;

	if ((game->IsKeyDown(DIK_LEFT) && game->IsKeyDown(DIK_RIGHT))
		|| (game->IsKeyDown(DIK_DOWN) && game->IsKeyDown(DIK_UP)))
	{
		mario->Idle();
	}
	else if (game->IsKeyDown(DIK_A) && game->IsKeyDown(DIK_RIGHT))
	{
		mario->isRunning = true;
		if (mario->vx < 0 && mario->isOnGround)
			mario->SetState(MARIO_STATE_STOP);
		else if (!mario->immovable)
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		else
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_A) && game->IsKeyDown(DIK_LEFT))
	{
		mario->isRunning = true;
		if (mario->vx > 0 && mario->isOnGround)
			mario->SetState(MARIO_STATE_STOP);
		else if (!mario->immovable)
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
		else if (mario->isOnGround)
			mario->SetState(MARIO_STATE_IDLE);
	}
	else if (game->IsKeyDown(DIK_UP) && mario->GetTop() > 495)
	{
		if (game->IsKeyDown(DIK_S) && mario->GetLevel() == MARIO_RACCOON && mario->isOnGround
			&& mario->GetLeft() > 2321 && mario->GetLeft() < 2333)
		{
			if (mario->GetState() == MARIO_STATE_OUT_OF_PIPE)
				return;
			mario->leaveHiddenArea = true;
			mario->inStartOfPipe = true;
			mario->SetState(MARIO_STATE_OUT_OF_PIPE);
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