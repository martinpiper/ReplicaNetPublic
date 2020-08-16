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

#include "Network.h"

#include "Plane.h"
#include "Camera.h"
#include "Projectile.h"
#include "Enemy.h"

#include "GameClass.h"

#include <assert.h>
#include <D3D8.h>
#define DIRECTINPUT_VERSION 0x0800
#include <DInput.h>
#include "DIInterface.h"

#include "External.h"
#include "GameDatabase.h"

#include "../Example1/BandwidthGraph.h"

using namespace RNReplicaNet;


/**
 * This initialises the game state.
 */
void CMyD3DApplication::DoGameStuff(void)
{
	gDIHandler = new TDIHandler();
	gDIHandler->Initialise();

	/* This starts the ReplicaNet session and allows the user to create or join a game */
	Network::Network_Init(gCommandLine);

	mLastFrameTime = Network::mNetwork->GetLocalTime();

	/*
	All of the game objects inherit from GameObject and a ReplicaObject derived network class.
	Each GameObject class is able to report position and calculate distances by using virtual functions.
	The GameObject manager makes sure that the list of in game objects is maintained and polled.
	It is probably best to step in to Plane() and other objects as they are being constructed to see the various stages that are used.
	*/
	// If we are not a dedicated server and we are not playing back a recording
	if (!Network::mDedicatedServer && !Network::mNetwork->GetSessionPlayback())
	{
		/* Allocate our plane */
		mPlayerConrolledPlane = new Plane();
		mPlayerConrolledPlane->SetPosition(D3DXVECTOR4(10,0,0,0));
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
		Network::mNetwork->SetObserver(mPlayerConrolledCamera);
	}

	// Spawn some enemies if we are not playing back a recording.
	if (!Network::mNetwork->GetSessionPlayback())
	{
		for (int i=0;i<5;i++)
		{
			Enemy::SpawnEnemy();
		}
	}
}

static float firetimeout = 0.0f;

bool autofly = false;

RNReplicaNet::SysTimeType framemovetime;
RNReplicaNet::SysTimeType repnettime;
RNReplicaNet::SysTimeType rendertime;
bool stableyet = false;

/**
 * Called once per frame, this call is the entry point for animating
 */
