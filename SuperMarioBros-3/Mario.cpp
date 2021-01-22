#include <algorithm>
#include <assert.h>

#include "Mario.h"
#include "Koopa.h"
#include "Goomba.h"

CMario* CMario::__instance = nullptr;

CMario::CMario(float x, float y) : CGameObject()
{
	category = Category::PLAYER;

	level = MARIO_LEVEL_SMALL;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CMario::UpdateAtOverworldMap(ULONGLONG dt, vector<LPGAMEOBJECT>* coPoints)
{
	CGameObject::Update(dt);
	x += dx;
	y += dy;

	float ml, mt, mr, mb, pl, pt, pr, pb; // main object (o) and the point (p)
	GetBoundingBox(ml, mt, mr, mb);
	for (UINT i = 0; i < coPoints->size(); i++)
	{
		LPGAMEOBJECT e = coPoints->at(i);
		e->GetBoundingBox(pl, pt, pr, pb);
		if (CGameObject::CheckAABB(ml, mt, mr, mb, pl, pt, pr, pb))
		{
			CMapPoint* point = dynamic_cast<CMapPoint*>(coPoints->at(i));
			if (point->id != currentPoint->id)
			{
				float playerLeft, playerTop, playerRight, playerBottom;
				GetBoundingBox(playerLeft, playerTop, playerRight, playerBottom);
				
				if ((vx > 0 && playerRight >= point->rightEdge)
					|| (vx < 0 && playerLeft <= point->leftEdge)
					|| (vy > 0 && playerBottom >= point->bottomEdge)
					|| (vy < 0 && playerTop <= point->topEdge))
				{
					vx = 0;
					vy = 0;
					SetPositionAtCurrentPoint(point->x, point->y);
					movementPermission.assign(point->hasPointAround.begin(), point->hasPointAround.end());
					*currentPoint = *point;
				}
			}
		}
	}
}

void CMario::RenderAtOverworldMap()
{
	switch (level)
	{
	case MARIO_LEVEL_SMALL:
		ani = MARIO_ON_OVERWORLD_MAP_ANI_SMALL;
		break;
	case MARIO_LEVEL_BIG:
		ani = MARIO_ON_OVERWORLD_MAP_ANI_BIG;
		break;
	case MARIO_RACCOON:
		ani = MARIO_ON_OVERWORLD_MAP_ANI_RACCOON;
		break;
	case MARIO_FIRE:
		ani = MARIO_ON_OVERWORLD_MAP_ANI_FIRE;
		break;
	}

	animation_set->at(ani)->Render(x, y);

	if (renderBBOX)
		RenderBoundingBox();
}

void CMario::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	/*if (CGame::GetInstance()->GetCurrentSceneID() == MAP_4_SCENE_ID)
	{
		if (x < CGame::GetInstance()->GetCamPosX())
		{
			walkByAutoCam = true;
			x = CGame::GetInstance()->GetCamPosX();
		}
		else
			walkByAutoCam = false;
	}*/

	// Simple fall down
	if (state == MARIO_STATE_GO_INTO_PIPE)
	{
		vy = MARIO_GO_INTO_PIPE_SPEED_Y;
	}
	else if (state == MARIO_STATE_OUT_OF_PIPE)
	{
		vy = -MARIO_GO_INTO_PIPE_SPEED_Y;
	}
	else if (isWaggingTail && isFalling)
	{
		//DebugOut(L"slowwwwww\n");
		vy += 0.000035f * dt;
	}
	else if (canFly)
	{
		vy += MARIO_FLY_GRAVITY * dt;
	}
	else
	{
		//DebugOut(L"666666666\n");
		//vy += MARIO_GRAVITY * dt;
		vy += 0.0006f * dt;
	}

	if (autoGoRight)
		SetState(MARIO_STATE_WALKING_RIGHT);

	if (level != MARIO_RACCOON)
		canFly = false;

	if (unpressDown && isOnGround)
		isSitting = false;

	if (((inStartOfPipe && GetTop() >= MARIO_UNDER_TOP_OF_PIPE && GetTop() < MARIO_UNDER_TOP_OF_PIPE + 1)
		|| (inStartOfPipe && GetBottom() <= 495 && GetBottom() > 494))
		&& CGame::GetInstance()->GetCurrentSceneID() == MAP_1_SCENE_ID)
	{
		vy = 0;
		screenDim = true;
	}

	if (inStartOfPipe && GetTop() >= 97 && CGame::GetInstance()->GetCurrentSceneID() == MAP_4_SCENE_ID)
	{
		vy = 0;
		screenDim = true;
	}

	if (inEndOfPipe)
		vy = 0;

	if (((state == MARIO_STATE_GO_INTO_PIPE && GetTop() >= MARIO_UNDER_END_OF_PIPE)
		|| (state == MARIO_STATE_OUT_OF_PIPE && GetBottom() <= 385))
		&& (CGame::GetInstance()->GetCurrentSceneID() == MAP_1_SCENE_ID))
	{
		//DebugOut(L"get bottom: %f", GetBottom());
		SetState(MARIO_STATE_IDLE);
	}

	if (CGame::GetInstance()->GetCurrentSceneID() == MAP_4_SCENE_ID && state == MARIO_STATE_OUT_OF_PIPE && GetBottom() <= 144)
	{
		SetState(MARIO_STATE_IDLE);
		onBackyardPipe = true;
	}
	//DebugOut(L"state: %d\n", state);
	//DebugOut(L"vy: %d\n", vy);

#pragma region Wait for animation

	//DebugOut(L"fly time: %d\n", flyTime->GetElapsedTime() / CLOCKS_PER_SEC);
	//DebugOut(L"state: %d\n", state);
	//DebugOut(L"is wagging tail = %d\n", isWaggingTail);
	if (/*canFly && */flyTime->IsTimeUp()) // có cần xét canFly?
	{
		canFly = false;
		flyTime->Stop();
		//DebugOut(L"STOPPPPP FLY\n");
	}

	if (level == MARIO_RACCOON && attackStartTime
		&& GetTickCount64() - attackStartTime < MARIO_SPINNING_TAIL_TIME)
		SetState(MARIO_STATE_ATTACK);
	else if (level == MARIO_FIRE && attackStartTime
		&& GetTickCount64() - attackStartTime < MARIO_SHOOTING_FIREBALL_TIME)
		SetState(MARIO_STATE_ATTACK);
	else
		attackStartTime = 0;

	if (!waggingTailStartTime
		|| GetTickCount64() - waggingTailStartTime >= MARIO_WAGGING_TAIL_TIME)
	{
		waggingTailStartTime = 0;
		isWaggingTail = false;
	}

	if (kickTime->IsTimeUp())
	{
		kickTime->Stop();
		kickShell = false;
	}

#pragma endregion

	if (level == MARIO_FIRE && isAttacking)
	{
		if (listWeapons.size() < 2)
		{
			CreateFireball();
		}
		isAttacking = false;
	}

#pragma region Update weapon and effect

	// update listWeapon
	for (int i = 0; i < listWeapons.size(); i++)
	{
		listWeapons[i]->Update(dt, coObjects);
		if (listWeapons[i]->isFinishedUsing)
		{
			float ax, ay; // accord (a)
			listWeapons[i]->GetPosition(ax, ay);
			CHitEffect* effect = new CHitEffect({ ax, ay }, listWeapons[i]->nx);
			listEffects.push_back(effect);
		}
	}

	for (int i = 0; i < listEffects.size(); i++)
	{
		listEffects[i]->Update(dt, coObjects);
	}

	tail->Update(dt, coObjects);

	// remove weapons and effects have done
	for (int i = 0; i < listWeapons.size(); i++)
	{
		if (listWeapons[i]->isFinishedUsing)
		{
			listWeapons.erase(listWeapons.begin() + i);
		}
	}

	for (int i = 0; i < listEffects.size(); i++)
	{
		if (listEffects[i]->isFinishedUsing)
		{
			listEffects.erase(listEffects.begin() + i);
		}
	}
#pragma endregion

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	/*if (isWaitingForAni && animation_set->at(ani)->IsOver())
	{
		isWaitingForAni = false;
	}*/

	// No collision occured, proceed normally
	//DebugOut(L"coEvent size: %d\n", coEvents.size());
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		if (y > last_y)
		{
			isFalling = true;
			if (!isOnFloatingWood)
			{				
				isOnGround = false;
			}
		}
		else
			isFalling = false;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.1f;

		//DebugOut(L"y = %f\n", y);
		//DebugOut(L"vy = %f\n", vy);
		//DebugOut(L"dt: %d", dt);
		if (nx != 0)
		{
			vx = last_vx;
			immovable = true;
		}
		else
			immovable = false;

		if (ny != 0)
		{
			if (ny < 0)
			{
				isFalling = false;
				isOnGround = true;
				if (canFly)
					isFlying = false;
			}
			vy = 0;
		}

		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState() != ENEMY_STATE_DIE_BY_WEAPON)
					{
						if (goomba->type == Type::YELLOW_GOOMBA || goomba->lostWings)
							goomba->SetState(GOOMBA_STATE_DIE_BY_CRUSH);
						else
							goomba->SetState(GOOMBA_STATE_NORMAL);
						score += 100;
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goomba->GetState() != ENEMY_STATE_DIE_BY_WEAPON)
						{
							CollideWithEnemy();
						}
					}
				}
			}
			else if (dynamic_cast<CKoopa*>(e->obj))
			{
				if (untouchable) return;

				CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);

				if (koopa->type == Type::GREEN_PARAKOOPA || koopa->type == Type::RED_PARAKOOPA)
				{
					if (ny < 0)
					{
						koopa->MakeEffectWhenSteppedOn();
						koopa->SetState(KOOPA_STATE_NORMAL);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
					else
						CollideWithEnemy();
					break;
				}

				if (koopa->GetState() == ENEMY_STATE_MOVE || koopa->GetState() == KOOPA_STATE_SPIN_AND_MOVE)
				{
					if (ny < 0 && nx == 0)
					{
						koopa->MakeEffectWhenSteppedOn();
						koopa->SetState(ENEMY_STATE_IDLE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
					else
						CollideWithEnemy();
				}
				else if (koopa->GetState() == ENEMY_STATE_IDLE || koopa->GetState() == KOOPA_STATE_VIBRATE)
				{
					if (nx != 0 && isRunning)
					{
						isHoldingShell = true;
						koopa->SetState(KOOPA_STATE_BEING_HELD);
					}
					else if (nx == 0 && ny < 0)
					{
						y += dy;
						koopa->object_colliding_nx = this->nx;
						koopa->MakeEffectWhenSteppedOn();
						koopa->SetState(KOOPA_STATE_SPIN_AND_MOVE);
					}
					else
					{
						if (ny != 0 && nx == 0)
							y += dy;
						koopa->object_colliding_nx = this->nx;
						kickShell = true;
						kickTime->Start();
						koopa->SetState(KOOPA_STATE_SPIN_AND_MOVE);
					}
				}
			}
			else if ((e->obj->type == Type::FIRE_PIRANHA || e->obj->type == Type::GREEN_PIRANHA) && !untouchable)
			{
				CollideWithEnemy();
			}
			else if (e->obj->type == Type::BOOMERANG_BROTHER)
			{
				if (e->ny < 0)
				{
					e->obj->SetState(BOOMERANG_BROTHER_STATE_DIE_BY_CRUSH);
					score += 100;
					vy = -MARIO_JUMP_DEFLECT_SPEED;
				}
				else if (e->nx != 0 && untouchable == 0)
					CollideWithEnemy();
			}
			else if (e->obj->type == Type::BRICK_CONTAINS_ITEM)
			{
				if (ny > 0)
				{
					e->obj->SetState(STATE_RAMMED);
				}
			}
			else if (e->obj->type == Type::COIN)
			{
				vy = last_vy;
				e->obj->isFinishedUsing = true;
				score += 50;
				money += 1;
			}
			else if (e->obj->type == Type::BRONZE_BRICK)
			{
				if (e->ny > 0)
				{
					if (level != MARIO_LEVEL_SMALL)
					{
						e->obj->SetState(STATE_DESTROYED);
						CMario::GetInstance()->score += 10;
					}
				}
			}
			else if (e->obj->type == Type::RANDOM_ITEM_BOX)
			{
				e->obj->SetState(STATE_EMPTY);
			}
			else if (e->obj->type == Type::FLOATING_WOOD)
			{
				if (e->ny < 0)
				{
					vy = 0.1f;
					isOnFloatingWood = true;
					e->obj->SetState(STATE_SINKING);
					
				}
			}
			else if (e->obj->type == Type::PIPE && (state == MARIO_STATE_GO_INTO_PIPE || state == MARIO_STATE_OUT_OF_PIPE))
			{
				y += dy;
			}
			else if (dynamic_cast<CColorBox*>(e->obj))
			{
				if (e->nx != 0)
				{
					x += dx;
				}
			}
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}
	}

	last_y = y;
	if (vy)
		last_vy = vy;

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::Render()
{
	//DebugOut(L"ani id khi vừa vào render %d\n", ani);
	/*if (isWaitingForAni)
		goto RENDER;*/

		// Fire
	if (level == MARIO_FIRE)
	{
		switch (state)
		{
		case MARIO_STATE_DIE:
			ani = MARIO_ANI_DIE;
			break;

		case MARIO_STATE_STOP:
			if (!isOnGround)
				goto CASE_FIRE_IS_FALLING;
			if (isHoldingShell)
				goto CASE_FIRE_WALK_AND_HOLD_SHELL;
			if (nx > 0)
				ani = MARIO_FIRE_ANI_STOP_RIGHT;
			else
				ani = MARIO_FIRE_ANI_STOP_LEFT;
			break;

		case MARIO_STATE_WALKING_RIGHT:
			if (isSitting && !isOnGround)
				goto CASE_FIRE_IS_SITTING;
			if (!isOnGround && isHoldingShell)
				goto CASE_FIRE_ON_AIR_AND_HOLD_SHELL;
			if (isHoldingShell)
				goto CASE_FIRE_WALK_AND_HOLD_SHELL;
			if (!isOnGround)
				goto CASE_FIRE_IS_FALLING;
			if (kickShell)
				goto CASE_FIRE_IS_KICKING;
			ani = MARIO_FIRE_ANI_WALK_RIGHT;
			break;

		case MARIO_STATE_WALKING_LEFT:
			if (isSitting && !isOnGround)
				goto CASE_FIRE_IS_SITTING;
			if (!isOnGround && isHoldingShell)
				goto CASE_FIRE_ON_AIR_AND_HOLD_SHELL;
			if (isHoldingShell)
				goto CASE_FIRE_WALK_AND_HOLD_SHELL;
			if (!isOnGround)
				goto CASE_FIRE_IS_FALLING;
			if (kickShell)
				goto CASE_FIRE_IS_KICKING;
			ani = MARIO_FIRE_ANI_WALK_LEFT;
			break;

		case MARIO_STATE_RUNNING_RIGHT:
			if (isSitting && !isOnGround)
				goto CASE_FIRE_IS_SITTING;
			if (!isOnGround && isHoldingShell)
				goto CASE_FIRE_ON_AIR_AND_HOLD_SHELL;
			if (isHoldingShell)
				goto CASE_FIRE_WALK_AND_HOLD_SHELL;
			if (!isOnGround)
				goto CASE_FIRE_IS_FALLING;
			if (vx < MARIO_RUNNING_SPEED)
				ani = MARIO_FIRE_ANI_WALK_RIGHT;
			else
				ani = MARIO_FIRE_ANI_RUNNING_RIGHT;
			break;

		case MARIO_STATE_RUNNING_LEFT:
			if (isSitting && !isOnGround)
				goto CASE_FIRE_IS_SITTING;
			if (!isOnGround && isHoldingShell)
				goto CASE_FIRE_ON_AIR_AND_HOLD_SHELL;
			if (isHoldingShell)
				goto CASE_FIRE_WALK_AND_HOLD_SHELL;
			if (!isOnGround)
				goto CASE_FIRE_IS_FALLING;
			if (vx > -MARIO_RUNNING_SPEED)
				ani = MARIO_FIRE_ANI_WALK_LEFT;
			else
				ani = MARIO_FIRE_ANI_RUNNING_LEFT;
			break;

		case MARIO_STATE_ATTACK:
			if (isOnGround)
			{
				if (nx > 0)
					ani = MARIO_FIRE_ANI_SHOOT_FIREBALL_RIGHT;
				else
					ani = MARIO_FIRE_ANI_SHOOT_FIREBALL_LEFT;
			}
			else
			{
				if (nx > 0)
					ani = MARIO_FIRE_ANI_SHOOT_FIREBALL_WHILE_JUMPING_RIGHT;
				else
					ani = MARIO_FIRE_ANI_SHOOT_FIREBALL_WHILE_JUMPING_LEFT;
			}
			break;

		case MARIO_STATE_JUMP_HIGH:
		case MARIO_STATE_JUMP_LOW:
			if (isSitting)
				goto CASE_FIRE_IS_SITTING;
			if (!isOnGround && isHoldingShell)
				goto CASE_FIRE_ON_AIR_AND_HOLD_SHELL;
			if (vy < 0)
			{
				if (nx > 0)
					ani = MARIO_FIRE_ANI_JUMP_RIGHT;
				else
					ani = MARIO_FIRE_ANI_JUMP_LEFT;
			}
			else
			{
				if (nx > 0)
					ani = MARIO_FIRE_ANI_FALLING_RIGHT;
				else
					ani = MARIO_FIRE_ANI_FALLING_LEFT;
			}
			break;

		CASE_FIRE_IS_SITTING:
		case MARIO_STATE_SIT_DOWN:
			if (nx > 0)
				ani = MARIO_FIRE_ANI_SITTING_RIGHT;
			else
				ani = MARIO_FIRE_ANI_SITTING_LEFT;
			break;

		case MARIO_STATE_IDLE:
			if (!isOnGround && isHoldingShell)
				goto CASE_FIRE_ON_AIR_AND_HOLD_SHELL;
			if (isHoldingShell)
				goto CASE_FIRE_IDLE_AND_HOLD_SHELL;
			if (!isOnGround)
				goto CASE_FIRE_IS_FALLING;
			if (kickShell)
				goto CASE_FIRE_IS_KICKING;
			else
			{
				if (vx > 0 || (nx > 0 && walkByAutoCam))
					ani = MARIO_FIRE_ANI_WALK_RIGHT;
				else if (vx < 0 || (nx < 0 && walkByAutoCam))
					ani = MARIO_FIRE_ANI_WALK_LEFT;
				else
				{
					if (nx > 0)
						ani = MARIO_FIRE_ANI_IDLE_RIGHT;
					else
						ani = MARIO_FIRE_ANI_IDLE_LEFT;
				}
			}
			break;

		CASE_FIRE_IS_KICKING:
		case MARIO_KICK:
			if (nx > 0)
				ani = MARIO_FIRE_ANI_KICK_RIGHT;
			else
				ani = MARIO_FIRE_ANI_KICK_LEFT;
			break;

		CASE_FIRE_WALK_AND_HOLD_SHELL:
		case MARIO_WALKING_WHILE_HOLDING_SHELL:
			if (nx > 0)
				ani = MARIO_FIRE_ANI_WALKING_HOLD_SHELL_RIGHT;
			else
				ani = MARIO_FIRE_ANI_WALKING_HOLD_SHELL_LEFT;
			break;

		CASE_FIRE_IDLE_AND_HOLD_SHELL:
		case MARIO_IDLE_WHILE_HOLDING_SHELL:
			if (nx > 0)
				ani = MARIO_FIRE_ANI_IDLE_HOLD_SHELL_RIGHT;
			else
				ani = MARIO_FIRE_ANI_IDLE_HOLD_SHELL_LEFT;
			break;

		CASE_FIRE_ON_AIR_AND_HOLD_SHELL:
		case MARIO_ON_AIR_WHILE_HOLDING_SHELL:
			if (nx > 0)
				ani = MARIO_FIRE_ANI_ON_AIR_HOLD_SHELL_RIGHT;
			else
				ani = MARIO_FIRE_ANI_ON_AIR_HOLD_SHELL_LEFT;
			break;

		CASE_FIRE_IS_FALLING:
		default:
			if (nx > 0)
				ani = MARIO_FIRE_ANI_FALLING_RIGHT;
			else
				ani = MARIO_FIRE_ANI_FALLING_LEFT;
		}

	}

	// Raccoon
	else if (level == MARIO_RACCOON)
	{
		switch (state)
		{
		case MARIO_STATE_DIE:
			ani = MARIO_ANI_DIE;
			break;

		case MARIO_STATE_STOP:
			if (!isOnGround && !canFly)
			{
				//DebugOut(L"falling\n");
				goto CASE_RACCOON_IS_FALLING;
			}
			if (isHoldingShell)
			{
				//DebugOut(L"hold shell\n");
				goto CASE_RACCOON_WALK_AND_HOLD_SHELL;
			}
			if (nx > 0)
				ani = MARIO_RACCOON_ANI_STOP_RIGHT;
			else
				ani = MARIO_RACCOON_ANI_STOP_LEFT;
			break;

		case MARIO_STATE_WALKING_RIGHT:
			if (isSitting && !isOnGround)
			{
				//DebugOut(L"sittttttttt\n");
				goto CASE_RACCOON_IS_SITTING;
			}
			if (!isOnGround && isHoldingShell)
				goto CASE_RACCOON_ON_AIR_AND_HOLD_SHELL;
			if (isHoldingShell)
				goto CASE_RACCOON_WALK_AND_HOLD_SHELL;
			if (canFly && !isOnGround && isFlying)
				goto CASE_RACCOON_IS_FLYING;
			if (isWaggingTail)
				goto CASE_RACCOON_WAG_TAIL_WHILE_FALLING;
			if (!isOnGround && !canFly)
				goto CASE_RACCOON_IS_FALLING;
			if (kickShell)
				goto CASE_RACCOON_IS_KICKING;
			ani = MARIO_RACCOON_ANI_WALK_RIGHT;
			break;

		case MARIO_STATE_WALKING_LEFT:
			if (isSitting && !isOnGround)
				goto CASE_RACCOON_IS_SITTING;
			if (!isOnGround && isHoldingShell)
				goto CASE_RACCOON_ON_AIR_AND_HOLD_SHELL;
			if (isHoldingShell)
				goto CASE_RACCOON_WALK_AND_HOLD_SHELL;
			if (canFly && !isOnGround && isFlying)
			{
				//DebugOut(L"flyyyyyyyy\n");
				goto CASE_RACCOON_IS_FLYING;
			}
			if (isWaggingTail)
				goto CASE_RACCOON_WAG_TAIL_WHILE_FALLING;
			if (!isOnGround && !canFly)
				goto CASE_RACCOON_IS_FALLING;
			if (kickShell)
				goto CASE_RACCOON_IS_KICKING;
			ani = MARIO_RACCOON_ANI_WALK_LEFT;
			break;

		case MARIO_STATE_RUNNING_RIGHT:
			if (isSitting && !isOnGround)
				goto CASE_RACCOON_IS_SITTING;
			if (!isOnGround && isHoldingShell)
				goto CASE_RACCOON_ON_AIR_AND_HOLD_SHELL;
			if (isHoldingShell)
				goto CASE_RACCOON_WALK_AND_HOLD_SHELL;
			if (canFly && !isOnGround && isFlying)
				goto CASE_RACCOON_IS_FLYING;
			if (!isOnGround && !canFly)
				goto CASE_RACCOON_IS_FALLING;
			if (vx < MARIO_RUNNING_SPEED)
				ani = MARIO_RACCOON_ANI_WALK_RIGHT;
			else
				ani = MARIO_RACCOON_ANI_RUNNING_RIGHT;
			break;

		case MARIO_STATE_RUNNING_LEFT:
			if (isSitting && !isOnGround)
				goto CASE_RACCOON_IS_SITTING;
			if (!isOnGround && isHoldingShell)
				goto CASE_RACCOON_ON_AIR_AND_HOLD_SHELL;
			if (isHoldingShell)
				goto CASE_RACCOON_WALK_AND_HOLD_SHELL;
			if (canFly && !isOnGround && isFlying)
				goto CASE_RACCOON_IS_FLYING;
			if (!isOnGround && !canFly)
				goto CASE_RACCOON_IS_FALLING;
			if (vx > -MARIO_RUNNING_SPEED)
				ani = MARIO_RACCOON_ANI_WALK_LEFT;
			else
				ani = MARIO_RACCOON_ANI_RUNNING_LEFT;
			break;

		CASE_RACCOON_IS_FLYING:
		case MARIO_STATE_FLYING:
			if (isHoldingShell)
				goto CASE_RACCOON_ON_AIR_AND_HOLD_SHELL;
			if (isWaggingTail)
			{
				//DebugOut(L"ani = wagging\n");
				if (nx > 0)
					ani = MARIO_RACCOON_ANI_WAG_TAIL_WHILE_FLYING_RIGHT;
				else
					ani = MARIO_RACCOON_ANI_WAG_TAIL_WHILE_FLYING_LEFT;
			}
			else
			{
				if (vy < 0)
				{
					if (nx > 0)
						ani = MARIO_RACCOON_ANI_FLYING_UP_RIGHT;
					else
						ani = MARIO_RACCOON_ANI_FLYING_UP_LEFT;
				}
				else
				{
					if (nx > 0)
						ani = MARIO_RACCOON_ANI_FLYING_DOWN_RIGHT;
					else
						ani = MARIO_RACCOON_ANI_FLYING_DOWN_LEFT;
				}
			}
			break;

		case MARIO_STATE_ATTACK:
			if (nx > 0)
				ani = MARIO_RACCOON_ANI_SPIN_TAIL_IDLE_RIGHT;
			else
				ani = MARIO_RACCOON_ANI_SPIN_TAIL_IDLE_LEFT;
			break;

		case MARIO_STATE_JUMP_HIGH:
		case MARIO_STATE_JUMP_LOW:
			if (isSitting)
				goto CASE_RACCOON_IS_SITTING;
			if (!isOnGround && isHoldingShell)
				goto CASE_RACCOON_ON_AIR_AND_HOLD_SHELL;
			if (vy < 0)
			{
				if (nx > 0)
					ani = MARIO_RACCOON_ANI_JUMP_RIGHT;
				else
					ani = MARIO_RACCOON_ANI_JUMP_LEFT;
			}
			else
			{
				if (isWaggingTail)
				{
					if (nx > 0)
						ani = MARIO_RACCOON_ANI_FALLING_WAG_TAIL_RIGHT;
					else
						ani = MARIO_RACCOON_ANI_FALLING_WAG_TAIL_LEFT;
				}
				else
				{
					if (nx > 0)
						ani = MARIO_RACCOON_ANI_FALLING_RIGHT;
					else
						ani = MARIO_RACCOON_ANI_FALLING_LEFT;
				}
			}
			break;

		CASE_RACCOON_IS_SITTING:
		case MARIO_STATE_SIT_DOWN:
			if (nx > 0)
				ani = MARIO_RACCOON_ANI_SITTING_RIGHT;
			else
				ani = MARIO_RACCOON_ANI_SITTING_LEFT;
			break;

		case MARIO_STATE_IDLE:
			if (!isOnGround && isHoldingShell)
				goto CASE_RACCOON_ON_AIR_AND_HOLD_SHELL;
			if (isHoldingShell)
				goto CASE_RACCOON_IDLE_AND_HOLD_SHELL;
			if (!isOnGround && !canFly)
			{
				//DebugOut(L"3333333333\n");
				goto CASE_RACCOON_IS_FALLING;
			}
			if (kickShell)
				goto CASE_RACCOON_IS_KICKING;
			if (vx > 0 || (nx > 0 && walkByAutoCam))
				ani = MARIO_RACCOON_ANI_WALK_RIGHT;
			else if (vx < 0 || (nx < 0 && walkByAutoCam))
				ani = MARIO_RACCOON_ANI_WALK_LEFT;
			else
			{
				if (nx > 0)
					ani = MARIO_RACCOON_ANI_IDLE_RIGHT;
				else
					ani = MARIO_RACCOON_ANI_IDLE_LEFT;
			}
			break;

		case MARIO_STATE_GO_INTO_PIPE:
		case MARIO_STATE_OUT_OF_PIPE:
			ani = MARIO_RACCOON_ANI_GO_PIPE;
			break;

		CASE_RACCOON_IS_KICKING:
		case MARIO_KICK:
			if (nx > 0)
				ani = MARIO_RACCOON_ANI_KICK_RIGHT;
			else
				ani = MARIO_RACCOON_ANI_KICK_LEFT;
			break;

		CASE_RACCOON_WALK_AND_HOLD_SHELL:
		case MARIO_WALKING_WHILE_HOLDING_SHELL:
			if (nx > 0)
				ani = MARIO_RACCOON_ANI_WALKING_HOLD_SHELL_RIGHT;
			else
				ani = MARIO_RACCOON_ANI_WALKING_HOLD_SHELL_LEFT;
			break;

		CASE_RACCOON_IDLE_AND_HOLD_SHELL:
		case MARIO_IDLE_WHILE_HOLDING_SHELL:
			if (nx > 0)
				ani = MARIO_RACCOON_ANI_IDLE_HOLD_SHELL_RIGHT;
			else
				ani = MARIO_RACCOON_ANI_IDLE_HOLD_SHELL_LEFT;
			break;

		CASE_RACCOON_ON_AIR_AND_HOLD_SHELL:
		case MARIO_ON_AIR_WHILE_HOLDING_SHELL:
			if (nx > 0)
				ani = MARIO_RACCOON_ANI_ON_AIR_HOLD_SHELL_RIGHT;
			else
				ani = MARIO_RACCOON_ANI_ON_AIR_HOLD_SHELL_LEFT;
			break;

		CASE_RACCOON_WAG_TAIL_WHILE_FALLING:
		case MARIO_WAG_TAIL_WHILE_FALLING:
			if (nx > 0)
				ani = MARIO_RACCOON_ANI_FALLING_WAG_TAIL_RIGHT;
			else
				ani = MARIO_RACCOON_ANI_FALLING_WAG_TAIL_LEFT;
			break;

		CASE_RACCOON_IS_FALLING:
		default:
			//if(!isOnFloatingWood)
			if (vy < 0)
			{
				if (nx > 0)
					ani = MARIO_RACCOON_ANI_JUMP_RIGHT;
				else
					ani = MARIO_RACCOON_ANI_JUMP_LEFT;
			}
			else
			{
				if (nx > 0)
					ani = MARIO_RACCOON_ANI_FALLING_RIGHT;
				else
					ani = MARIO_RACCOON_ANI_FALLING_LEFT;
			}
		}

	}

	// Big
	else if (level == MARIO_LEVEL_BIG)
	{
		switch (state)
		{
		case MARIO_STATE_DIE:
			ani = MARIO_ANI_DIE;
			break;

		case MARIO_STATE_STOP:
			if (!isOnGround)
				goto CASE_BIG_IS_FALLING;
			if (isHoldingShell)
				goto CASE_BIG_WALK_AND_HOLD_SHELL;
			if (nx > 0)
				ani = MARIO_ANI_BIG_STOP_RIGHT;
			else
				ani = MARIO_ANI_BIG_STOP_LEFT;
			break;

		case MARIO_STATE_WALKING_RIGHT:
			if (isSitting && !isOnGround)
				goto CASE_BIG_IS_SITTING;
			if (!isOnGround && isHoldingShell)
				goto CASE_BIG_ON_AIR_AND_HOLD_SHELL;
			if (isHoldingShell)
				goto CASE_BIG_WALK_AND_HOLD_SHELL;
			if (!isOnGround)
				goto CASE_BIG_IS_FALLING;
			if (kickShell)
				goto CASE_BIG_IS_KICKING;
			ani = MARIO_ANI_BIG_WALKING_RIGHT;
			break;

		case MARIO_STATE_WALKING_LEFT:
			if (isSitting && !isOnGround)
				goto CASE_BIG_IS_SITTING;
			if (!isOnGround && isHoldingShell)
				goto CASE_BIG_ON_AIR_AND_HOLD_SHELL;
			if (isHoldingShell)
				goto CASE_BIG_WALK_AND_HOLD_SHELL;
			if (!isOnGround)
				goto CASE_BIG_IS_FALLING;
			if (kickShell)
				goto CASE_BIG_IS_KICKING;
			ani = MARIO_ANI_BIG_WALKING_LEFT;
			break;

		case MARIO_STATE_RUNNING_RIGHT:
			if (isSitting && !isOnGround)
				goto CASE_BIG_IS_SITTING;
			if (!isOnGround && isHoldingShell)
				goto CASE_BIG_ON_AIR_AND_HOLD_SHELL;
			if (isHoldingShell)
				goto CASE_BIG_WALK_AND_HOLD_SHELL;
			if (!isOnGround)
				goto CASE_BIG_IS_FALLING;
			if (vx < MARIO_RUNNING_SPEED)
				ani = MARIO_ANI_BIG_WALKING_RIGHT;
			else
				ani = MARIO_ANI_BIG_RUNNING_RIGHT;
			break;

		case MARIO_STATE_RUNNING_LEFT:
			if (isSitting && !isOnGround)
				goto CASE_BIG_IS_SITTING;
			if (!isOnGround && isHoldingShell)
				goto CASE_BIG_ON_AIR_AND_HOLD_SHELL;
			if (isHoldingShell)
				goto CASE_BIG_WALK_AND_HOLD_SHELL;
			if (!isOnGround)
				goto CASE_BIG_IS_FALLING;
			if (vx > -MARIO_RUNNING_SPEED)
				ani = MARIO_ANI_BIG_WALKING_LEFT;
			else
				ani = MARIO_ANI_BIG_RUNNING_LEFT;
			break;

		case MARIO_STATE_JUMP_HIGH:
		case MARIO_STATE_JUMP_LOW:
			if (isSitting)
				goto CASE_BIG_IS_SITTING;
			//if (isOnGround) // no working
			//	goto CASE_BIG_IS_IDLING;
			if (!isOnGround && isHoldingShell)
				goto CASE_BIG_ON_AIR_AND_HOLD_SHELL;
			if (vy < 0)
			{
				if (nx > 0)
					ani = MARIO_ANI_BIG_JUMP_RIGHT;
				else
					ani = MARIO_ANI_BIG_JUMP_LEFT;
			}
			else
			{
				if (nx > 0)
					ani = MARIO_ANI_BIG_FALLING_RIGHT;
				else
					ani = MARIO_ANI_BIG_FALLING_LEFT;
			}
			break;

		CASE_BIG_IS_SITTING:
		case MARIO_STATE_SIT_DOWN:
			if (nx > 0)
				ani = MARIO_ANI_BIG_SITTING_RIGHT;
			else
				ani = MARIO_ANI_BIG_SITTING_LEFT;
			break;

			//CASE_BIG_IS_IDLING:
		case MARIO_STATE_IDLE:
			if (!isOnGround && isHoldingShell)
				goto CASE_BIG_ON_AIR_AND_HOLD_SHELL;
			if (isHoldingShell)
				goto CASE_BIG_IDLE_AND_HOLD_SHELL;
			if (!isOnGround)
				goto CASE_BIG_IS_FALLING;
			if (kickShell)
				goto CASE_BIG_IS_KICKING;
			if (vx > 0 || (nx > 0 && walkByAutoCam))
				ani = MARIO_ANI_BIG_WALKING_RIGHT;
			else if (vx < 0 || (nx < 0 && walkByAutoCam))
				ani = MARIO_ANI_BIG_WALKING_LEFT;
			else
			{
				if (nx > 0)
					ani = MARIO_ANI_BIG_IDLE_RIGHT;
				else
					ani = MARIO_ANI_BIG_IDLE_LEFT;
			}
			break;

		CASE_BIG_IS_KICKING:
		case MARIO_KICK:
			if (nx > 0)
				ani = MARIO_ANI_BIG_KICK_RIGHT;
			else
				ani = MARIO_ANI_BIG_KICK_LEFT;
			break;

		CASE_BIG_WALK_AND_HOLD_SHELL:
		case MARIO_WALKING_WHILE_HOLDING_SHELL:
			if (nx > 0)
				ani = MARIO_ANI_BIG_WALKING_HOLD_SHELL_RIGHT;
			else
				ani = MARIO_ANI_BIG_WALKING_HOLD_SHELL_LEFT;
			break;

		CASE_BIG_IDLE_AND_HOLD_SHELL:
		case MARIO_IDLE_WHILE_HOLDING_SHELL:
			if (nx > 0)
				ani = MARIO_ANI_BIG_IDLE_HOLD_SHELL_RIGHT;
			else
				ani = MARIO_ANI_BIG_IDLE_HOLD_SHELL_LEFT;
			break;

		CASE_BIG_ON_AIR_AND_HOLD_SHELL:
		case MARIO_ON_AIR_WHILE_HOLDING_SHELL:
			if (nx > 0)
				ani = MARIO_ANI_BIG_ON_AIR_HOLD_SHELL_RIGHT;
			else
				ani = MARIO_ANI_BIG_ON_AIR_HOLD_SHELL_LEFT;
			break;

		CASE_BIG_IS_FALLING:
		default:
			if (nx > 0)
				ani = MARIO_ANI_BIG_FALLING_RIGHT;
			else
				ani = MARIO_ANI_BIG_FALLING_LEFT;
		}
	}

	// Small
	else
	{
		switch (state)
		{
		case MARIO_STATE_DIE:
			ani = MARIO_ANI_DIE;
			break;

		case MARIO_STATE_STOP:
			if (!isOnGround)
				goto CASE_SMALL_IS_FALLING;
			if (isHoldingShell)
				goto CASE_SMALL_WALK_AND_HOLD_SHELL;
			if (nx > 0)
				ani = MARIO_ANI_SMALL_STOP_RIGHT;
			else
				ani = MARIO_ANI_SMALL_STOP_LEFT;
			break;

		case MARIO_STATE_WALKING_RIGHT:
			if (!isOnGround && isHoldingShell)
				goto CASE_SMALL_ON_AIR_AND_HOLD_SHELL;
			if (isHoldingShell)
				goto CASE_SMALL_WALK_AND_HOLD_SHELL;
			if (!isOnGround)
				goto CASE_SMALL_IS_FALLING;
			if (kickShell)
				goto CASE_SMALL_IS_KICKING;
			ani = MARIO_ANI_SMALL_WALKING_RIGHT;
			break;

		case MARIO_STATE_WALKING_LEFT:
			if (!isOnGround && isHoldingShell)
				goto CASE_SMALL_ON_AIR_AND_HOLD_SHELL;
			if (isHoldingShell)
				goto CASE_SMALL_WALK_AND_HOLD_SHELL;
			if (!isOnGround)
				goto CASE_SMALL_IS_FALLING;
			if (kickShell)
				goto CASE_SMALL_IS_KICKING;
			ani = MARIO_ANI_SMALL_WALKING_LEFT;
			break;

		case MARIO_STATE_RUNNING_RIGHT:
			if (!isOnGround && isHoldingShell)
				goto CASE_SMALL_ON_AIR_AND_HOLD_SHELL;
			if (isHoldingShell)
				goto CASE_SMALL_WALK_AND_HOLD_SHELL;
			if (!isOnGround)
				goto CASE_SMALL_IS_FALLING;
			if (vx < MARIO_RUNNING_SPEED)
				ani = MARIO_ANI_SMALL_WALKING_RIGHT;
			else
				ani = MARIO_ANI_SMALL_RUNNING_RIGHT;
			break;

		case MARIO_STATE_RUNNING_LEFT:
			if (!isOnGround && isHoldingShell)
				goto CASE_SMALL_ON_AIR_AND_HOLD_SHELL;
			if (isHoldingShell)
				goto CASE_SMALL_WALK_AND_HOLD_SHELL;
			if (!isOnGround)
				goto CASE_SMALL_IS_FALLING;
			if (vx > -MARIO_RUNNING_SPEED)
				ani = MARIO_ANI_SMALL_WALKING_LEFT;
			else
				ani = MARIO_ANI_SMALL_RUNNING_LEFT;
			break;

		case MARIO_STATE_JUMP_HIGH:
		case MARIO_STATE_JUMP_LOW:
			if (!isOnGround && isHoldingShell)
				goto CASE_BIG_ON_AIR_AND_HOLD_SHELL;
			if (nx > 0)
				ani = MARIO_ANI_SMALL_JUMP_RIGHT;
			else
				ani = MARIO_ANI_SMALL_JUMP_LEFT;
			break;

		case MARIO_STATE_IDLE:
			if (!isOnGround && isHoldingShell)
				goto CASE_SMALL_ON_AIR_AND_HOLD_SHELL;
			if (isHoldingShell)
				goto CASE_SMALL_IDLE_AND_HOLD_SHELL;
			if (!isOnGround)
				goto CASE_SMALL_IS_FALLING;
			if (kickShell)
				goto CASE_SMALL_IS_KICKING;
			else
			{
				if (vx > 0 || (nx > 0 && walkByAutoCam))
					ani = MARIO_ANI_SMALL_WALKING_RIGHT;
				else if (vx < 0 || (nx < 0 && walkByAutoCam))
					ani = MARIO_ANI_SMALL_WALKING_LEFT;
				else
				{
					if (nx > 0)
						ani = MARIO_ANI_SMALL_IDLE_RIGHT;
					else
						ani = MARIO_ANI_SMALL_IDLE_LEFT;
				}
			}
			break;

		CASE_SMALL_IS_KICKING:
		case MARIO_KICK:
			if (nx > 0)
				ani = MARIO_ANI_SMALL_KICK_RIGHT;
			else
				ani = MARIO_ANI_SMALL_KICK_LEFT;
			break;

		CASE_SMALL_WALK_AND_HOLD_SHELL:
		case MARIO_WALKING_WHILE_HOLDING_SHELL:
			if (nx > 0)
				ani = MARIO_ANI_SMALL_WALKING_HOLD_SHELL_RIGHT;
			else
				ani = MARIO_ANI_SMALL_WALKING_HOLD_SHELL_LEFT;
			break;

		CASE_SMALL_IDLE_AND_HOLD_SHELL:
		case MARIO_IDLE_WHILE_HOLDING_SHELL:
			if (nx > 0)
				ani = MARIO_ANI_SMALL_IDLE_HOLD_SHELL_RIGHT;
			else
				ani = MARIO_ANI_SMALL_IDLE_HOLD_SHELL_LEFT;
			break;

		CASE_SMALL_ON_AIR_AND_HOLD_SHELL:
		case MARIO_ON_AIR_WHILE_HOLDING_SHELL:
			if (nx > 0)
				ani = MARIO_ANI_SMALL_ON_AIR_HOLD_SHELL_RIGHT;
			else
				ani = MARIO_ANI_SMALL_ON_AIR_HOLD_SHELL_LEFT;
			break;

		CASE_SMALL_IS_FALLING:
		default:
			if (nx > 0)
				ani = MARIO_ANI_SMALL_JUMP_RIGHT;
			else
				ani = MARIO_ANI_SMALL_JUMP_LEFT;
		}
	}

RENDER:
	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y, alpha);
	//DebugOut(L"ani id %d\n", ani);
	for (int i = 0; i < listWeapons.size(); i++)
	{
		listWeapons[i]->Render();
	}

	for (int i = 0; i < listEffects.size(); i++)
	{
		listEffects[i]->Render();
	}

	tail->Render();

	if (renderBBOX)
		RenderBoundingBox();
}

