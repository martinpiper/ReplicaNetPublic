/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
#ifndef __SHADOW_H__
#define __SHADOW_H__

#include <D3DX8.h>
#include "D3DApp.h"
#include "D3DUtil.h"
#include "D3DFile.h"
#include "D3DFont.h"
#include "DXUtil.h"

//-----------------------------------------------------------------------------
// Name: struct ShadowVolume
// Desc: A shadow volume object
//-----------------------------------------------------------------------------
class ShadowVolume
{
	D3DXVECTOR3 m_pVertices[32000]; // Vertex data for rendering shadow volume
	DWORD	   m_dwNumVertices;

public:
	VOID	Reset() { m_dwNumVertices = 0L; }
	HRESULT BuildFromMesh( LPD3DXMESH pObject, D3DXVECTOR3 vLight );
	HRESULT Render( LPDIRECT3DDEVICE8 pd3dDevice );
};


#endif