HRESULT CMyD3DApplication::FrameMove()
{
	RNReplicaNet::SysTimeType ttime;

	framemovetime = Network::mNetwork->GetLocalTime();
	ttime = Network::mNetwork->GetLocalTime();
	GameDatabase::mGameFrameTick = (float)(ttime - mLastFrameTime);

	// For good thread management we give the OS time to do a context switch as this app is quite a heavy graphics user
	Sleep(0);

	// If our session isn't stable yet then just return quickly while we wait for the connection to be made
	if (!stableyet)
	{
		if (Network::mNetwork->IsStable())
		{
			stableyet = true;
			mLastFrameTime = ttime;
		}
		else
		{
			return S_OK;
		}
	}

	mLastFrameTime = ttime;
	if (GameDatabase::mGameFrameTick > 1.0f)
	{
		GameDatabase::mGameFrameTick = 1.0f;
	}

	// Process DataBlock updates before handing any inputs
	repnettime = Network::mNetwork->GetLocalTime();
	Network::mNetwork->ProcessDataBlockUpdate();
	repnettime = Network::mNetwork->GetLocalTime() - repnettime;

	// Lock the object list
	// This tells ReplicaNet that we don't want to have objects allocated or freed while scanning the object list
	Network::mNetwork->LockObjects();
	int i;
	for (i=0;i<(int)GameDatabase::mGameObjects.size();i++)
	{
		// Call each object's PollIt() function
		GameDatabase::mGameObjects[i]->PollIt();
	}
	for (i=0;i<(int)GameDatabase::mGameObjects.size();i++)
	{
		// If the Delete() function got called then delete the object properly
		if (GameDatabase::mGameObjects[i]->GetDelete())
		{
			delete GameDatabase::mGameObjects[i];
			i--;
		}
	}
	// Tell ReplicaNet that the object list is no longer being used and unlock it
	Network::mNetwork->UnLockObjects();

	D3DXMatrixTranslation( &m_matTerrainMatrix, 0.0f, 0.0f, 0.0f );

	// Move the light
	FLOAT Lx =   5;
	FLOAT Ly =   5;
	FLOAT Lz =  -5;
	D3DLIGHT8 light;
	D3DUtil_InitLight( light, D3DLIGHT_POINT, Lx, Ly, Lz );
	light.Attenuation0 = 0.9f;
	light.Attenuation1 = 0.0f;
	m_pd3dDevice->SetLight( 0, &light );

	// Get user input
	gDIHandler->ReadAllInputs();

	// If playing a recording this allows the recording to be played in slow-motion, faster than normal, pause and skip 10 seconds
	if (Network::mNetwork->GetSessionPlayback())
	{
		if (SysKeyDown(DIK_1))
		{
			Network::mNetwork->SetPlaybackSpeed(0.1f);
		}
		if (SysKeyDown(DIK_2))
		{
			Network::mNetwork->SetPlaybackSpeed(0.5f);
		}
		if (SysKeyDown(DIK_3))
		{
			Network::mNetwork->SetPlaybackSpeed(1.0f);
		}
		if (SysKeyDown(DIK_4))
		{
			Network::mNetwork->SetPlaybackSpeed(1.5f);
		}
		if (SysKeyDown(DIK_5))
		{
			Network::mNetwork->SetPlaybackSpeed(2.0f);
		}
		if (SysKeyDown(DIK_0))
		{
			Network::mNetwork->SetPlaybackSpeed(0.0f);
		}
		if (SysKeyDownOnce(DIK_9))
		{
			Network::mNetwork->PlaybackAdvanceTo(Network::mNetwork->GetTime()+10.0f);
			// After advancing the simulation it is a good idea to explicitly poll and use update so the time jump appears clean
			Network::mNetwork->Poll();
			Network::mNetwork->ProcessDataBlockUpdate();
		}
	}


	// If we are not a dedicated server and we are not playing back a recording then run through the player control stuff
	if (!Network::mDedicatedServer && !Network::mNetwork->GetSessionPlayback())
	{
		D3DXVECTOR4 rot = mPlayerConrolledPlane->GetRotation();

		if (firetimeout > 0)
		{
			firetimeout -= GameDatabase::mGameFrameTick;
		}

#ifndef EXTERNALMODE
		if (SysKeyDown(DIK_A))
		{
			autofly = true;
		}

		if (SysKeyDown(DIK_S))
		{
			autofly = false;
		}
#endif

		// Test for the shoot key
		if ((SysKeyDown(DIK_LCONTROL) || autofly) && firetimeout <= 0.0)
		{
			Projectile *proj = new Projectile();
			proj->SetPosition(mPlayerConrolledPlane->GetPosition());
			proj->SetRotation(mPlayerConrolledPlane->GetRotation());
			proj->Publish();
			// Delay for our auto-fire repeat rate
			firetimeout += 0.05f;
			firetimeout += 0.15f;
		}


		if (SysKeyDown(DIK_D))
		{
			float speed = mPlayerConrolledPlane->GetSpeed();
			speed -= 40.0f * GameDatabase::mGameFrameTick;
			if (speed < 0.0f)
			{
				speed = 0.0f;
			}
			mPlayerConrolledPlane->SetSpeed(speed);
		}
		if (SysKeyDown(DIK_F))
		{
			float speed = mPlayerConrolledPlane->GetSpeed();
			speed += 40.0f * GameDatabase::mGameFrameTick;
			if (speed > 40.0f)
			{
				speed = 40.0f;
			}
			mPlayerConrolledPlane->SetSpeed(speed);
		}

		if (SysKeyDown(DIK_LEFT) || autofly)
		{
			rot.z -= D3DXToRadian(100.0f) * GameDatabase::mGameFrameTick;
			if (rot.z > 0)
			{
				rot.z -= D3DXToRadian(100.0f) * GameDatabase::mGameFrameTick;
			}
		}
		if (SysKeyDown(DIK_RIGHT))
		{
			rot.z += D3DXToRadian(100.0f) * GameDatabase::mGameFrameTick;
			if (rot.z < 0)
			{
				rot.z += D3DXToRadian(100.0f) * GameDatabase::mGameFrameTick;
			}
		}
		if (rot.z > D3DXToRadian(80.0f))
		{
			rot.z = D3DXToRadian(80.0f);
		}
		if (rot.z < -D3DXToRadian(80.0f))
		{
			rot.z = -D3DXToRadian(80.0f);
		}

		if (!((SysKeyDown(DIK_LEFT)||autofly)||SysKeyDown(DIK_RIGHT)))
		{
			float max = (float)fabs(rot.z);
			if (max > (D3DXToRadian(80.0f) * GameDatabase::mGameFrameTick))
			{
				max = (D3DXToRadian(80.0f) * GameDatabase::mGameFrameTick);
			}
			if (rot.z > 0)
			{
				rot.z -= max;
			}
			else
			{
				rot.z += max;
			}
		}
		rot.y += (rot.z*1.2f) * GameDatabase::mGameFrameTick;
		D3DXVECTOR4 planepos = mPlayerConrolledPlane->GetPosition();
		if ((SysKeyDown(DIK_UP) && planepos.y > -5.0f))
		{
			rot.x -= D3DXToRadian(80.0f) * GameDatabase::mGameFrameTick;
		}
		if (SysKeyDown(DIK_DOWN))
		{
			rot.x += D3DXToRadian(80.0f) * GameDatabase::mGameFrameTick;
		}
		if (rot.x > D3DXToRadian(45.0f))
		{
			rot.x = D3DXToRadian(45.0f);
		}
		if (rot.x < -D3DXToRadian(45.0f))
		{
			rot.x = -D3DXToRadian(45.0f);
		}
		if (!(  (SysKeyDown(DIK_UP) && planepos.y > -5.0f)||SysKeyDown(DIK_DOWN)))
		{
			float max = (float)fabs(rot.x);
			if (max > (D3DXToRadian(40.0f) * GameDatabase::mGameFrameTick))
			{
				max = D3DXToRadian(40.0f) * GameDatabase::mGameFrameTick;
			}
			if (rot.x > 0)
			{
				rot.x -= max;
			}
			else
			{
				rot.x += max;
			}
		}
		mPlayerConrolledPlane->SetRotation(rot);
	}

	// If there is a valid player controlled plane allocated then use it to render the camera with
	if (mPlayerConrolledPlane)
	{
		D3DXVECTOR4 campt = D3DXVECTOR4(0,5,15,0);
		D3DXVec4Transform(&campt,&campt,&mPlayerConrolledPlane->m_matObjectMatrix);

		campt += mPlayerConrolledPlane->GetPosition();

		D3DXMATRIX matWorld, matView, matProj;

		// Set the transform matrices
		D3DXVECTOR3 vEyePt	= D3DXVECTOR3( campt.x, campt.y, campt.z );
		D3DXVECTOR3 vLookatPt = D3DXVECTOR3( mPlayerConrolledPlane->GetPosition().x,  mPlayerConrolledPlane->GetPosition().y,   mPlayerConrolledPlane->GetPosition().z  );

		D3DXVECTOR4 vUpVec2	= D3DXVECTOR4( 0.0f,  1.0f,   0.0f , 0.0f  );

		D3DXMATRIX planemat;
		D3DXMatrixIdentity(&planemat);
		D3DXMatrixRotationYawPitchRoll(&planemat,mPlayerConrolledPlane->GetRotation().y,mPlayerConrolledPlane->GetRotation().x,mPlayerConrolledPlane->GetRotation().z/2.0f);
		D3DXVECTOR4 vUpVec3	= D3DXVECTOR4( 0.0f,  1.0f,   0.0f , 0.0f );
		D3DXVec4Transform(&vUpVec3,&vUpVec2,&planemat);

		D3DXVECTOR3 vUpVec	= D3DXVECTOR3( vUpVec3.x,  vUpVec3.y,   vUpVec3.z  );

		D3DXMatrixIdentity( &matWorld );
		D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
		FLOAT fAspect = m_d3dsdBackBuffer.Width / (FLOAT)m_d3dsdBackBuffer.Height;
		D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, fAspect, 1.0f, 200.0f );


		m_pd3dDevice->SetTransform( D3DTS_WORLD,	  &matWorld );
		m_pd3dDevice->SetTransform( D3DTS_VIEW,	   &matView );
		m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

		if (mPlayerConrolledCamera)
		{
			mPlayerConrolledCamera->mPosition.x = vEyePt.x;
			mPlayerConrolledCamera->mPosition.y = vEyePt.y;
			mPlayerConrolledCamera->mPosition.z = vEyePt.z;
		}
	}

	framemovetime = Network::mNetwork->GetLocalTime() - framemovetime;

	return S_OK;
}

