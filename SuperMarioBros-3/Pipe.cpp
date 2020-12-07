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
	else if (pipeType == TypeOfPipe::LONG_PIPE)
		ani = LONG_PIPE_ANI;
	else if (pipeType == TypeOfPipe::HIDDEN_PIPE)
		ani = HIDDEN_PIPE_ANI;
	else
		ani = MEDIUM_PIPE_PAIR_WITH_LONG_ONE_ANI;

	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + PIPE_WIDTH;
	
	if (pipeType == TypeOfPipe::LONG_PIPE)
		b = y + LONG_PIPE_HEIGHT;
	else if (pipeType == TypeOfPipe::MEDIUM_PIPE || pipeType == TypeOfPipe::MEDIUM_PIPE_PAIR_WITH_LONG_ONE)
		b = y + MEDIUM_PIPE_HEIGHT;
	else
		b = y + SHORT_PIPE_HEIGHT;
}
