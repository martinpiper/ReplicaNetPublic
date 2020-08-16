/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
/*
See Dolphin.cpp
*/
#ifndef __DOLPHIN_H__
#define __DOLPHIN_H__

#include "d3dfile.h"

#include "GameObject.h"
#include "Shadow.h"

#include "_RO_Dolphin.h"

class Dolphin : _RO_DO_PUBLIC_RO(Dolphin) , public GameObject
{
public:
	Dolphin();
	virtual ~Dolphin();

	void Render(void);

	void PollIt(void);

	D3DXVECTOR4 GetPosition(void)
	{
		return mPosition;
	}

	void SetPosition(D3DXVECTOR4 position)
	{
		mPosition = position;
	}

	D3DXVECTOR4 GetRotation(void)
	{
		return mRotation;
	}

	CD3DMesh *mMesh;
	ShadowVolume *mShadow;
	D3DXMATRIX	m_matObjectMatrix;

/* These variables are defined in the ROL file for this object */
private:
_RO_DO_ALLOW_FRIEND_RO(Dolphin)
	D3DXVECTOR4 mPosition;
	D3DXVECTOR4 mRotation;
};

#endif
