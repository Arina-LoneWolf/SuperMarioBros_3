#include "QuestionBrick.h"

CQuestionBrick::CQuestionBrick()
{
	type = QUESTION_BRICK;
	category = MISC;
}

void CQuestionBrick::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CQuestionBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_SIDE_LENGTH;
	b = y + BRICK_BBOX_SIDE_LENGTH;
}