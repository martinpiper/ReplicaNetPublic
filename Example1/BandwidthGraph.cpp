/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
/*
Uses the ReplicaNet socket debug bandwidth values to display a draw into a LPDIRECT3DDEVICE8
*/
#define STRICT
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include "BandwidthGraph.h"
#include "RNXPSockets/Inc/XPSocket.h"
#include "RNPlatform/Inc/SysTime.h"

using namespace RNReplicaNet;

static LPDIRECT3DVERTEXBUFFER8 spVertices = NULL;

static bool sFirst = true;
static SysTime sPacketTimer;
static const int kNumPacketHistory = 100;
static float sSentPerSec[kNumPacketHistory];
static float sRecvPerSec[kNumPacketHistory];

void GraphRender(LPDIRECT3DDEVICE8 pd3dDevice , CD3DFont* pFont)
{
	if (sFirst)
	{
		sFirst = false;
		memset( sSentPerSec , 0 , sizeof(sSentPerSec) );
		memset( sRecvPerSec , 0 , sizeof(sRecvPerSec) );
	}
	// Update the graph data only every second or so.
	if ( sPacketTimer.FloatTime() > 1.0f )
	{
		sPacketTimer.Reset();

		// Shift down
		int i;
		for ( i = 0 ; i < kNumPacketHistory-1 ; i++ )
		{
			sSentPerSec[ i ] = sSentPerSec[ i + 1 ];
			sRecvPerSec[ i ] = sRecvPerSec[ i + 1 ];
		}

		// Add new entries based on the physical packets sent over the network.
		// This includes any packets resent due to latency or packet loss.
		sSentPerSec[ kNumPacketHistory-1 ]  = asocket_packets_got_size_per_second;
		sRecvPerSec[ kNumPacketHistory-1 ]  = asocket_packets_sent_size_per_second;
	}

	float maxSentRecv = 0.0f;
	int i;
	for ( i = 0 ; i < kNumPacketHistory-1 ; i++ )
	{
		if ( sSentPerSec[ i ]  > maxSentRecv )
		{
			maxSentRecv = sSentPerSec[ i ];
		}

		if ( sRecvPerSec[ i ]  > maxSentRecv )
		{
			maxSentRecv = sRecvPerSec[ i ];
		}
	}

	// Draw the graphs
	if( SUCCEEDED( pd3dDevice->BeginScene() ) )
	{
		DWORD oldLightState;
		pd3dDevice->GetRenderState(D3DRS_LIGHTING, &oldLightState);

		pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		pd3dDevice->Clear( 0L, NULL, D3DCLEAR_ZBUFFER,0xff0000ff, 1.0f, 0L );

		D3DXMATRIX Ortho2D;
		D3DXMATRIX Identity;
		D3DXMatrixOrthoLH(&Ortho2D, 100.0f, 100.0f , 0.0f, 1.0f);
		D3DXMatrixIdentity(&Identity);
		pd3dDevice->SetTransform(D3DTS_PROJECTION, &Ortho2D);
		pd3dDevice->SetTransform(D3DTS_WORLD, &Identity);
		pd3dDevice->SetTransform(D3DTS_VIEW, &Identity);

		D3DXMATRIX Position;
		D3DXMatrixTranslation(&Position, -49.0f, -49.0f, 0.0f);
		pd3dDevice->SetTransform(D3DTS_WORLD, &Position);

		struct PANELVERTEX
		{
			FLOAT x, y, z;
			DWORD color;
		};
		#define D3DFVF_PANELVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

		float PanelWidth = 100.0f;
		float PanelHeight = 20.0f;
		if ( !spVertices )
		{
			pd3dDevice->CreateVertexBuffer(kNumPacketHistory * sizeof(PANELVERTEX), D3DUSAGE_WRITEONLY, D3DFVF_PANELVERTEX, D3DPOOL_MANAGED, &spVertices);
		}
		PANELVERTEX* pVertices = NULL;
		int i;

		spVertices->Lock(0, kNumPacketHistory * sizeof(PANELVERTEX), (BYTE**)&pVertices, 0);
		for ( i = 0 ; i < kNumPacketHistory ; i++ )
		{
			pVertices[i].color = D3DCOLOR_ARGB(255,255,0,0);
			pVertices[i].x = (PanelWidth * i) / kNumPacketHistory;
			pVertices[i].y = (PanelHeight * sSentPerSec[i]) / maxSentRecv;
			pVertices[i].z = 1.0f;
		}
		spVertices->Unlock();

		pd3dDevice->SetVertexShader(D3DFVF_PANELVERTEX);
		pd3dDevice->SetStreamSource(0, spVertices, sizeof(PANELVERTEX));
		pd3dDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, kNumPacketHistory-1);

		spVertices->Lock(0, kNumPacketHistory * sizeof(PANELVERTEX), (BYTE**)&pVertices, 0);
		for ( i = 0 ; i < kNumPacketHistory ; i++ )
		{
			pVertices[i].color = D3DCOLOR_ARGB(255,0,255,0);
			pVertices[i].x = (PanelWidth * i) / kNumPacketHistory;
			pVertices[i].y = (PanelHeight * sRecvPerSec[i]) / maxSentRecv;
			pVertices[i].z = 1.0f;
		}
		spVertices->Unlock();

		pd3dDevice->SetVertexShader(D3DFVF_PANELVERTEX);
		pd3dDevice->SetStreamSource(0, spVertices, sizeof(PANELVERTEX));
		pd3dDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, kNumPacketHistory-1);

		D3DVIEWPORT8 vp;
		pd3dDevice->GetViewport( &vp );

		char buffer[256];
		sprintf(buffer,"%d bytes/sec", (int)maxSentRecv);
		pFont->DrawText( 2, (float) vp.Height - ( (vp.Height * PanelHeight ) / 100 ) , D3DCOLOR_ARGB(255,255,255,0), buffer );


		pd3dDevice->SetRenderState(D3DRS_LIGHTING, oldLightState);

		// End the scene.
		pd3dDevice->EndScene();

	}
}
