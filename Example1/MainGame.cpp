/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
/*
This file contains the main parts of the game that deal with ReplicaNet objects.
There are three mains parts. The init routine (DoGameStuff), the frame tick (FrameMove) and the render (Render)
*/
#define STRICT
#include <math.h>
#include <stdio.h>
#include <assert.h>
#define DIRECTINPUT_VERSION 0x0800
#include <DInput.h>
#include "DIInterface.h"

#include "Network.h"

#include "Plane.h"
#include "Camera.h"

#include "GameClass.h"

#include "RNXPSockets/Inc/XPSocket.h"
#include "RNXPURL/Inc/NetworkEmulation.h"
#include "BandwidthGraph.h"

using namespace RNReplicaNet;

/**
 * This initialises the game state.
 */
void CMyD3DApplication::DoGameStuff(void)
{
	gDIHandler = new TDIHandler();
	gDIHandler->Initialise();

	/* This starts the ReplicaNet session and allows the user to create or join a game */
	Network_Init(gCommandLine);

	/* Allocate our plane */
	mPlayerConrolledPlane = new Plane();
	/* Publish our plane */
	mPlayerConrolledPlane->Publish();

	/* Allocate a camera */
	mPlayerConrolledCamera = new Camera();
	/* Publish our camera */
	mPlayerConrolledCamera->Publish();
	/* Set the observer for this game session to be this camera object */
	/* This is used by the prediction algorithm so calculate the distance to other objects in the scene */
	/* The greater the distance then the less bandwidth the object can use. Each object's parameters for prediction */
	/* are defined in the ROL file for the object */
	gNetwork->SetObserver(mPlayerConrolledCamera);
}

SysTimeType repnettime;
SysTimeType framemovetime;
SysTimeType rendertime;

static SysTime sLastFrameTick;
/**
 * Called once per frame, this call is the entry point for animating
 */
HRESULT CMyD3DApplication::FrameMove()
{
	// If the frame rate is fast then we can afford to do a slightly longer Sleep() to avoid this demo running at >100fps
	while( sLastFrameTick.FloatTime() < (1.0f/100.0f) )
	{
		Sleep(1);
	}
	sLastFrameTick.Reset();

	// For good thread management we give the OS time to do a context switch as this app is quite a heavy graphics user
	Sleep(0);

	gDIHandler->ReadAllInputs();

	// Poll ReplicaNet because we have enabled the manual Poll() calling structure
	repnettime = gNetwork->GetLocalTime();
	gNetwork->Poll();
	repnettime = gNetwork->GetLocalTime() - repnettime;

	framemovetime = gNetwork->GetLocalTime();
	// Tell ReplicaNet that we don't want to have objects allocated or freed while scanning the object list
	gNetwork->LockObjects();
	for (int i=0;i<(int)gGameObjects.size();i++)
	{
		// Call each object's PollIt() function
		gGameObjects[i]->PollIt();
		if (gGameObjects[i]->mReplica->IsMaster() && (gGameObjects[i]->mReplica->GetClassID() == Plane::StaticGetClassID()))
		{
			if (SysKeyDown(DIK_Y))
			{
				// This type cast is safe because GetClassID() matches the Plane::StaticGetClassID() 
				((Plane*)gGameObjects[i])->NetworkCallAMemberFunction(D3DX_PI / 2.0f);
			}
			else
			{
				((Plane*)gGameObjects[i])->NetworkCallAMemberFunction(0.0f);
			}
			if (SysKeyDownOnce(DIK_U))
			{
				((Plane*)gGameObjects[i])->Unpublish();
			}
		}

		// If the Delete() function got called then delete the object properly
		if (gGameObjects[i]->mDeleteMe)
		{
			delete gGameObjects[i];
			i--;
		}
	}
	// Tell ReplicaNet that the object list is no longer being used
	gNetwork->UnLockObjects();

	// Setup emulated network values depending on keyboard input
	if (SysKeyDown(DIK_Q))
	{
		RNReplicaNet::NetworkEmulation::SetAverageLatency(max(min(RNReplicaNet::NetworkEmulation::GetAverageLatency() + 0.01f , 1.0f) , 0.0f));
	}
	if (SysKeyDown(DIK_A))
	{
		RNReplicaNet::NetworkEmulation::SetAverageLatency(max(min(RNReplicaNet::NetworkEmulation::GetAverageLatency() - 0.01f , 1.0f) , 0.0f));
	}
	if (SysKeyDown(DIK_W))
	{
		RNReplicaNet::NetworkEmulation::SetPacketLoss(max(min(RNReplicaNet::NetworkEmulation::GetPacketLoss() + 1.0f , 100.0f) , 0.0f));
	}
	if (SysKeyDown(DIK_S))
	{
		RNReplicaNet::NetworkEmulation::SetPacketLoss(max(min(RNReplicaNet::NetworkEmulation::GetPacketLoss() - 1.0f , 100.0f) , 0.0f));
	}
//	RNReplicaNet::NetworkEmulation::SetJitter(0.030f);
//	RNReplicaNet::NetworkEmulation::Set(11.11f);	// 1 out of 9 packets lost


	// Now setup the camera position and lights for this frame

	// Setup the world spin matrix
	D3DXMatrixTranslation( &m_matTerrainMatrix, 0.0f, 0.0f, 0.0f );

	D3DXVECTOR4 campt = D3DXVECTOR4(0,5,15,0);
	if (mPlayerConrolledPlane)
	{
		D3DXVec4Transform(&campt,&campt,&mPlayerConrolledPlane->m_matObjectMatrix);
		campt += mPlayerConrolledPlane->GetPosition();
	}


	framemovetime = gNetwork->GetLocalTime() - framemovetime;

	return S_OK;
}

