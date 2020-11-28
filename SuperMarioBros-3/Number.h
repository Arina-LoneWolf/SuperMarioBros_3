#pragma once
#include "Sprites.h"
#include "Textures.h"
#include "define.h"
#include "string"

class CNumber
{
	LPSPRITE num;
public:
	CNumber();
	string ConvertToCorrectNumString(int number, int max_num_size);
	void Render(float x, float y, int number, int max_num_size);
	~CNumber();
};

