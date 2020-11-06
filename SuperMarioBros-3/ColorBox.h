#include "GameObject.h"


class CColorBox : public CGameObject
{
public:
	CColorBox(int width_in_units, int height_in_units);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};