CMario* CMario::GetInstance()
{
	if (__instance == NULL) __instance = new CMario();
	return __instance;
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		if (vx > MARIO_MAX_WALKING_SPEED)
		{
			DecelerateSlightly();
		}
		vx += MARIO_WALKING_ACCELERATION * dt;
		nx = 1;
		last_vx = vx;
		break;

	case MARIO_STATE_WALKING_LEFT:
		if (vx < -MARIO_MAX_WALKING_SPEED)
		{
			DecelerateSharply();
		}
		vx -= MARIO_WALKING_ACCELERATION * dt;
		nx = -1;
		last_vx = vx;
		break;

	case MARIO_STATE_RUNNING_RIGHT:
		vx += MARIO_RUNNING_ACCELERATION * dt;
		if (vx >= MARIO_RUNNING_SPEED)
		{
			vx = MARIO_RUNNING_SPEED;
			if (level == MARIO_RACCOON)
				canFly = true;
		}
		nx = 1;
		last_vx = vx;
		break;

	case MARIO_STATE_RUNNING_LEFT:
		vx -= MARIO_RUNNING_ACCELERATION * dt;
		if (vx <= -MARIO_RUNNING_SPEED)
		{
			vx = -MARIO_RUNNING_SPEED;
			if (level == MARIO_RACCOON)
				canFly = true;
		}
		nx = -1;
		last_vx = vx;
		break;

	case MARIO_STATE_FLYING:
		if (vx > 0)
		{
			vy = -(MARIO_GRAVITY + 0.004f * 4) * dt;
			vx += MARIO_WALKING_ACCELERATION * dt;
			if (vx > MARIO_MAX_WALKING_SPEED)
				vx = MARIO_MAX_WALKING_SPEED;
			if (vy <= -0.13f)
			{
				vy = -0.13f;
			}
		}
		else
		{
			vy = -(MARIO_GRAVITY + 0.004f * 4) * dt;
			vx -= MARIO_WALKING_ACCELERATION * dt;
			if (vx < -MARIO_MAX_WALKING_SPEED)
				vx = -MARIO_MAX_WALKING_SPEED;
			if (vy <= -0.1f)
			{
				vy = -0.1f;
			}
		}

	case MARIO_STATE_JUMP_LOW:
		if (isOnGround)
		{
			jumpTime->Start();
			vy = -MARIO_LOW_JUMP_SPEED_Y;
			isOnGround = false;
		}
		if (jumpTime->IsTimeUp() && !isFalling)
			vy += MARIO_LOW_JUMP_GRAVITY * dt;
		break;

	case MARIO_STATE_JUMP_HIGH:
		if (isWaggingTail)
			return;
		if (isOnGround)
		{
			y_when_started_to_jump = y;
			vy = -MARIO_HIGH_JUMP_SPEED_Y;
			isOnGround = false;
		}
		break;

	case MARIO_STATE_ATTACK:
		break;

	case MARIO_STATE_STOP:
		if (vx > 0)
			nx = -1;
		else
			nx = 1;
		DecelerateSharply();
		break;

	case MARIO_STATE_SIT_DOWN:

	case MARIO_STATE_IDLE:
		DecelerateSlightly();
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;

	case MARIO_STATE_GO_INTO_PIPE:
	case MARIO_STATE_OUT_OF_PIPE:
		isSitting = false;
		break;
	}

