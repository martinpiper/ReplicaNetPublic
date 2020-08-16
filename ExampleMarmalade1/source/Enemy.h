#ifndef _Enemy_H_
#define _Enemy_H_

#include "BaseObject.h"
#include "_RO_Enemy.h"

class Enemy : _RO_DO_PUBLIC_RO(Enemy) , public BaseObject
{
public:
	Enemy();
	virtual ~Enemy();

	void Init(void);
	void Tick(float timeDelta);
	void Render(void);

	void PostObjectCreate(void);

	int16 mEnemyColour;
	float mPosX,mPosY;
	float mVelX,mVelY;
	int mSeed;
};

#endif
