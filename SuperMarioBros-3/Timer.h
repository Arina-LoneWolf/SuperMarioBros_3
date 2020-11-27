#pragma once
#include "Windows.h"
class CTimer
{
	ULONGLONG start, end;
public:
	CTimer(ULONGLONG end);

	void Start();
	void Stop();
	bool IsTimeUp();
	bool IsStopping();

	~CTimer();
};