#pragma region Spin tail to attack
	if (GetAni() == MARIO_RACCOON_ANI_SPIN_TAIL_IDLE_RIGHT
		|| GetAni() == MARIO_RACCOON_ANI_SPIN_TAIL_IDLE_LEFT)
	{
		LPANIMATION current_ani = animation_set->at(ani);
		switch (current_ani->GetCurrentFrame())
		{
		case 0:
		case 4:
			if (nx > 0)
			{
				tail->SetPosition(x + 2, y + 20);
				tail->nx = -1;
			}
			else
			{
				tail->SetPosition(x + 23, y + 20);
				tail->nx = 1;
			}
			tail->appear = true;
			break;
		case 2:
			if (nx > 0)
			{
				tail->SetPosition(x + 23, y + 20);
				tail->nx = 1;
			}
			else
			{
				tail->SetPosition(x + 2, y + 20);
				tail->nx = -1;
			}
			tail->appear = true;
			break;
		default:
			tail->appear = false;
		}
	}
	else
		tail->appear = false;
#pragma endregion

}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (onOverworldMap)
	{
		switch (level)
		{
		case MARIO_LEVEL_SMALL:
			left = x - MARIO_ON_OVERWORLD_MAP_SMALL_BBOX_ADDEND;
			top = y;
			break;

		case MARIO_LEVEL_BIG:
		case MARIO_FIRE:
			left = x;
			top = y + MARIO_ON_OVERWORLD_MAP_BIG_BBOX_ADDEND;
			break;

		case MARIO_RACCOON:
			left = x;
			top = y + MARIO_ON_OVERWORLD_MAP_RACCOON_BBOX_ADDEND;
			break;
		}

		right = left + MARIO_ON_OVERWORLD_MAP_BBOX_SIDE_LENGTH;
		bottom = top + MARIO_ON_OVERWORLD_MAP_BBOX_SIDE_LENGTH;

		return;
	}

	if (level == MARIO_LEVEL_SMALL)
	{
		if (nx > 0)
			left = x + MARIO_SMALL_LEFT_SUBTRAHEND_R;
		else
			left = x + MARIO_SMALL_LEFT_SUBTRAHEND_L;

		top = y + MARIO_SMALL_TOP_ADDEND;
		right = left + MARIO_SMALL_BBOX_WIDTH;
	}

	else if (level == MARIO_LEVEL_BIG || level == MARIO_FIRE)
	{
		if (isSitting)
			top = y + MARIO_SIT_BBOX_DIFFERENCE_NUM;
		else
			top = y + MARIO_BIG_TOP_ADDEND;

		if (nx > 0)
			left = x + MARIO_BIG_LEFT_SUBTRAHEND_R;
		else
			left = x + MARIO_BIG_LEFT_SUBTRAHEND_L;

		right = left + MARIO_BIG_BBOX_WIDTH;
	}

	else if (level == MARIO_RACCOON)
	{
		if (isSitting)
			top = y + MARIO_SIT_BBOX_DIFFERENCE_NUM;
		else
			top = y + MARIO_RACCOON_TOP_ADDEND;

		if (nx > 0)
			left = x + MARIO_RACCOON_LEFT_SUBTRAHEND_R;
		else
			left = x + MARIO_RACCOON_LEFT_SUBTRAHEND_L;

		right = left + MARIO_RACCOON_BBOX_WIDTH;
	}

	bottom = y + MARIO_BBOX_HEIGHT;
}

