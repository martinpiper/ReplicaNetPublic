#include "s3e.h"
#include <math.h>
#include "main.h"
#include "Player.h"

Player::Player() : mPlayerColour(-1) , mPosX(50.0f) , mPosY(50.0f) , mRotation(0.0f)
{
	mPosX = (float) s3eSurfaceGetInt(S3E_SURFACE_WIDTH) / 2.0f;
	mPosY = (float) s3eSurfaceGetInt(S3E_SURFACE_HEIGHT) / 2.0f;

	mTargetPosX = mPosX;
	mTargetPosY = mPosY;

	time_t theTime;
	theTime = time(&theTime);
	srand(theTime);
	// Pick a bright random colour
	do
	{
		mPlayerColour = rand();
	} while ( ((mPlayerColour & 31) < 15) || (((mPlayerColour >> 5) & 31) < 15) || (((mPlayerColour >> 10) & 31) < 15) );
}

Player::~Player()
{
}

void Player::PostObjectCreate(void)
{
}

void Player::Tick(float timeDelta)
{
//	printf("Player::Tick(%f)\n",timeDelta);
//	printf("mPlayerColour = %d\n",mPlayerColour);

	if (IsMaster())
	{
		if (s3ePointerGetState(S3E_POINTER_BUTTON_SELECT) & S3E_POINTER_STATE_PRESSED)
		{
			mTargetPosX = (float)s3ePointerGetX();
			mTargetPosY = (float)s3ePointerGetY();
		}

		float deltaX;
		deltaX = mTargetPosX - mPosX;

		float deltaY;
		deltaY = mTargetPosY - mPosY;

		if ((fabs(deltaX) > 1.0f) || (fabs(deltaY) > 1.0f))
		{
			if (fabs(deltaY) < 0.1f)
			{
				deltaY = 0.001f;
			}
			float targetRotation = atanf(-deltaX / deltaY);
			targetRotation = (360.0f * targetRotation) / (2.0f * (float) M_PI);

			if (deltaY > 0.0f)
			{
				targetRotation = targetRotation + 180.0f;
			}

			// Now handle the rotations, with checks for swings around 0, 360 and 180 degrees.
			while (targetRotation < 0.0f)
			{
				targetRotation += 360.0f;
			}
			while (targetRotation >= 360.0f)
			{
				targetRotation -= 360.0f;
			}

			float rotD = targetRotation - mRotation;
			if (rotD > 180.0f)
			{
				rotD -= 180.0f;
				rotD = -rotD;
			}
			if (rotD < -180.0f)
			{
				rotD += 180.0f;
				rotD = -rotD;
			}

			if (fabs(rotD) > 1.0f)
			{
				float swing = rotD;

				if (rotD > 45.0f)
				{
					swing = 45.0f;
				}
				else if (rotD < -45.0f)
				{
					swing = -45.0f;
				}

				mRotation += 4.0f * timeDelta * swing;
				GiveDeltaHint(mRotation, 4.0f * swing);
			}
			else
			{
				GiveDeltaHint(mRotation, 0.0f);
				ContinuityBreak(mRotation,RNReplicaNet::DataBlock::kStop);
			}

			while (mRotation < 0.0f)
			{
				mRotation += 360.0f;
			}
			while (mRotation >= 360.0f)
			{
				mRotation -= 360.0f;
			}

			float distance = sqrtf((deltaX*deltaX) + (deltaY*deltaY));
			distance = distance * 0.5f;

			deltaX = sinf(DEG2RAD(mRotation));
			deltaY = -cosf(DEG2RAD(mRotation));
			float delta = deltaX * distance;
			mPosX += delta * timeDelta;
			GiveDeltaHint(mPosX,delta);
			delta = deltaY * distance;
			mPosY += delta * timeDelta;
			GiveDeltaHint(mPosY,delta);
		}
		else	//< if ((fabs(deltaX) > 1.0f) || (fabs(deltaY) > 1.0f))
		{
			// No movement hint
			GiveDeltaHint(mPosX,0.0f);
			ContinuityBreak(mPosX,RNReplicaNet::DataBlock::kStop);
			GiveDeltaHint(mPosY,0.0f);
			ContinuityBreak(mPosY,RNReplicaNet::DataBlock::kStop);
			GiveDeltaHint(mRotation, 0.0f);
			ContinuityBreak(mRotation,RNReplicaNet::DataBlock::kStop);
		}
	}
}

void Player::Render(void)
{
	float dx,dy;
	float newX,newY;
	float newRot;
	float len;

	newRot = mRotation + 135.0f;
	dx = sinf(DEG2RAD(newRot));
	dy = cosf(DEG2RAD(newRot));
	len = 10.0f;
	newX = (float) (mPosX + (dx * len));
	newY = (float) (mPosY - (dy * len));
	DrawLine((int) mPosX,(int) mPosY,(int) newX,(int) newY, mPlayerColour);

	newRot = mRotation;
	dx = sinf(DEG2RAD(newRot));
	dy = cosf(DEG2RAD(newRot));
	len = 15.0f;
	DrawLine((int) (mPosX + (dx * len)),(int) (mPosY - (dy * len)),(int) newX,(int) newY, mPlayerColour);


	newRot = mRotation + 225.0f;
	dx = sinf(DEG2RAD(newRot));
	dy = cosf(DEG2RAD(newRot));
	len = 10.0f;
	newX = (float) (mPosX + (dx * len));
	newY = (float) (mPosY - (dy * len));
	DrawLine((int) mPosX,(int) mPosY,(int) newX,(int) newY, mPlayerColour);

	newRot = mRotation;
	dx = sinf(DEG2RAD(newRot));
	dy = cosf(DEG2RAD(newRot));
	len = 15.0f;
	DrawLine((int) (mPosX + (dx * len)),(int) (mPosY - (dy * len)),(int) newX,(int) newY, mPlayerColour);
}
