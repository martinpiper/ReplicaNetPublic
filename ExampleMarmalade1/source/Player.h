#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "BaseObject.h"
#include "_RO_Player.h"

// MPi: TODO: Tidy
extern void DrawLineRGB(int x1, int y1, int x2, int y2, int r, int g, int b);
extern void DrawLine(int x1, int y1, int x2, int y2, int16 color);


class Player : _RO_DO_PUBLIC_RO(Player) , public BaseObject
{
public:
	Player();
	virtual ~Player();

	void Init(void);
	void Tick(float timeDelta);
	void Render(void);

	void PostObjectCreate(void);

	int16 mPlayerColour;
	float mPosX,mPosY;
	float mRotation;

	float mTargetPosX,mTargetPosY;
};

#endif
