#include "Number.h"

string CNumber::ConvertToCorrectNumString(int number, int max_num_size)
{
	string num = to_string(number);

	switch (max_num_size)
	{
	case SCORE_NUM_MAX_SIZE:
	case TIME_REMAINING_NUM_MAX_SIZE:
		return string(max_num_size - num.size(), '0').append(num);
		break;

	case WORLD_NUM_MAX_SIZE:
	case LIVES_NUM_MAX_SIZE: // also MONEY_NUM_MAX_SIZE
		return string(max_num_size - num.size(), ' ').append(num);
		break;
	}
}

void CNumber::Render(float x, float y, int number, int max_num_size)
{
	string num_str = ConvertToCorrectNumString(number, max_num_size);

	for (UINT i = 0; i < num_str.size(); i++)
	{
		if (num_str[i] == ' ')
			continue;
		num = CSprites::GetInstance()->Get(numSpriteID.at(num_str[i]));
		num->Draw(x + (i * NUMBER_WIDTH), y);
	}
}

CNumber::~CNumber()
{
}
