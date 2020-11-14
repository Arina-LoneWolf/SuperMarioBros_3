#pragma once
#include "Effect.h"

class CTailHitEffect : public CEffect
{
public:
	CTailHitEffect(D3DXVECTOR2 position, int nx);
	void Render();
};

