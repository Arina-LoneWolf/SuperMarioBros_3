#include "Timer.h"

CTimer::CTimer(ULONGLONG maxTime)
{
	startTime = 0;
	this->maxTime = maxTime;
}

void CTimer::Start()
{
	startTime = GetTickCount64();
}

void CTimer::Stop()
{
	startTime = 0;
}

bool CTimer::IsTimeUp()
{
	return (startTime && GetTickCount64() - startTime > maxTime);
}

bool CTimer::IsStopped()
{
	return startTime == 0;
}

ULONGLONG CTimer::GetElapsedTime()
{
	return GetTickCount64() - startTime;
}

CTimer::~CTimer()
{
}
