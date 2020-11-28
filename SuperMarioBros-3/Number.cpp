#include "Number.h"

CNumber::CNumber()
{
}

string CNumber::ConvertToCorrectNumber(int number, int max_num_size)
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
	}
}

void CNumber::Render(float x, float y, int number, int max_num_size)
{
	string num_str = ConvertToCorrectNumber(number, max_num_size);

	for (UINT i = 0; i < num_str.size(); i++)
	{
		if (num_str[i] == ' ')
			continue;

		if (num_str[i] == '0')
		{
			num = CSprites::GetInstance()->Get(NUM_0);
		}
		else if (num_str[i] == '1')
		{
			num = CSprites::GetInstance()->Get(NUM_1);
		}
		else if (num_str[i] == '2')
		{
			num = CSprites::GetInstance()->Get(NUM_2);
		}
		else if (num_str[i] == '3')
		{
			num = CSprites::GetInstance()->Get(NUM_3);
		}
		else if (num_str[i] == '4')
		{
			num = CSprites::GetInstance()->Get(NUM_4);
		}
		else if (num_str[i] == '5')
		{
			num = CSprites::GetInstance()->Get(NUM_5);
		}
		else if (num_str[i] == '6')
		{
			num = CSprites::GetInstance()->Get(NUM_6);
		}
		else if (num_str[i] == '7')
		{
			num = CSprites::GetInstance()->Get(NUM_7);
		}
		else if (num_str[i] == '8')
		{
			num = CSprites::GetInstance()->Get(NUM_8);
		}
		else if (num_str[i] == '9')
		{
			num = CSprites::GetInstance()->Get(NUM_9);
		}

		num->Draw(x + (i * NUMBER_WIDTH), y);
	}
}

CNumber::~CNumber()
{
}