float CMario::GetLeft()
{
	float left = 0.0f;

	if (level == MARIO_LEVEL_SMALL)
	{
		if (nx > 0)
			left = x + MARIO_SMALL_LEFT_SUBTRAHEND_R;
		else
			left = x + MARIO_SMALL_LEFT_SUBTRAHEND_L;
	}

	else if (level == MARIO_LEVEL_BIG || level == MARIO_FIRE)
	{
		if (nx > 0)
			left = x + MARIO_BIG_LEFT_SUBTRAHEND_R;
		else
			left = x + MARIO_BIG_LEFT_SUBTRAHEND_L;
	}

	else if (level == MARIO_RACCOON)
	{
		if (nx > 0)
			left = x + MARIO_RACCOON_LEFT_SUBTRAHEND_R;
		else
			left = x + MARIO_RACCOON_LEFT_SUBTRAHEND_L;
	}

	return left;
}

float CMario::GetTop()
{
	float top = 0.0f;

	if (level == MARIO_LEVEL_SMALL)
		top = y + MARIO_SMALL_TOP_ADDEND;

	else if (level == MARIO_LEVEL_BIG || level == MARIO_FIRE)
	{
		if (isSitting)
			top = y + MARIO_SIT_BBOX_DIFFERENCE_NUM;
		else
			top = y + MARIO_BIG_TOP_ADDEND;
	}

	else if (level == MARIO_RACCOON)
	{
		if (isSitting)
			top = y + MARIO_SIT_BBOX_DIFFERENCE_NUM;
		else
			top = y + MARIO_RACCOON_TOP_ADDEND;
	}

	return top;
}