/**
 * Called once per frame, the call is the entry point for 3d
 *	   rendering. This function sets up render states, clears the
 *	   viewport, and renders the scene.
 */
static int total;
HRESULT CMyD3DApplication::Render()
{
	rendertime = Network::mNetwork->GetLocalTime();

	char buffer[256];

	total = 0;
	HRESULT ret = m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,0xff0000ff, 1.0f, 0L );

	if (ret == D3DERR_INVALIDCALL)
	{
		OutputDebugString("Invalid call\n");
	}

	// Begin the scene
	if( FAILED( m_pd3dDevice->BeginScene() ) )
	{
		return S_OK;
	}

	if (Network::mNetwork->IsStable())
	{
		RenderModels();
	}
	else
	{
		sprintf(buffer,"Waiting on the server");
		m_pFont->DrawText( 128, 128, D3DCOLOR_ARGB(255,255,255,0), buffer );
	}

	if (Network::mNetwork->GetStatus() == ReplicaNet::kReplicaNet_EERROR)
	{
		if (Network::mNetwork->GetSessionPlayback())
		{
			sprintf(buffer,"The playback of the recording has finished");
			// Set the playback speed to 0.0 so the simulation appears to stop. Without this the objects will all appear to carry on moving without any further input exactly like a pause in the network simulation.
			Network::mNetwork->SetPlaybackSpeed(0.0f);
			m_pFont->DrawText( 64, 138, D3DCOLOR_ARGB(255,255,255,0), buffer );
		}
		else
		{
			sprintf(buffer,"There is a session error as there is no defined master");
			m_pFont->DrawText( 64, 138, D3DCOLOR_ARGB(255,255,255,0), buffer );
		}
	}


	rendertime = Network::mNetwork->GetLocalTime() - rendertime;

	// Output statistics
	m_pFont->DrawText( 2,  0, D3DCOLOR_ARGB(255,255,255,0), m_strFrameStats );
	m_pFont->DrawText( 2, 10, D3DCOLOR_ARGB(255,255,255,0), m_strDeviceStats );

	char buffer2[32];
	if (Network::mNetwork->IsMaster())
	{
		strcpy(buffer2,"Master");
	}
	else
	{
		strcpy(buffer2,"Client");
	}

	if (Network::mNetwork->GetCanBecomeMaster())
	{
		strcat(buffer2,"Cbm");
	}
	if (Network::mNetwork->GetCanSpider())
	{
		strcat(buffer2,"Spd");
	}
	if (Network::mNetwork->GetCanAcceptObjects())
	{
		strcat(buffer2,"Cao");
	}

	if (Network::mNetwork->GetStatus() == ReplicaNet::kReplicaNet_EERROR)
	{
		strcat(buffer2,"ERROR");
	}

	sprintf(buffer,"%s ID %d Send bytes/sec %d Recv %d bytes/sec",buffer2,Network::mNetwork->GetSessionID(),(int) Network::mNetwork->GetNetworkSendRate(),(int) Network::mNetwork->GetNetworkReceiveRate());
	m_pFont->DrawText( 2, 20, D3DCOLOR_ARGB(255,255,255,0), buffer );
	sprintf(buffer,"Packets resent %d",Network::mNetwork->GetNetworkPacketsLost());
	m_pFont->DrawText( 2, 30, D3DCOLOR_ARGB(255,255,255,0), buffer );
	sprintf(buffer,"URL: %s",Network::mNetwork->SessionExportURL().c_str());
	m_pFont->DrawText( 2, 40, D3DCOLOR_ARGB(255,255,255,0), buffer );
	sprintf(buffer,"Players %d Network time %.2lf Latency %.2fms",total,Network::mNetwork->GetTime(),Network::mNetwork->GetLatencyToMasterSession()*1000.0f);
	m_pFont->DrawText( 2, 50, D3DCOLOR_ARGB(255,255,255,0), buffer );