/**
 * Called once per frame, the call is the entry point for 3d
 *	   rendering. This function sets up render states, clears the
 *	   viewport, and renders the scene.
 */
HRESULT CMyD3DApplication::Render()
{
	rendertime = gNetwork->GetLocalTime();

	// Move the light
	FLOAT Lx =   5;
	FLOAT Ly =   5;
	FLOAT Lz =  -5;
	D3DLIGHT8 light;
	D3DUtil_InitLight( light, D3DLIGHT_POINT, Lx, Ly, Lz );
	light.Attenuation0 = 0.9f;
	light.Attenuation1 = 0.0f;
	m_pd3dDevice->SetLight( 0, &light );

	// Set the transform matrices
	D3DXVECTOR3 vEyePt;//	= D3DXVECTOR3( campt.x, campt.y, campt.z );
	D3DXVECTOR3 vLookatPt;// = D3DXVECTOR3( mPlayerConrolledPlane->GetPosition().x,  mPlayerConrolledPlane->GetPosition().y,   mPlayerConrolledPlane->GetPosition().z  );

	//	vEyePt	= D3DXVECTOR3( 0, 20.0f, 30.0f );
	//	vLookatPt = D3DXVECTOR3( 0,  0,   0  );

	vEyePt	= D3DXVECTOR3( 0, 30.0f, 40.0f );
	vLookatPt = D3DXVECTOR3( -30.0f,  0,   0  );

	D3DXVECTOR3 vUpVec	= D3DXVECTOR3( 0.0f,  1.0f,   0.0f  );
	D3DXMATRIX matWorld, matView, matProj;

	D3DXMatrixIdentity( &matWorld );
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	FLOAT fAspect = m_d3dsdBackBuffer.Width / (FLOAT)m_d3dsdBackBuffer.Height;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, fAspect, 1.0f, 100.0f );

	m_pd3dDevice->SetTransform( D3DTS_WORLD,	  &matWorld );
	m_pd3dDevice->SetTransform( D3DTS_VIEW,	   &matView );
	m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	mPlayerConrolledCamera->mPosition.x = vEyePt.x;
	mPlayerConrolledCamera->mPosition.y = vEyePt.y;
	mPlayerConrolledCamera->mPosition.z = vEyePt.z;


	HRESULT ret = m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,0xff0000ff, 1.0f, 0L );

	if (ret == D3DERR_INVALIDCALL)
	{
		OutputDebugString("Invalid call\n");
	}

	// Begin the scene
	if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
	{
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matTerrainMatrix );
		m_pTerrainObject->Render( m_pd3dDevice );

		/* Again lock our game objects list while we render */
		gNetwork->LockObjects();

		char buffer[256];
		float tY = 60.0f;
		for (int i=0;i<(int)gGameObjects.size();i++)
		{
			ReplicaObject *robject;
			// Get the derived class object pointer that contains a base class type of ReplicaObject from the GameObject class
			robject = gGameObjects[i]->mReplica;
			// If there is no object pointer then ignore this object for rendering
			if (!robject)
			{
				continue;
			}
			// Compare the class id of the ReplicaObject with the class ID of the ReplicaObject Plane object
			// If they are the same then we can assume the type cast to a Plane class is valid
			if (robject->GetClassID() == _MAKE_RO(Plane)::StaticGetClassID())
			{
				// Perform the type cast
				Plane *theplane = (Plane *) gGameObjects[i];

				// Calculate the real matrix for the object
				D3DXMatrixRotationYawPitchRoll(&theplane->m_matObjectMatrix,theplane->GetRotation().y,theplane->GetRotation().x + theplane->mFunctionXRot,theplane->GetRotation().z);
				if (theplane->IsMaster())
				{
					D3DXMATRIX scale;
					D3DXMatrixScaling(&scale,2.0f,2.0f,2.0f);
					D3DXMatrixMultiply( &theplane->m_matObjectMatrix, &theplane->m_matObjectMatrix, &scale);
				}

				D3DXMATRIX transmat;
				D3DXMatrixTranslation(&transmat,theplane->GetPosition().x,theplane->GetPosition().y,theplane->GetPosition().z);
				D3DXMatrixMultiply( &theplane->m_matObjectMatrix, &theplane->m_matObjectMatrix, &transmat);
				m_matObjectMatrix = theplane->m_matObjectMatrix;
				m_pd3dDevice->SetTransform( D3DTS_WORLD, &theplane->m_matObjectMatrix );
				theplane->mMesh->Render( m_pd3dDevice );

				m_pShadowVolume = 0;

				if (gNetwork->GetObserver())
				{
					sprintf(buffer,"Plane %d:%d im%d distance to observer %f",theplane->GetSessionID(),theplane->GetUniqueID(),(int)theplane->IsMaster(),theplane->CalculateDistanceToObject(gNetwork->GetObserver()));
					m_pFont->DrawText( 2, tY+=10.0f, D3DCOLOR_ARGB(255,255,255,0), buffer );
				}
			}
		}

		/* Now unlock our game objects list since we don't access it from now on */
		gNetwork->UnLockObjects();

		rendertime = gNetwork->GetLocalTime() - rendertime;

		// Output statistics
		m_pFont->DrawText( 2,  0, D3DCOLOR_ARGB(255,255,255,0), m_strFrameStats );
		m_pFont->DrawText( 2, 10, D3DCOLOR_ARGB(255,255,255,0), m_strDeviceStats );

		if (gNetwork->IsMaster())
		{
			sprintf(buffer,"MasterSession ID %d Send bytes/sec %d Recv %d bytes/sec",gNetwork->GetSessionID(),(int) gNetwork->GetNetworkSendRate(),(int) gNetwork->GetNetworkReceiveRate());
		}
		else
		{
			sprintf(buffer,"Session ID %d Send bytes/sec %d Recv %d bytes/sec",gNetwork->GetSessionID(),(int) gNetwork->GetNetworkSendRate(),(int) gNetwork->GetNetworkReceiveRate());
		}
		m_pFont->DrawText( 2, 20, D3DCOLOR_ARGB(255,255,255,0), buffer );
		sprintf(buffer,"Packets resent %d",gNetwork->GetNetworkPacketsLost());
		m_pFont->DrawText( 2, 30, D3DCOLOR_ARGB(255,255,255,0), buffer );
		sprintf(buffer,"URL: %s",gNetwork->SessionExportURL().c_str());
		m_pFont->DrawText( 2, 40, D3DCOLOR_ARGB(255,255,255,0), buffer );
		sprintf(buffer,"ReplicaNet %.2fms Frame move %.2fms Render %.2fms",repnettime*1000.0f,framemovetime*1000.0f,rendertime*1000.0f);
		m_pFont->DrawText( 2, 50, D3DCOLOR_ARGB(255,255,255,0), buffer );

		sprintf(buffer,"Emulated latency %.2fs packet loss %d%%",RNReplicaNet::NetworkEmulation::GetAverageLatency() , (int) RNReplicaNet::NetworkEmulation::GetPacketLoss());
		m_pFont->DrawText( 2, 60, D3DCOLOR_ARGB(255,255,255,0), buffer );
		

		// End the scene.
		m_pd3dDevice->EndScene();
	}

	GraphRender(m_pd3dDevice , m_pFont);

	return S_OK;
}
