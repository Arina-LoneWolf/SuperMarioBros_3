#pragma once
#include "Effect.h"

class CHitEffect : public CEffect
{
public:
	CHitEffect(D3DXVECTOR2 position);
	void Render();
	~CHitEffect();
};