#ifndef EXTERNALMODE
	sprintf(buffer,"ReplicaNet %.2fms Frame move %.2fms Render %.2fms",repnettime*1000.0f,framemovetime*1000.0f,rendertime*1000.0f);
	m_pFont->DrawText( 2, 60, D3DCOLOR_ARGB(255,255,255,0), buffer );
#endif

	// End the scene.
	m_pd3dDevice->EndScene();

	GraphRender(m_pd3dDevice , m_pFont);

	return S_OK;
}

HRESULT CMyD3DApplication::RenderModels()
{
#ifndef EXTERNALMODE
	char buffer[256];
#endif

	if (Network::mRenderModels)
	{
		D3DXMatrixTranslation( &m_matTerrainMatrix,   0.0f, 0.0f, 0.0f );
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matTerrainMatrix );
		m_pTerrainObject->Render( m_pd3dDevice );
		D3DXMatrixTranslation( &m_matTerrainMatrix,  80.0f, 0.0f, 0.0f );
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matTerrainMatrix );
		m_pTerrainObject->Render( m_pd3dDevice );
		D3DXMatrixTranslation( &m_matTerrainMatrix, -80.0f, 0.0f, 0.0f );
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matTerrainMatrix );
		m_pTerrainObject->Render( m_pd3dDevice );
		D3DXMatrixTranslation( &m_matTerrainMatrix,   0.0f, 0.0f, 80.0f );
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matTerrainMatrix );
		m_pTerrainObject->Render( m_pd3dDevice );
		D3DXMatrixTranslation( &m_matTerrainMatrix,  80.0f, 0.0f, 80.0f );
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matTerrainMatrix );
		m_pTerrainObject->Render( m_pd3dDevice );
		D3DXMatrixTranslation( &m_matTerrainMatrix, -80.0f, 0.0f, 80.0f );
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matTerrainMatrix );
		m_pTerrainObject->Render( m_pd3dDevice );
		D3DXMatrixTranslation( &m_matTerrainMatrix,   0.0f, 0.0f, -80.0f );
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matTerrainMatrix );
		m_pTerrainObject->Render( m_pd3dDevice );
		D3DXMatrixTranslation( &m_matTerrainMatrix,  80.0f, 0.0f, -80.0f );
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matTerrainMatrix );
		m_pTerrainObject->Render( m_pd3dDevice );
		D3DXMatrixTranslation( &m_matTerrainMatrix, -80.0f, 0.0f, -80.0f );
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matTerrainMatrix );
		m_pTerrainObject->Render( m_pd3dDevice );
	}

	int starty = 70;

	/* Again lock our game objects list while we render */
	Network::mNetwork->LockObjects();

	int i;
	for (i=0;i<(int)GameDatabase::mGameObjects.size();i++)
	{
		ReplicaObject *robject;
		// Get the derived class object pointer that contains a base class type of ReplicaObject from the GameObject class
		robject = GameDatabase::mGameObjects[i]->GetReplicaObject();
		// If there is no object pointer then ignore this object for rendering
		if (!robject)
		{
			continue;
		}
		// Count the number of plane models as the number of players active
		if (robject->GetClassID() == _MAKE_RO(Plane)::StaticGetClassID())
		{
			total++;
		}
		if (Network::mRenderModels)
		{
			// Compare the class id of the ReplicaObject with the class ID of the ReplicaObject Plane object
			// If they are the same then we can assume the type cast to a Plane class is valid
			if (robject->GetClassID() == _MAKE_RO(Plane)::StaticGetClassID())
			{
				// Perform the type cast
				Plane *theplane = (Plane *) GameDatabase::mGameObjects[i];

				// Calculate the real matrix for the object
				D3DXMatrixRotationYawPitchRoll(&theplane->m_matObjectMatrix,theplane->GetRotation().y,theplane->GetRotation().x,theplane->GetRotation().z);
				D3DXMATRIX transmat;
				D3DXMatrixTranslation(&transmat,theplane->GetPosition().x,theplane->GetPosition().y,theplane->GetPosition().z);
				D3DXMatrixMultiply( &theplane->m_matObjectMatrix, &theplane->m_matObjectMatrix, &transmat);
				m_matObjectMatrix = theplane->m_matObjectMatrix;
				m_pd3dDevice->SetTransform( D3DTS_WORLD, &theplane->m_matObjectMatrix );
				theplane->mMesh->Render( m_pd3dDevice );

				m_pShadowVolume = 0;
			}
			if (robject->GetClassID() == _MAKE_RO(Projectile)::StaticGetClassID())
			{
				// Perform the type cast
				Projectile *theprojectile = (Projectile *) GameDatabase::mGameObjects[i];

				// Calculate the real matrix for the object
				D3DXMatrixRotationYawPitchRoll(&theprojectile->m_matObjectMatrix,theprojectile->GetRotation().y,theprojectile->GetRotation().x,theprojectile->GetRotation().z);

				D3DXMATRIX transmat;
				D3DXMatrixTranslation(&transmat,theprojectile->GetPosition().x,theprojectile->GetPosition().y,theprojectile->GetPosition().z);
				D3DXMatrixMultiply( &theprojectile->m_matObjectMatrix, &theprojectile->m_matObjectMatrix, &transmat);
				m_matObjectMatrix = theprojectile->m_matObjectMatrix;
				m_pd3dDevice->SetTransform( D3DTS_WORLD, &theprojectile->m_matObjectMatrix );

				gpProjectileMesh->Render(m_pd3dDevice);

				m_pShadowVolume = 0;
			}
			if (robject->GetClassID() == _MAKE_RO(Enemy)::StaticGetClassID())
			{
				// Perform the type cast
				Enemy *theobject = (Enemy *) GameDatabase::mGameObjects[i];

				// Calculate the real matrix for the object
				D3DXMATRIX matrix;
				D3DXMatrixRotationYawPitchRoll(&matrix,theobject->GetRotation().y,theobject->GetRotation().x,theobject->GetRotation().z);

				D3DXMATRIX temp;
				D3DXMatrixScaling(&temp,0.05f,0.05f,0.05f);
				D3DXMatrixMultiply(&matrix, &matrix, &temp);

				D3DXMatrixRotationYawPitchRoll(&temp,D3DXToRadian(-90.0f),0,0);
				D3DXMatrixMultiply(&matrix, &temp,&matrix);

				D3DXMATRIX transmat;
				D3DXMatrixTranslation(&transmat,theobject->GetPosition().x,theobject->GetPosition().y,theobject->GetPosition().z);
				D3DXMatrixMultiply( &matrix, &matrix, &transmat);
				m_matObjectMatrix = matrix;
				m_pd3dDevice->SetTransform( D3DTS_WORLD, &matrix );

				gpEnemyMesh->Render(m_pd3dDevice);

				m_pShadowVolume = 0;
			}
		} // if (!gDedicatedServer)

#ifndef EXTERNALMODE
		sprintf(buffer,"Object mst %d sessid %d uni %d clsid %d ",robject->IsMaster(),robject->GetSessionID(),robject->GetUniqueID(),robject->GetClassID());
		m_pFont->DrawText( 2.0f, (float)starty, D3DCOLOR_ARGB(255,255,255,0), buffer );
		starty += 10;
#endif
	}

	/* Now unlock our game objects list since we don't access it from now on */
	Network::mNetwork->UnLockObjects();

	return S_OK;
}

