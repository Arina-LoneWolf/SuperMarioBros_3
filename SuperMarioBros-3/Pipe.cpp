#include "Pipe.h"

CPipe::CPipe(int pipeType)
{
	type = Type::PIPE;
	category = Category::MISC;
	this->pipeType = pipeType;
}

void CPipe::Render()
{
	if (pipeType == TypeOfPipe::SHORT_PIPE)
		ani = SHORT_PIPE_ANI;
	else if (pipeType == TypeOfPipe::MEDIUM_PIPE)
		ani = MEDIUM_PIPE_ANI;
	else
		ani = LONG_PIPE_ANI;

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + PIPE_WIDTH;

	if (pipeType == TypeOfPipe::SHORT_PIPE)
		b = y + SHORT_PIPE_HEIGHT;
	else if (pipeType == TypeOfPipe::MEDIUM_PIPE)
		b = y + MEDIUM_PIPE_HEIGHT;
	else
		b = y + LONG_PIPE_HEIGHT;
}
