/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
#ifndef __GAMECLASS_H__
#define __GAMECLASS_H__

#include "Shadow.h"
#include "RNPlatform/Inc/SysTime.h"

//-----------------------------------------------------------------------------
// External definitions and prototypes
//-----------------------------------------------------------------------------
inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }

struct VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	FLOAT	   tu, tv;
};

struct SHADOWVERTEX
{
	D3DXVECTOR4 p;
	D3DCOLOR	color;
};

#define D3DFVF_VERTEX	   (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)
#define D3DFVF_SHADOWVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

extern char *gCommandLine;

//-----------------------------------------------------------------------------
// Name: class CMyD3DApplication
// Desc: Application class. The base class (CD3DApplication) provides the 
//	   generic functionality needed in all Direct3D samples. CMyD3DApplication 
//	   adds functionality specific to this sample program.
//-----------------------------------------------------------------------------
class CMyD3DApplication : public CD3DApplication
{
private:
	CD3DFont*	 m_pFont;
	CD3DArcBall   m_ArcBall;

	CD3DMesh*	 m_pAirplane;

public:
	Plane *mPlayerConrolledPlane;
private:
	Camera *mPlayerConrolledCamera;

	CD3DMesh*	 m_pTerrainObject;
	ShadowVolume* m_pShadowVolume;

	D3DXMATRIX	m_matObjectMatrix;
	D3DXMATRIX	m_matTerrainMatrix;

	LPDIRECT3DVERTEXBUFFER8 m_pBigSquareVB;
	HRESULT DrawShadow();
	HRESULT RenderShadow();

	HRESULT ConfirmDevice( D3DCAPS8*, DWORD, D3DFORMAT );

protected:
	HRESULT OneTimeSceneInit();
	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
	HRESULT Render();
	HRESULT FrameMove();
	HRESULT FinalCleanup();


	HRESULT RenderModels();

public:
	LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	CMyD3DApplication();
	void DoGameStuff(void);
	static CMyD3DApplication *GetCMyD3DApplication(void);

private:
	RNReplicaNet::SysTimeType mLastFrameTime;
	static CMyD3DApplication *mSingleton;
};

extern CD3DMesh*	 gpAirplaneMesh;
extern CD3DMesh*	 gpProjectileMesh;
extern CD3DMesh*	 gpEnemyMesh;

#endif
