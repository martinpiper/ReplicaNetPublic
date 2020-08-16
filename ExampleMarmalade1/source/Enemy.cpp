#include "s3e.h"
#include <math.h>
#include "main.h"
#include "Enemy.h"
#include "RNReplicaNet/Inc/ReplicaNet.h"

Enemy::Enemy()
{
	int32 width = s3eSurfaceGetInt(S3E_SURFACE_WIDTH);
	int32 height = s3eSurfaceGetInt(S3E_SURFACE_HEIGHT);

	if (IsMaster())
	{
		mSeed = rand();
		mPosX = (float)(rand() % width);
		mPosY = (float)(rand() % height);


		mVelX = -32.0f + ((float)(rand() & 63) / 2.0f);
		mVelY = -32.0f + ((float)(rand() & 63) / 2.0f);
	}
}

Enemy::~Enemy()
{
}

void Enemy::PostObjectCreate(void)
{
}

void Enemy::Tick(float timeDelta)
{
	if (IsMaster())
	{
		float width = (float)s3eSurfaceGetInt(S3E_SURFACE_WIDTH);
		float height = (float)s3eSurfaceGetInt(S3E_SURFACE_HEIGHT);

		if ( ((mPosX < 0) && (mVelX < 0)) || ((mPosX > width) && (mVelX > 0)) )
		{
			ContinuityBreak(mPosX,RNReplicaNet::DataBlock::kSuddenChange);	// Flag a change for the position since the velocity has changed direction
			mVelX = -mVelX;
		}

		if ( ((mPosY < 0) && (mVelY < 0)) || ((mPosY > height) && (mVelY > 0)) )
		{
			ContinuityBreak(mPosY,RNReplicaNet::DataBlock::kSuddenChange);	// Flag a change for the position since the velocity has changed direction
			mVelY = -mVelY;
		}

		mPosX += mVelX * timeDelta;
		GiveDeltaHint(mPosX,mVelX);
		mPosY += mVelY * timeDelta;
		GiveDeltaHint(mPosY,mVelY);
	}
}

// Random number generation
#define N 624
#define M 397
#define MATRIX_A 0x9908b0df
#define UPPER_MASK 0x80000000
#define LOWER_MASK 0x7fffffff

/* Tempering parameters */   
#define TEMPERING_MASK_B 0x9d2c5680
#define TEMPERING_MASK_C 0xefc60000
#define TEMPERING_SHIFT_U(y)  (y >> 11)
#define TEMPERING_SHIFT_S(y)  (y << 7)
#define TEMPERING_SHIFT_T(y)  (y << 15)
#define TEMPERING_SHIFT_L(y)  (y >> 18)

static unsigned long mt[N];
static int mti=N+1;

void sgenrand(unsigned long seed)
{
	// [KNUTH 1981, The Art of Computer Programming Vol. 2 (2nd Ed.), pp102]
	mt[0]= seed & 0xffffffff;
	for (mti=1; mti<N; mti++)
	{
		mt[mti] = (69069 * mt[mti-1]) & 0xffffffff;
	}
}

unsigned long genrand()
{
	unsigned long y;
	static unsigned long mag01[2]={0x0, MATRIX_A};

	if (mti >= N)
	{
		int kk;

		if (mti == N+1)
		{
			sgenrand(4357);
		}

		for (kk=0;kk<N-M;kk++)
		{
			y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
			mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		for (;kk<N-1;kk++)
		{
			y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
			mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
		mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1];

		mti = 0;
	}

	y = mt[mti++];
	y ^= TEMPERING_SHIFT_U(y);
	y ^= TEMPERING_SHIFT_S(y) & TEMPERING_MASK_B;
	y ^= TEMPERING_SHIFT_T(y) & TEMPERING_MASK_C;
	y ^= TEMPERING_SHIFT_L(y);

	return y; 
}


void Enemy::Render(void)
{
	float dx,dy;
	float newX,newY;
	float startX,startY;
	float newRot;
	float len;

	// MPi: TODO: This means all random numbers will be seeded for each render. If random numbers are needed during the game update loop
	// this can cause issues, so consider using a different random number generator, or using some kind of tracked seed for the game
	// update loop.

	// rand() is not consistent across platforms given the same seed. So this is using a local version of rand() for rendering.
	sgenrand(mSeed);

	newRot = (float) ((mSeed & 31)-16) * (float) GetBoundReplicaNet()->GetTime();
	int segments = (mSeed & 7) + 5;
	len = (float)5.0f + (genrand() & 31);
	startX = mPosX + (sinf(DEG2RAD(newRot)) * len);
	startY = mPosY + (cosf(DEG2RAD(newRot)) * len);
	dx = startX;
	dy = startY;

	int i;
	for (i=0;i<segments;i++)
	{
		len = (float)5.0f + (genrand() & 31);

		newX = mPosX + (sinf(DEG2RAD(newRot)) * len);
		newY = mPosY + (cosf(DEG2RAD(newRot)) * len);
		DrawLineRGB((int) dx,(int) dy,(int) newX,(int) newY, 255,255,255);
		dx = newX;
		dy = newY;
		newRot += 300.0f / float(segments);
	}

	DrawLineRGB((int) dx,(int) dy,(int) startX,(int) startY, 255,255,255);
}
