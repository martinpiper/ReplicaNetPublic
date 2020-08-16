/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
#define STRICT
#include <math.h>
#include <stdio.h>

#include "Network.h"

#include "Plane.h"
#include "Camera.h"

#include "GameClass.h"

#include "RNReplicaNet\Inc\ReplicaNet.h"
#include "RNXPURL\Inc\XPURL.h"
#include "RNPlatform\Inc\PlatformHeap.h"


CD3DMesh*	 gpAirplaneMesh;
char *gCommandLine;

HINSTANCE ghInst;
HINSTANCE gAppInstance = 0;
HWND ghWnd = 0; 


//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Entry point to the program. Initializes everything, and goes into a
//	   message-processing loop. Idle time is used to render the scene.
//-----------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR commandline, INT )
{
	gAppInstance = ghInst = hInst;
	gCommandLine = commandline;

	CMyD3DApplication d3dApp;

	if( FAILED( d3dApp.Create( hInst ) ) )
		return 0;

	d3dApp.DoGameStuff();

	return d3dApp.Run();
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT ShadowVolume::Render( LPDIRECT3DDEVICE8 pd3dDevice )
{
	pd3dDevice->SetVertexShader( D3DFVF_XYZ );

	return pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, m_dwNumVertices/3,
										m_pVertices, sizeof(D3DXVECTOR3) );
}




//-----------------------------------------------------------------------------
// Name: AddEdge()
// Desc: Adds an edge to a list of silhouette edges of a shadow volume.
//-----------------------------------------------------------------------------
VOID AddEdge( WORD* pEdges, DWORD& dwNumEdges, WORD v0, WORD v1 )
{
	// Remove interior edges (which appear in the list twice)
	DWORD i;
	for( i=0; i < dwNumEdges; i++ )
	{
		if( ( pEdges[2*i+0] == v0 && pEdges[2*i+1] == v1 ) ||
			( pEdges[2*i+0] == v1 && pEdges[2*i+1] == v0 ) )
		{
			if( dwNumEdges > 1 )
			{
				pEdges[2*i+0] = pEdges[2*(dwNumEdges-1)+0];
				pEdges[2*i+1] = pEdges[2*(dwNumEdges-1)+1];
			}
			dwNumEdges--;
			return;
		}
	}

	pEdges[2*dwNumEdges+0] = v0;
	pEdges[2*dwNumEdges+1] = v1;
	dwNumEdges++;
}




