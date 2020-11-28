#include "P_Meter.h"

CP_Meter::CP_Meter(CMario* player)
{
	this->player = player;
	powerMeter = CAnimationSets::GetInstance()->Get(20);
}

void CP_Meter::Update()
{
	float playerSpeed = abs(player->vx);

	if (playerSpeed < SPEED_X_TIER_1)
		powerState = PowerMeter::TIER_0;
	else if (playerSpeed < SPEED_X_TIER_2)
		powerState = PowerMeter::TIER_1;
	else if (playerSpeed < SPEED_X_TIER_3)
		powerState = PowerMeter::TIER_2;
	else if (playerSpeed < SPEED_X_TIER_4)
		powerState = PowerMeter::TIER_3;
	else if (playerSpeed < SPEED_X_TIER_5)
		powerState = PowerMeter::TIER_4;
	else if (playerSpeed < SPEED_X_TIER_6)
		powerState = PowerMeter::TIER_5;
	else if (playerSpeed < SPEED_X_TIER_7)
		powerState = PowerMeter::TIER_6;
	else
		powerState = PowerMeter::TIER_7;
}

void CP_Meter::Render(float x, float y)
{
	powerMeter->at(static_cast<int>(powerState))->Render(x, y);
}

CP_Meter::~CP_Meter()
{
}
