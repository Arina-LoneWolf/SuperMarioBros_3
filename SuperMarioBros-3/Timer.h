#pragma once
#include "Windows.h"
class CTimer
{
	ULONGLONG startTime, maxTime;
public:
	CTimer(ULONGLONG end);

	void Start();
	void Stop();
	bool IsTimeUp();
	bool IsStopped();

	ULONGLONG GetElapsedTime();

	~CTimer();
};