//-----------------------------------------------------------------------------
// Name: BuildFromMesh()
// Desc: Takes a mesh as input, and uses it to build a shadowvolume. The
//	   technique used considers each triangle of the mesh, and adds it's
//	   edges to a temporary list. The edge list is maintained, such that
//	   only silhouette edges are kept. Finally, the silhouette edges are
//	   extruded to make the shadow volume vertex list.
//-----------------------------------------------------------------------------
HRESULT ShadowVolume::BuildFromMesh( LPD3DXMESH pMesh, D3DXVECTOR3 vLight )
{
	// Note: the MESHVERTEX format depends on the FVF of the mesh
	struct MESHVERTEX { D3DXVECTOR3 p, n; FLOAT tu, tv; };
	DWORD i,dwFVF = pMesh->GetFVF();

	MESHVERTEX* pVertices;
	WORD*	   pIndices;

	// Lock the geometry buffers
	pMesh->LockVertexBuffer( 0L, (BYTE**)&pVertices );
	pMesh->LockIndexBuffer( 0L, (BYTE**)&pIndices );
	DWORD dwNumVertices = pMesh->GetNumVertices();
	DWORD dwNumFaces	= pMesh->GetNumFaces();

	// Allocate a temporary edge list
	WORD* pEdges = new WORD[dwNumFaces*6];
	DWORD dwNumEdges = 0;

	// For each face
	for( i=0; i<dwNumFaces; i++ )
	{
		WORD wFace0 = pIndices[3*i+0];
		WORD wFace1 = pIndices[3*i+1];
		WORD wFace2 = pIndices[3*i+2];

		D3DXVECTOR3 v0 = pVertices[wFace0].p;
		D3DXVECTOR3 v1 = pVertices[wFace1].p;
		D3DXVECTOR3 v2 = pVertices[wFace2].p;

		// Transform vertices or transform light?
		D3DXVECTOR3 vNormal;
		D3DXVec3Cross( &vNormal, &(v2-v1), &(v1-v0) );

		if( D3DXVec3Dot( &vNormal, &vLight ) >= 0.0f )
		{
			AddEdge( pEdges, dwNumEdges, wFace0, wFace1 );
			AddEdge( pEdges, dwNumEdges, wFace1, wFace2 );
			AddEdge( pEdges, dwNumEdges, wFace2, wFace0 );
		}
	}

	for( i=0; i<dwNumEdges; i++ )
	{
		D3DXVECTOR3 v1 = pVertices[pEdges[2*i+0]].p;
		D3DXVECTOR3 v2 = pVertices[pEdges[2*i+1]].p;
		D3DXVECTOR3 v3 = v1 - vLight*10;
		D3DXVECTOR3 v4 = v2 - vLight*10;

		// Add a quad (two triangles) to the vertex list
		m_pVertices[m_dwNumVertices++] = v1;
		m_pVertices[m_dwNumVertices++] = v2;
		m_pVertices[m_dwNumVertices++] = v3;

		m_pVertices[m_dwNumVertices++] = v2;
		m_pVertices[m_dwNumVertices++] = v4;
		m_pVertices[m_dwNumVertices++] = v3;
	}
	// Delete the temporary edge list
	delete pEdges;

	// Unlock the geometry buffers
	pMesh->UnlockVertexBuffer();
	pMesh->UnlockIndexBuffer();

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CMyD3DApplication()
// Desc: Application constructor. Sets attributes for the app.
//-----------------------------------------------------------------------------
CMyD3DApplication::CMyD3DApplication()
{
	m_strWindowTitle	= _T("Example1: Planes flying around");
	m_bUseDepthBuffer   = TRUE;
//	m_bUseDepthBuffer   = FALSE;
	m_dwMinDepthBits	= 16;
	m_dwMinStencilBits  = 4;
	m_dwMinStencilBits  = 0;
	m_bShowCursorWhenFullscreen = TRUE;

	m_pFont		   = new CD3DFont( _T("Arial"), 8, 0 /* No bold font flag */ );
	m_pAirplane	   = new CD3DMesh();
	m_pTerrainObject  = new CD3DMesh();
	m_pShadowVolume   = NULL;
	m_pBigSquareVB	= NULL;

	gpAirplaneMesh = m_pAirplane;

	mPlayerConrolledPlane = 0;
	mPlayerConrolledCamera = 0;
}


//-----------------------------------------------------------------------------
// Name: OneTimeSceneInit()
// Desc: Called during initial app startup, this function performs all the
//	   permanent initialization.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::OneTimeSceneInit()
{
	// Construct a shadow volume object;
//	m_pShadowVolume = new ShadowVolume();

	// Set cursor to indicate that user can move the object with the mouse
#ifdef _WIN64
	SetClassLongPtr( m_hWnd, GCLP_HCURSOR, (LONG_PTR)LoadCursor( NULL, IDC_SIZEALL ) );
#else
	SetClassLong( m_hWnd, GCL_HCURSOR, (LONG)LoadCursor( NULL, IDC_SIZEALL ) );
#endif
	return S_OK;
}



//-----------------------------------------------------------------------------
// Name: RenderShadow()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::RenderShadow()
{
	// Disable z-buffer writes (note: z-testing still occurs), and enable the
	// stencil-buffer
	m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,  FALSE );
	m_pd3dDevice->SetRenderState( D3DRS_STENCILENABLE, TRUE );

	// Dont bother with interpolating color
	m_pd3dDevice->SetRenderState( D3DRS_SHADEMODE,	 D3DSHADE_FLAT );

	// Set up stencil compare fuction, reference value, and masks.
	// Stencil test passes if ((ref & mask) cmpfn (stencil & mask)) is true.
	// Note: since we set up the stencil-test to always pass, the STENCILFAIL
	// renderstate is really not needed.
	m_pd3dDevice->SetRenderState( D3DRS_STENCILFUNC,  D3DCMP_ALWAYS );
	m_pd3dDevice->SetRenderState( D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP );
	m_pd3dDevice->SetRenderState( D3DRS_STENCILFAIL,  D3DSTENCILOP_KEEP );

	// If ztest passes, inc/decrement stencil buffer value
	m_pd3dDevice->SetRenderState( D3DRS_STENCILREF,	   0x1 );
	m_pd3dDevice->SetRenderState( D3DRS_STENCILMASK,	  0xffffffff );
	m_pd3dDevice->SetRenderState( D3DRS_STENCILWRITEMASK, 0xffffffff );
	m_pd3dDevice->SetRenderState( D3DRS_STENCILPASS,	  D3DSTENCILOP_INCR );

	// Make sure that no pixels get drawn to the frame buffer
	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ZERO );
	m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	// Draw front-side of shadow volume in stencil/z only
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matObjectMatrix );
	m_pShadowVolume->Render( m_pd3dDevice );

	// Now reverse cull order so back sides of shadow volume are written.
	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,   D3DCULL_CW );

	// Decrement stencil buffer value
	m_pd3dDevice->SetRenderState( D3DRS_STENCILPASS, D3DSTENCILOP_DECR );

	// Draw back-side of shadow volume in stencil/z only
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matObjectMatrix );
	m_pShadowVolume->Render( m_pd3dDevice );

	// Restore render states
	m_pd3dDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,  D3DCULL_CCW );
	m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,	 TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_STENCILENABLE,	FALSE );
	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: DrawShadow()