float CMario::GetBottom()
{
	float bottom = y + MARIO_BBOX_HEIGHT;
	return bottom;
}

void CMario::SetPositionAtCurrentPoint(float pointX, float pointY)
{
	switch (level)
	{
	case MARIO_LEVEL_SMALL:
		this->x = pointX + MARIO_ON_OVERWORLD_MAP_SMALL_BBOX_ADDEND;
		this->y = pointY;
		break;

	case MARIO_LEVEL_BIG:
	case MARIO_FIRE:
		this->x = pointX;
		this->y = pointY - MARIO_ON_OVERWORLD_MAP_BIG_BBOX_ADDEND;
		break;

	case MARIO_RACCOON:
		this->x = pointX;
		this->y = pointY - MARIO_ON_OVERWORLD_MAP_RACCOON_BBOX_ADDEND;
		break;
	}
}

void CMario::DecelerateSharply()
{
	if (isHoldingShell)
	{
		if (vx > 0) {
			vx -= MARIO_LARGEST_ACCELERATION_SUBTRAHEND * dt;
			last_vx = vx;
			if (vx < 0)
				vx = 0;
		}
		else if (vx < 0) {
			vx += MARIO_LARGEST_ACCELERATION_SUBTRAHEND * dt;
			last_vx = vx;
			if (vx > 0)
				vx = 0;
		}
	}
	else
	{
		if (vx > 0) {
			vx -= MARIO_LARGE_ACCELERATION_SUBTRAHEND * dt;
			last_vx = vx;
			if (vx < 0)
				vx = 0;
		}
		else if (vx < 0) {
			vx += MARIO_LARGE_ACCELERATION_SUBTRAHEND * dt;
			last_vx = vx;
			if (vx > 0)
				vx = 0;
		}
	}
}

