#pragma once
#include "Sprites.h"
#include "Textures.h"
#include "define.h"
#include "string"

class CNumber
{
	LPSPRITE num;
	map<char, int> numSpriteID{ { '0', NUM_0 }, { '1', NUM_1 }, { '2', NUM_2 }, { '3', NUM_3 }, { '4', NUM_4 }, { '5', NUM_5 }, { '6', NUM_6 }, { '7', NUM_7 }, { '8', NUM_8 }, { '9', NUM_9 } };

public:
	CNumber() = default;
	string ConvertToCorrectNumString(int number, int max_num_size);
	void Render(float x, float y, int number, int max_num_size);
	~CNumber();
};

