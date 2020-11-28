#include "Timer.h"

CTimer::CTimer(ULONGLONG maxTime)
{
	elapsedTime = 0;
	this->maxTime = maxTime;
}

void CTimer::Start()
{
	elapsedTime = GetTickCount64();
}

void CTimer::Stop()
{
	elapsedTime = 0;
}

bool CTimer::IsTimeUp()
{
	return (elapsedTime && GetTickCount64() - elapsedTime > maxTime);
}

bool CTimer::IsStopped()
{
	return elapsedTime == 0;
}

CTimer::~CTimer()
{
}