void CMario::DecelerateSlightly()
{
	if (vx > 0) {
		vx -= MARIO_SMALL_ACCELERATION_SUBTRAHEND * dt;
		last_vx = vx;
		if (vx < 0)
			vx = 0;
	}
	if (vx < 0) {
		vx += MARIO_SMALL_ACCELERATION_SUBTRAHEND * dt;
		last_vx = vx;
		if (vx > 0)
			vx = 0;
	}
}

void CMario::Idle()
{
	SetState(MARIO_STATE_IDLE);
	isSitting = false;
}

void CMario::Sit()
{
	SetState(MARIO_STATE_SIT_DOWN);
	isSitting = true;
}

void CMario::Fly()
{
	//DebugOut(L"fly function\n");
	isWaggingTail = true;
	if (canFly)
		isFlying = true;
	if (isOnGround && flyTime->IsStopped())
	{
		//DebugOut(L"START FLY\n");
		isFlying = true;
		flyTime->Start();
	}
	SetState(MARIO_STATE_FLYING);
	waggingTailStartTime = GetTickCount64();
}

void CMario::Attack()
{
	if (level == MARIO_FIRE && listWeapons.size() == 2)
		return;
	SetState(MARIO_STATE_ATTACK);
	attackStartTime = GetTickCount64();
	if (level == MARIO_FIRE)
		isAttacking = true; // use also for Tail
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	if (lives < 1)
		return;
	lives--;
	SetLevel(MARIO_LEVEL_SMALL);
	SetState(MARIO_STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
	CGame::GetInstance()->cam_x = 0;
	CGame::GetInstance()->cam_y = CAMERA_INITIAL_Y;
}

void CMario::CreateFireball()
{
	CFireball* fireball = new CFireball({ x, y }, nx);
	listWeapons.push_back(fireball);
}

void CMario::RefreshAtPlayScene()
{
	jumpTime->Stop();
	flyTime->Stop();
	kickTime->Stop();
	onOverworldMap = false;
	justPickedReward = false;
	last_vx = 0;
	last_vy = 0;
	vx = 0;
	vy = 0;
	autoGoRight = false;
}

void CMario::RefreshAtOverworldMap()
{
	jumpTime->Stop();
	flyTime->Stop();
	kickTime->Stop();
	onOverworldMap = true;
	justPickedReward = false;
	last_vx = 0;
	last_vy = 0;
	vx = 0;
	vy = 0;
	autoGoRight = false;
}

void CMario::CollideWithEnemy()
{
	if (level > MARIO_LEVEL_BIG)
	{
		level = MARIO_LEVEL_BIG;
		StartUntouchable();
	}
	else if (level > MARIO_LEVEL_SMALL)
	{
		level = MARIO_LEVEL_SMALL;
		StartUntouchable();
	}
	else
		SetState(MARIO_STATE_DIE);
}

void CMario::CheckCollisionWithItems(vector<LPGAMEOBJECT>* listItem)
{
	float ml, mt, mr, mb, il, it, ir, ib; // main object (o) and the item (i)
	GetBoundingBox(ml, mt, mr, mb);
	for (UINT i = 0; i < listItem->size(); i++)
	{
		LPGAMEOBJECT e = listItem->at(i);
		e->GetBoundingBox(il, it, ir, ib);
		if (CGameObject::CheckAABB(ml, mt, mr, mb, il, it, ir, ib))
		{
			CScoreEffect* effect;
			switch (e->type)
			{
			case Type::SUPER_MUSHROOM:
				SetLevel(MARIO_LEVEL_BIG);
				effect = new CScoreEffect({ GetLeft() + 3, GetTop() }, MONEY_EFFECT_1000);
				listEffects.push_back(effect);
				score += 1000;
				break;
			case Type::SUPER_LEAF:
				SetLevel(MARIO_RACCOON);
				effect = new CScoreEffect({ GetLeft() + 3, GetTop() }, MONEY_EFFECT_1000);
				listEffects.push_back(effect);
				score += 1000;
				break;
			case Type::ICE_FLOWER:
				SetLevel(MARIO_FIRE);
				effect = new CScoreEffect({ GetLeft() + 3, GetTop() }, MONEY_EFFECT_1000);
				listEffects.push_back(effect);
				score += 1000;
				break;
			case Type::UP_MUSHROOM:
				lives++;
				effect = new CScoreEffect({ GetLeft() + 6, GetTop() }, MONEY_EFFECT_1_UP);
				listEffects.push_back(effect);
				break;
			case Type::P_SWITCH:
				if (vy > 0 && e->state != STATE_PRESSED)
				{
					vy = -MARIO_JUMP_DEFLECT_SPEED;
					e->SetState(STATE_PRESSED);
				}
				break;
			}
			if (e->type != Type::P_SWITCH)
				e->isFinishedUsing = true;
		}
	}
}


