/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
/*
See Plane.cpp
*/
#ifndef __PLANE_H__
#define __PLANE_H__

#include "d3dfile.h"

#include "GameObject.h"
#include "Shadow.h"

#include "_RO_Plane.h"
#include <set>

class Plane : _RO_DO_PUBLIC_RO(Plane) , public GameObject
{
public:
	Plane();
	virtual ~Plane();

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

	void SetRotation(D3DXVECTOR4 rotation)
	{
		mRotation = rotation;
	}

	CD3DMesh *mMesh;
	ShadowVolume *mShadow;
	D3DXMATRIX	m_matObjectMatrix;

/* These variables are defined in the ROL file for this object */
private:
_RO_DO_ALLOW_FRIEND_RO(Plane)
	D3DXVECTOR4 mPosition;
	D3DXVECTOR4 mRotation;
};

#endif