// Desc: Draws a big gray polygon over scene according to the mask in the
//	   stencil buffer. (Any pixel with stencil==1 is in the shadow.)
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::DrawShadow()
{
	// Set renderstates (disable z-buffering, enable stencil, disable fog, and
	// turn on alphablending)
	m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,		  FALSE );
	m_pd3dDevice->SetRenderState( D3DRS_STENCILENABLE,	TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,		FALSE );
	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	// Only write where stencil val >= 1 (count indicates # of shadows that
	// overlap that pixel)
	m_pd3dDevice->SetRenderState( D3DRS_STENCILREF,  0x1 );
	m_pd3dDevice->SetRenderState( D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL );
	m_pd3dDevice->SetRenderState( D3DRS_STENCILPASS, D3DSTENCILOP_KEEP );

	// Draw a big, gray square
	m_pd3dDevice->SetVertexShader( D3DFVF_SHADOWVERTEX );
	m_pd3dDevice->SetStreamSource( 0, m_pBigSquareVB, sizeof(SHADOWVERTEX) );
	m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

	// Restore render states
	m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,		  TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_STENCILENABLE,	FALSE );
	m_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,		TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

	return S_OK;
}






//-----------------------------------------------------------------------------
// Name: InitDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::InitDeviceObjects()
{
	// Initialize the font's internal textures
	m_pFont->InitDeviceObjects( m_pd3dDevice );

	// Load an object to cast the shadow
	if( FAILED( m_pAirplane->Create( m_pd3dDevice, _T("airplane 2.x") ) ) )
		return D3DAPPERR_MEDIANOTFOUND;

	// Load some terrain
	if( FAILED( m_pTerrainObject->Create( m_pd3dDevice, _T("SeaFloor.x") ) ) )
		return D3DAPPERR_MEDIANOTFOUND;

	// Set a reasonable vertex type
	m_pAirplane->SetFVF( m_pd3dDevice, D3DFVF_VERTEX );
	m_pTerrainObject->SetFVF( m_pd3dDevice, D3DFVF_VERTEX );

	// Tweak the terrain vertices
	{
		LPDIRECT3DVERTEXBUFFER8 pVB;
		VERTEX* pVertices;
		DWORD   dwNumVertices = m_pTerrainObject->GetSysMemMesh()->GetNumVertices();

		// Lock the vertex buffer to access the terrain geometry
		m_pTerrainObject->GetSysMemMesh()->GetVertexBuffer( &pVB );
		pVB->Lock( 0, 0, (BYTE**)&pVertices, 0 );

		// Add some more bumpiness to the terrain object
		DWORD i;
		for( i=0; i<dwNumVertices; i++ )
		{
			pVertices[i].p.y  += 1*(rand()/(FLOAT)RAND_MAX);
			pVertices[i].p.y  += 2*(rand()/(FLOAT)RAND_MAX);
			pVertices[i].p.y  += 1*(rand()/(FLOAT)RAND_MAX);
		}

		// Release the vertex buffer
		pVB->Unlock();
		pVB->Release();
	}

	// Create a big square for rendering the stencilbuffer contents
//	if( FAILED( m_pd3dDevice->CreateVertexBuffer( 4*sizeof(SHADOWVERTEX),
//									   D3DUSAGE_WRITEONLY, D3DFVF_SHADOWVERTEX,
//									   D3DPOOL_MANAGED, &m_pBigSquareVB ) ) )
//		return E_FAIL;

	ghWnd = m_hWnd;

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: RestoreDeviceObjects()
// Desc: Restore device-memory objects and state after a device is created or
//	   resized.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::RestoreDeviceObjects()
{
	m_pFont->RestoreDeviceObjects();

	// Initialize the vertex buffers for the file-based objects
	m_pAirplane->RestoreDeviceObjects( m_pd3dDevice );
	m_pTerrainObject->RestoreDeviceObjects( m_pd3dDevice );

	// Create and set up the shine materials w/ textures
	D3DMATERIAL8 mtrl;
	D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 1.0f );
	m_pd3dDevice->SetMaterial( &mtrl );

	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );

	m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );

	// Set the transform matrices
	D3DXVECTOR3 vEyePt	= D3DXVECTOR3( 0.0f, 10.0f, -20.0f );
	D3DXVECTOR3 vLookatPt = D3DXVECTOR3( 0.0f,  0.0f,   0.0f  );
	D3DXVECTOR3 vUpVec	= D3DXVECTOR3( 0.0f,  1.0f,   0.0f  );
	D3DXMATRIX matWorld, matView, matProj;

	D3DXMatrixIdentity( &matWorld );
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	FLOAT fAspect = m_d3dsdBackBuffer.Width / (FLOAT)m_d3dsdBackBuffer.Height;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, fAspect, 1.0f, 100.0f );

	m_pd3dDevice->SetTransform( D3DTS_WORLD,	  &matWorld );
	m_pd3dDevice->SetTransform( D3DTS_VIEW,	   &matView );
	m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	// Turn on fog
	FLOAT fFogStart =  30.0f;
	FLOAT fFogEnd   =  100.0f;
	m_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,	  TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_FOGCOLOR,	   0xff0000ff );
	m_pd3dDevice->SetRenderState( D3DRS_FOGTABLEMODE,   D3DFOG_NONE );
	m_pd3dDevice->SetRenderState( D3DRS_FOGVERTEXMODE,  D3DFOG_LINEAR );
	m_pd3dDevice->SetRenderState( D3DRS_RANGEFOGENABLE, FALSE );
	m_pd3dDevice->SetRenderState( D3DRS_FOGSTART,	   FtoDW(fFogStart) );
	m_pd3dDevice->SetRenderState( D3DRS_FOGEND,		 FtoDW(fFogEnd) );

	// Set the ArcBall parameters
	m_ArcBall.SetWindow( m_d3dsdBackBuffer.Width, m_d3dsdBackBuffer.Height, 2.0f );
	m_ArcBall.SetRadius( 5.0f );

	m_pd3dDevice->LightEnable( 0, TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_AMBIENT,  0x00303030 );

	// Set the size of the big square shadow
	SHADOWVERTEX* v = 0;
	FLOAT sx = (FLOAT)m_d3dsdBackBuffer.Width;
	FLOAT sy = (FLOAT)m_d3dsdBackBuffer.Height;
