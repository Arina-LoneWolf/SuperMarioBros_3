#include "Timer.h"

CTimer::CTimer(ULONGLONG end)
{
	start = 0;
	this->end = end;
}

void CTimer::Start()
{
	start = GetTickCount64();
}

void CTimer::Stop()
{
	start = 0;
}

bool CTimer::IsTimeUp()
{
	return (start && GetTickCount64() - start > end);
}

bool CTimer::IsStopping()
{
	return start == 0;
}

CTimer::~CTimer()
{
}
