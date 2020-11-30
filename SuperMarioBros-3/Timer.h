#pragma once
#include "Windows.h"
class CTimer
{
	ULONGLONG startTime, limitedTime;
public:
	CTimer(ULONGLONG limitedTime);

	void Start();
	void Stop();
	bool IsTimeUp();
	bool IsStopped();
	ULONGLONG GetElapsedTime();

	~CTimer();
};