#if 0
	m_pBigSquareVB->Lock( 0, 0, (BYTE**)&v, 0 );
	v[0].p = D3DXVECTOR4(  0, sy, 0.0f, 1.0f );
	v[1].p = D3DXVECTOR4(  0,  0, 0.0f, 1.0f );
	v[2].p = D3DXVECTOR4( sx, sy, 0.0f, 1.0f );
	v[3].p = D3DXVECTOR4( sx,  0, 0.0f, 1.0f );
	v[0].color = 0x7f000000;
	v[1].color = 0x7f000000;
	v[2].color = 0x7f000000;
	v[3].color = 0x7f000000;
	m_pBigSquareVB->Unlock();
#endif

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects()
// Desc: Called when the device-dependent objects are about to be lost.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::InvalidateDeviceObjects()
{
	m_pFont->InvalidateDeviceObjects();
	m_pAirplane->InvalidateDeviceObjects();
	m_pTerrainObject->InvalidateDeviceObjects();
	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: Called when the app is exiting, or the device is being changed,
//	   this function deletes any device dependent objects.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::DeleteDeviceObjects()
{
	m_pFont->DeleteDeviceObjects();
	m_pAirplane->Destroy();
	m_pTerrainObject->Destroy();

	SAFE_RELEASE( m_pBigSquareVB );

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: FinalCleanup()
// Desc: Called before the app exits, this function gives the app the chance
//	   to cleanup after itself.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::FinalCleanup()
{
	// This demonstrates the nice way to disconnect from a network session
	// None of this is strictly required by ReplicaNet because using delete on the ReplicaNet class also disconnects,
	// however it does perform a very graceful disconnect.

	// First disconnect
	gNetwork->Disconnect();

	// After deleteing ReplicaNet it is not a good idea to use ReplicaNet functions on ReplicaObject derived classes
	delete gNetwork;
	gNetwork = 0;

	// We can shutdown the network here. Not strictly necessary but it demonstrates the graceful shutdown procedure.
	RNReplicaNet::XPURL::ShutdownNetwork();

	// This force frees all remaining, one time allocated memory, from the ReplicaNet library. This memory usually exists
	// for the lifetime of the application.
	RNReplicaNet::PlatformHeap::ForceFree();

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: ConfirmDevice()
// Desc: Called during device intialization, this code checks the device
//	   for some minimum set of capabilities
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::ConfirmDevice( D3DCAPS8* pCaps, DWORD dwBehavior,
										  D3DFORMAT Format )
{
	// Make sure device supports point lights
	if( (dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING ) ||
		(dwBehavior & D3DCREATE_MIXED_VERTEXPROCESSING ) )
	{
		if( 0 == ( pCaps->VertexProcessingCaps & D3DVTXPCAPS_POSITIONALLIGHTS ) )
			return E_FAIL;
	}

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: Message proc function to handle key and menu input
//-----------------------------------------------------------------------------
LRESULT CMyD3DApplication::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam,
									LPARAM lParam )
{
	// Pass mouse messages to the ArcBall so it can build internal matrices
	m_ArcBall.HandleMouseMessages( hWnd, uMsg, wParam, lParam );

	// Trap the context menu
	if( WM_CONTEXTMENU == uMsg )
		return 0;

	return CD3DApplication::MsgProc( hWnd, uMsg, wParam, lParam );
}



