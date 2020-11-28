#pragma once
#include "Windows.h"
class CTimer
{
	ULONGLONG elapsedTime, maxTime;
public:
	CTimer(ULONGLONG end);

	void Start();
	void Stop();
	bool IsTimeUp();
	bool IsStopped();

	ULONGLONG GetElapsedTime() { return elapsedTime; }

	~CTimer();
};