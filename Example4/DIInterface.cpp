/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
// Direct Input Interface code
#include <stdio.h>
#include <windows.h> 
#include <assert.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h> 
#include <math.h> 
#include "diinterface.h"

// Lots of nasty globals
TDIHandler* gDIHandler = 0;
bool gIgnoreDInputWarnings = true;


TDIDevice::TDIDevice(void)
{
	fEffect = NULL;
	fDevice = NULL;
	fBuffer = NULL;

	fJoystick.fDIDevice = this;
}

TDIDevice::~TDIDevice(void)
{
	if (fBuffer) delete [] fBuffer;
	
	if (fDevice)
	{
		fDevice->Unacquire();
		fDevice->Release();
	}
}




//
//  void TDIDevice::Acquire(int acquire)
//
//
//
void TDIDevice::Acquire(int acquire)
{
	HRESULT hr;
	
	if (acquire)
	{
		hr=fDevice->Acquire();
		if (NULL != fEffect) 
		{	  
			HRESULT hr2;
			hr2 = fEffect->Start( 1, 0 );
		}
	}
	else
		hr=fDevice->Unacquire();

#ifdef _DEBUG
	if (!gIgnoreDInputWarnings)
	{
		switch (hr)
		{
			case DI_OK :
				printf("TDIDevice::Acquire - result = DI_OK\n");
				break;

			case S_FALSE :
				printf("TDIDevice::Acquire - result = S_FALSE (device already acquired)\n");
				break;

			case DIERR_INVALIDPARAM :
				printf("TDIDevice::Acquire - result = DIERR_INVALIDPARAM\n");
				break;

			case DIERR_NOTINITIALIZED :
				printf("TDIDevice::Acquire - result = DIERR_NOTINITIALIZED\n");
				break;

			case DIERR_OTHERAPPHASPRIO :
				printf("TDIDevice::Acquire - result = DIERR_OTHERAPPHASPRIO\n");
				break;
		};

		assert(SUCCEEDED(hr));
	}
#endif
}


//
// void TDIDevice::Read()
//
//
//
void TDIDevice::Read()
{
	HRESULT dirval = fDevice->GetDeviceState(fBufSize,(LPVOID)fBuffer);
	if( dirval == DI_OK )
		return;
	
	assert(dirval == DIERR_INPUTLOST || dirval == DIERR_NOTACQUIRED);
	dirval = fDevice->Acquire();		// Might fail, but will work next frame
	memset(fBuffer,0,fBufSize);
}



//
//  void TDIDevice::InitialiseKeyboard(REFGUID guid,LPCDIDATAFORMAT dataFormat,int bufferSize)
//
//
//
extern HWND					ghWnd; 
void TDIKeyboard::InitialiseKeyboard(REFGUID guid,LPCDIDATAFORMAT dataFormat,int bufferSize)
{
	memset(fDeBounce, 0, 256 * sizeof (int));

#if DIRECTINPUT_VERSION < DIRECTINPUT8_VERSION
	LPDIRECTINPUTDEVICE tempDevice;
#endif

	// initialise the buffer
	fBufSize=bufferSize;
	fBuffer = new char[bufferSize];
	memset(fBuffer,0,bufferSize);
	
#if DIRECTINPUT_VERSION >= DIRECTINPUT8_VERSION
	// (DirectX8) create keyboard device
	#if D3D_SDK_VERSION == DX_BETA_WEEKLY2
		HRESULT hr = gDIHandler->fDI->CreateDevice(	guid,							// guid
													&fDevice,						// device
													NULL );							// must be set to NULL
	#else
		HRESULT hr = gDIHandler->fDI->CreateDeviceEx(	guid,						// guid
														IID_IDirectInputDevice8,	// interface req
														(LPVOID*) &fDevice,			// device
														NULL );						// must be set to NULL
	#endif
#else
	// create keyboard device
	HRESULT hr = gDIHandler->fDI->CreateDevice(	guid,								// guid
												&tempDevice,						// device
												NULL );								// must be set to NULL
	assert(SUCCEEDED(hr));

	// query for IDirectInputDevice2 interface
	hr = tempDevice->QueryInterface( IID_IDirectInputDevice2, (VOID**)&fDevice );
	assert(SUCCEEDED(hr));
#endif
	
	// set the data format
	hr = fDevice->SetDataFormat(dataFormat); 
	assert(SUCCEEDED(hr));
	
	// set the cooperative level
	hr = fDevice->SetCooperativeLevel(ghWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); 
	assert(SUCCEEDED(hr));
	
	// acquire it
	Acquire(true); 
}


//
//   void TDIKeyboard::ReadInputs(void) 
//
//   fills in the fJoystick flags with the keyboard
//
void TDIKeyboard::ReadInputs(void) 
{
	fJoystick.Clear();

	Read();		  // Check we have the keyboard

	if (SysKeyDown(DIK_LEFT))	   fJoystick.fButtons |= JOYSTICK_LEFT;
	if (SysKeyDown(DIK_RIGHT))	  fJoystick.fButtons |= JOYSTICK_RIGHT;
	if (SysKeyDown(DIK_UP))		 fJoystick.fButtons |= JOYSTICK_UP;
	if (SysKeyDown(DIK_DOWN))	   fJoystick.fButtons |= JOYSTICK_DOWN;
	if (SysKeyDown(DIK_SPACE))	  fJoystick.fButtons |= JOYSTICK_JUMP;
	if (SysKeyDown(DIK_H))			fJoystick.fButtons |= JOYSTICK_HORN;
	if (SysKeyDown(DIK_O))			fJoystick.fButtons |= JOYSTICK_USE1;
	if (SysKeyDown(DIK_P))			fJoystick.fButtons |= JOYSTICK_USE2;
	if (SysKeyDown(DIK_J))		  fJoystick.fButtons |= JOYSTICK_USE3;
	if (SysKeyDown(DIK_TAB))		fJoystick.fButtons |= JOYSTICK_PULL_CAMERA;

	if (SysKeyDown(DIK_N))		  fJoystick.fButtons |= JOYSTICK_LEFT;
	if (SysKeyDown(DIK_M))		  fJoystick.fButtons |= JOYSTICK_RIGHT;
	if (SysKeyDown(DIK_A))		  fJoystick.fButtons |= JOYSTICK_ACCELERATE;
	if (SysKeyDown(DIK_Z))		  fJoystick.fButtons |= JOYSTICK_BRAKE;

	if (SysKeyDown(DIK_W))		  fJoystick.fButtons |= JOYSTICK_W;
	if (SysKeyDown(DIK_A))		  fJoystick.fButtons |= JOYSTICK_A;
	if (SysKeyDown(DIK_S))		  fJoystick.fButtons |= JOYSTICK_S;
	if (SysKeyDown(DIK_D))		  fJoystick.fButtons |= JOYSTICK_D;	
}


//
// void TDIJoystick::ReadInputs(void) 
//
//   Read the joystick
//
void TDIJoystick::ReadInputs(void) 
{
	DIJOYSTATE*			  js;  

	// poll the joystick to read the current state
	fDevice->Poll(); 

	// Fill in the buffer
	Read();
	
	js = (DIJOYSTATE*)fBuffer;

	fJoystick.Clear();

	if (0)
	{
		int i = 0;

		/*if (js->lX > 0) { gRenderer->DebugPrintf(200, 10 + 10*i, "lX +ve (RHS)"); i++; }
		if (js->lX < 0) { gRenderer->DebugPrintf(200, 10 + 10*i, "lX -ve (LHS)"); i++; }

		if (js->lY > 0) { gRenderer->DebugPrintf(200, 10 + 10*i, "lY -ve (FWD)"); i++; }
		if (js->lY < 0) { gRenderer->DebugPrintf(200, 10 + 10*i, "lY +ve (REV)"); i++; }*/

		char buf[256];
		for(int c = 0; c < 32; c++)
			if (js->rgbButtons[c] & 0x80) 
			{ 
				sprintf(buf, "BTN %d\0", c); 
			   // gRenderer->DebugPrintf(200, 10 + 10*i, buf);
				i++; 
			}
	}

	// Now study the position of the stick and the buttons.

	fJoystick.fstickX = js->lX;
	fJoystick.fstickY = js->lY;

	// USE1 reverse
	// USE2 re-start
	// JUMP jump

	int									deadZone = 150;

  //  if (!gPreferences.fUseWheel)
	{
	 /*   if (js->lX < -deadZone)		 fJoystick.fButtons |= JOYSTICK_LEFT;
		else if (js->lX > deadZone)	 fJoystick.fButtons |= JOYSTICK_RIGHT;

		if (js->lY < -deadZone)		 fJoystick.fButtons |= JOYSTICK_UP;
		else if (js->lY > deadZone)	 fJoystick.fButtons |= JOYSTICK_DOWN;*/

		if (js->rgbButtons[7] & 0x80)   
			fJoystick.fButtons |= JOYSTICK_ACCELERATE; 
		
		if (js->rgbButtons[6] & 0x80)   
			fJoystick.fButtons |= JOYSTICK_BRAKE;

		if (js->rgbButtons[1] & 0x80)  
			fJoystick.fButtons |= JOYSTICK_JUMP;

		if (js->rgbButtons[2] & 0x80)   
			fJoystick.fButtons |= JOYSTICK_USE1;

		if (js->rgbButtons[8] & 0x80)   
			fJoystick.fButtons |= JOYSTICK_USE2;

		if (js->rgbButtons[9] & 0x80)   
			fJoystick.fButtons |= JOYSTICK_HORN;

		if (js->rgbButtons[3] & 0x80)   
			fJoystick.fButtons |= JOYSTICK_USE3;

		if (js->rgbButtons[4] & 0x80)   
			fJoystick.fButtons |= JOYSTICK_PULL_CAMERA;

		if (js->rgbButtons[5] & 0x80)
			fJoystick.fButtons |= JOYSTICK_W;

		if (js->rgbButtons[0] & 0x80)
			fJoystick.fButtons |= JOYSTICK_A;

		if (js->rgbButtons[10] & 0x80)
			fJoystick.fButtons |= JOYSTICK_S;

		if (js->rgbButtons[11] & 0x80)
			fJoystick.fButtons |= JOYSTICK_D;
	}

	// Read the 'hat' (digital joypad directions)
	if (LOWORD(js->rgdwPOV[0]) == 0xFFFF)			// -1 -> centre
	{
		fJoystick.frightstickX = 0;
		fJoystick.frightstickY = 0;
	}
	else
	{
		fJoystick.frightstickX = long(sinf(float(js->rgdwPOV[0]) / 100.0f / 360.0f * 2.0f * 3.14159265f) * 1000.0f);
		fJoystick.frightstickY = long(cosf(float(js->rgdwPOV[0]) / 100.0f / 360.0f * 2.0f * 3.14159265f) * -1000.0f);
	}

   /* else // hacked wheel case...
	{
		if (js->lX < 0)				 fJoystick.fButtons |= JOYSTICK_LEFT;
		else if (js->lX > 0)			fJoystick.fButtons |= JOYSTICK_RIGHT;

		if (js->lY < -100)				 fJoystick.fButtons |= JOYSTICK_ACCELERATE;
		else if (js->lY > 100)			fJoystick.fButtons |= JOYSTICK_BRAKE;

		if (js->rgbButtons[0] & 0x80)   fJoystick.fButtons |= JOYSTICK_JUMP;
		if (js->rgbButtons[3] & 0x80)   fJoystick.fButtons |= JOYSTICK_USE1;
		if (js->rgbButtons[5] & 0x80)   fJoystick.fButtons |= JOYSTICK_USE2;
	}*/
} 


//
//  TDIHandler::TDIHandler(void)
//
//
//
TDIHandler::TDIHandler(void)
{
	fDI=NULL;
	fNumDevices = 0;
	fNextJoystick = 0;

	for (int i = 0; i < MAX_NUM_DEVICES; i++)
	{
		fDevice[i] = NULL;
	}

	fMouse = NULL;
	fGrabbed = false;
}



//
//  TDIHandler::~TDIHandler(void)
//
//
//
TDIHandler::~TDIHandler(void)
{
	for (int i = 0; i < fNumDevices; i++)
	{
		delete fDevice[i];
	}


	if (fDI)
	{
		fDI->Release();
		fDI=NULL;
	}

	if (fMouse != NULL)
	{
		fMouse->Unacquire();
		fMouse->Release();
		fMouse = NULL;
	}
}



//
// void TDIHandler::Initialise(void)
//
//
//  Create the input driver and the devices (keyboard and joystick) here
//
extern HINSTANCE	gAppInstance;
void TDIHandler::Initialise(void)
{
// DirectX 8 initialisation
#if DIRECTINPUT_VERSION >= DIRECTINPUT8_VERSION
	HRESULT hr = DirectInput8Create(	gAppInstance,
										DIRECTINPUT_VERSION,
										IID_IDirectInput8,
										(LPVOID*) &fDI,
										NULL );
#else
	// Previous versions
	HRESULT hr = DirectInputCreate(gAppInstance,DIRECTINPUT_VERSION,&fDI,NULL);
#endif
	assert(SUCCEEDED(hr));

// Enumerate for available joystick/game controllers
#if DIRECTINPUT_VERSION >= DIRECTINPUT8_VERSION_WEEKLY2
	// DirectX 8 enumeration
	// joysticks are now referred to as game controllers
	hr = fDI->EnumDevices(	DI8DEVCLASS_GAMECTRL,
							InitialiseJoystickEnum,
							this,
							DIEDFL_ATTACHEDONLY );
#else
	hr = fDI->EnumDevices(	DIDEVTYPE_JOYSTICK,
							InitialiseJoystickEnum,
							this,
							DIEDFL_ATTACHEDONLY );
#endif
	assert(SUCCEEDED(hr));

	for (int i = 0; i < fNumDevices; i++)
	{
		{
			HRESULT  hr;

			hr = fDevice[i]->fDevice->EnumEffects(	JoystickEffectsEnum,
													(LPVOID) fDevice[i]->fDevice,
													DIEFT_CONSTANTFORCE );
			assert(hr == DI_OK);

			// acquire device
			fDevice[i]->Acquire(true);
		}
	}

	// create the system keyboard
	fDevice[fNumDevices] = new TDIKeyboard;
	fDISysKeyboard = (TDIKeyboard*)fDevice[fNumDevices];  // Cache ptr to keyboard for fast key reading in editor
	fDISysKeyboard->InitialiseKeyboard(GUID_SysKeyboard,&c_dfDIKeyboard,256);
	fNumDevices++;

	InitMouse();
}


//
//  void TDIHandler::ReadAllInputs()
//
//   Called every frame,  fills in the fJoystick flags within each device
//
void TDIHandler::ReadAllInputs()
{
	for (int i = 0; i < fNumDevices; i++)
		fDevice[i]->ReadInputs();

#if 0
	static int counter = 0;
	DWORD max_t = 0;
	DWORD pre_t = timeGetTime();

	counter++;
	if ( counter > 100 )
	{
		DWORD t = timeGetTime() - pre_t;
		max_t = max(max_t, t);
		gRenderer->DebugPrintf(40, 250, "INPUT TIME: %d", t);
		gRenderer->DebugPrintf(40, 260, "INPUT TIME: %d MAX", max_t);
	}
#endif
}

TJoystick* TDIHandler::GetJoystick(int theIndex)
{
	// Used to get joysticks for recording, probably only ever used to 
	// get joystick 0
	assert (theIndex < MAX_NUM_DEVICES);
	assert (fDevice[theIndex] != NULL);

	return &fDevice[theIndex]->fJoystick;

}


TJoystick* TDIHandler::GetNextJoystick()
{
	// Used to alloca t the joysticks on a first-come first served basis

	if (fNextJoystick >= MAX_NUM_DEVICES)
		return NULL;

	if (fDevice[fNextJoystick] != NULL)
	{
		fNextJoystick++;
		return (&fDevice[fNextJoystick - 1]->fJoystick);
	}

	return NULL;
}


void TDIHandler::ResetJoystickAllocation()
{
	fNextJoystick = 0;
}


BOOL PASCAL JoystickEffectsEnum(LPCDIEFFECTINFO pdei, LPVOID pvRef)
{
	return true;
}


void TDIDevice::SetForce(float x, float y)
{
	if (!fEffect) return;

	// Modifying an effect is basically the same as creating
	// a new one, except you need only specify the parameters
	// you are modifying; the rest are left alone.
	LONG rglDirection[2] = { (LONG)x, (LONG)y };

	DICONSTANTFORCE		cf;
	DIEFFECT			eff;
	cf.lMagnitude = (DWORD)sqrt( (double)x * (double)x +
								 (double)y * (double)y );

	eff.dwSize = sizeof(DIEFFECT);
	eff.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
	eff.cAxes = 2;
	eff.rglDirection = rglDirection;
	eff.lpEnvelope = 0;
	eff.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
	eff.lpvTypeSpecificParams = &cf;

	// now set the new parameters and start the effect immediately.
	HRESULT hr;
	hr = fEffect->SetParameters(	&eff, 
									DIEP_DIRECTION |
									DIEP_TYPESPECIFICPARAMS |
									DIEP_START );

	if (FAILED(hr)) assert(false);
}


//
//  BOOL PASCAL InitialiseJoystickEnum(LPCDIDEVICEINSTANCE pdinst, LPVOID pvRef) 
//
//   Call back for enumeration of the joystick devices
//
BOOL PASCAL InitialiseJoystickEnum(LPCDIDEVICEINSTANCE pdinst, LPVOID pvRef) 
{ 
   TDIHandler*		handler = (TDIHandler*)pvRef;
   HRESULT			hResult;

#if DIRECTINPUT_VERSION >= DIRECTINPUT8_VERSION
   // DirectX 8 pointers
   LPDIRECTINPUT8			pdi = handler->fDI;
#else
   // Previous version pointers
   LPDIRECTINPUT			pdi = handler->fDI; 
   LPDIRECTINPUTDEVICE		tempDevice;		  // Till we get a v2 interface
#endif

   if (handler->fDevice[handler->fNumDevices] == NULL)			  
   {
	  handler->fDevice[handler->fNumDevices] = new TDIJoystick;
	  assert (handler->fDevice[handler->fNumDevices] != NULL);
   }

#if DIRECTINPUT_VERSION >= DIRECTINPUT8_VERSION
	#if D3D_SDK_VERSION == DX_BETA_WEEKLY2
		hResult = pdi->CreateDevice(	pdinst->guidInstance,
										&handler->fDevice[handler->fNumDevices]->fDevice,
										NULL );
	#else
		hResult = pdi->CreateDeviceEx(	pdinst->guidInstance,
										IID_IDirectInputDevice8,
										(LPVOID*) &handler->fDevice[handler->fNumDevices]->fDevice,
										NULL );
	#endif
#else
	hResult = pdi->CreateDevice(	pdinst->guidInstance,
									&tempDevice,
									NULL );
#endif

	if( FAILED( hResult ) )
	{ 
		OutputDebugString("IDirectInput::CreateDevice FAILED\n"); 
		delete handler->fDevice[handler->fNumDevices];
		handler->fDevice[handler->fNumDevices] = NULL;
		return DIENUM_CONTINUE;	
	} 

#if DIRECTINPUT_VERSION < DIRECTINPUT8_VERSION
	//  Get a version 2 interface
	if (tempDevice->QueryInterface(IID_IDirectInputDevice2, (VOID**)&handler->fDevice[handler->fNumDevices]->fDevice) != DI_OK)
	{ 
		OutputDebugString("IDirectInput::Getting device interface 2 FAILED\n"); 
		delete handler->fDevice[handler->fNumDevices];
		handler->fDevice[handler->fNumDevices] = NULL;
		return DIENUM_CONTINUE;	
	}
#endif

	//  Tell direct input its a joystick
	if (handler->fDevice[handler->fNumDevices]->fDevice->SetDataFormat(&c_dfDIJoystick) != DI_OK) 
	{ 
		OutputDebugString("IDirectInputDevice::SetDataFormat FAILED\n"); 
		handler->fDevice[handler->fNumDevices]->fDevice->Release(); 
		delete handler->fDevice[handler->fNumDevices];
		handler->fDevice[handler->fNumDevices] = NULL;
		return DIENUM_CONTINUE; 
	} 

	// Set the cooperation level
	//if(handler->fDevice[handler->fNumDevices]->fDevice->SetCooperativeLevel(ghWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND) != DI_OK) 
	if(handler->fDevice[handler->fNumDevices]->fDevice->SetCooperativeLevel(ghWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND) != DI_OK) 
	{ 
		OutputDebugString("IDirectInputDevice::SetCooperativeLevel FAILED\n"); 
		handler->fDevice[handler->fNumDevices]->fDevice->Release(); 
		delete handler->fDevice[handler->fNumDevices];
		handler->fDevice[handler->fNumDevices] = NULL;
		return DIENUM_CONTINUE;
	} 


	// Set the range to be returned for the x-axis, from -1000 to 1000
	DIPROPRANGE diprg;	 
	diprg.diph.dwSize	   = sizeof(diprg); 
	diprg.diph.dwHeaderSize = sizeof(diprg.diph); 
	diprg.diph.dwObj		= DIJOFS_X; 
	diprg.diph.dwHow		= DIPH_BYOFFSET;	
	diprg.lMin			  = -1000; 
	diprg.lMax			  = +1000;  
   
	if FAILED(handler->fDevice[handler->fNumDevices]->fDevice->SetProperty(DIPROP_RANGE, &diprg.diph))	
	{ 
		OutputDebugString("IDirectInputDevice::SetProperty(DIPH_RANGE) FAILED\n");	   
		handler->fDevice[handler->fNumDevices]->fDevice->Release(); 
		delete handler->fDevice[handler->fNumDevices];
		handler->fDevice[handler->fNumDevices] = NULL;
		return DIENUM_CONTINUE;	
	}

	// Set the range to be returned for the y-axis, from -1000 to 1000
	diprg.diph.dwSize	   = sizeof(diprg); 
	diprg.diph.dwHeaderSize = sizeof(diprg.diph); 
	diprg.diph.dwObj		= DIJOFS_Y; 
	diprg.diph.dwHow		= DIPH_BYOFFSET;	
	diprg.lMin			  = -1000; 
	diprg.lMax			  = +1000;  

	if FAILED(handler->fDevice[handler->fNumDevices]->fDevice->SetProperty(DIPROP_RANGE, &diprg.diph))	
	{ 
		OutputDebugString("IDirectInputDevice::SetProperty(DIPH_RANGE) FAILED\n");	   
		handler->fDevice[handler->fNumDevices]->fDevice->Release(); 
		delete handler->fDevice[handler->fNumDevices];
		handler->fDevice[handler->fNumDevices] = NULL;
		return DIENUM_CONTINUE;	
	} 


	// Set the dead zone to extend for half the range of the x axis movement
	DIPROPDWORD dipdead;

	dipdead.diph.dwSize	   = sizeof(dipdead); 
	dipdead.diph.dwHeaderSize = sizeof(dipdead.diph); 
	dipdead.diph.dwObj		= DIJOFS_X; 
	dipdead.diph.dwHow		= DIPH_BYOFFSET;   
	dipdead.dwData = 0;
	//(gPreferences.fUseWheel) ? dipdead.dwData = 0 : dipdead.dwData = 200; // the range


	// set X axis dead zone to 50% (to avoid accidental turning) 
	if FAILED(handler->fDevice[handler->fNumDevices]->fDevice->SetProperty(DIPROP_DEADZONE, &dipdead.diph)) 
	{ 
		OutputDebugString("IDirectInputDevice:: SetProperty(DIPH_DEADZONE) FAILED\n"); 
		handler->fDevice[handler->fNumDevices]->fDevice->Release(); 
		delete handler->fDevice[handler->fNumDevices];
		handler->fDevice[handler->fNumDevices] = NULL;
		return DIENUM_CONTINUE;	
	} 

	// Set the dead zone to extend for half the range of the y axis movement
	dipdead.diph.dwSize	   = sizeof(dipdead); 
	dipdead.diph.dwHeaderSize = sizeof(dipdead.diph); 
	dipdead.diph.dwObj		= DIJOFS_Y; 
	dipdead.diph.dwHow		= DIPH_BYOFFSET;	
	dipdead.dwData			= 200;				 // the range


	// set X axis dead zone to 50% (to avoid accidental turning) 
	if FAILED(handler->fDevice[handler->fNumDevices]->fDevice->SetProperty(DIPROP_DEADZONE, &dipdead.diph)) 
	{ 
		OutputDebugString("IDirectInputDevice:: SetProperty(DIPH_DEADZONE) FAILED\n"); 
		handler->fDevice[handler->fNumDevices]->fDevice->Release(); 
		delete handler->fDevice[handler->fNumDevices];
		handler->fDevice[handler->fNumDevices] = NULL;
		return DIENUM_CONTINUE;	
	} 

	{
		HRESULT hr;

		// remove the force centering effect

		DIPROPDWORD dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = FALSE;

		//hr = handler->fDevice[handler->fNumDevices]->fDevice->
		//	SetProperty( DIPROP_AUTOCENTER, &dipdw.diph );
//		if (hr != DI_OK) assert(false);	 // Removed ph 10/11/98

		// This application needs only one effect:  Applying raw forces.
		DIEFFECT eff;
		DWORD rgdwAxes[2] = { DIJOFS_X, DIJOFS_Y };
		LONG rglDirection[2] = { 0, 0 };
		DICONSTANTFORCE cf = { 0 };

		eff.dwSize = sizeof(DIEFFECT);
		eff.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
		eff.dwDuration = INFINITE;
		eff.dwSamplePeriod = 0;
		eff.dwGain = DI_FFNOMINALMAX;
		eff.dwTriggerButton = DIEB_NOTRIGGER;
		eff.dwTriggerRepeatInterval = 0;
		eff.cAxes = 2;
		eff.rgdwAxes = rgdwAxes;
		eff.rglDirection = rglDirection;
		eff.lpEnvelope = 0;
		eff.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
		eff.lpvTypeSpecificParams = &cf;

		// create the prepared effect
		hr = handler->fDevice[handler->fNumDevices]->fDevice->
			CreateEffect(GUID_ConstantForce,
				&eff,
				&handler->fDevice[handler->fNumDevices]->fEffect,
				NULL );

//		assert(hr == DI_OK);  // Removed ph 10/11/98 

		/*
		if (hr != DI_OK)
		{
			int error = -1;
			switch(hr)
			{
			case DIERR_DEVICENOTREG:
				error = 1;
				break;
			case DIERR_DEVICEFULL:
				error = 2;
				break;
			case DIERR_INVALIDPARAM:
				error = 3;
				break;
			case DIERR_NOTINITIALIZED:
				error = 4;
				break;
			default:
				assert(false);
			}
			assert(hr == DI_OK);
		}
		*/
	}

	// Fill in the buffers for later
	handler->fDevice[handler->fNumDevices]->fBufSize = sizeof(DIJOYSTATE);
	handler->fDevice[handler->fNumDevices]->fBuffer = new char[sizeof(DIJOYSTATE)];
	assert (handler->fDevice[handler->fNumDevices]->fBuffer != NULL);

	handler->fNumDevices++;	  // Succesfully got one
	return DIENUM_CONTINUE;	
}


void TDIHandler::GrabMouse()
{
	HRESULT hr; 
	if (fMouse != NULL)
	{
		hr = fMouse->Acquire();
	}
}

void TDIHandler::ReleaseMouse()
{
	HRESULT hr; 
	if (fMouse != NULL)
	{
		hr = fMouse->Unacquire();
	}
}
 

void TDIHandler::InitMouse()
{
	assert (fMouse == NULL);

	HRESULT hResult;

#if DIRECTINPUT_VERSION >= DIRECTINPUT8_VERSION
	// (DirectX8) Create Mouse Device
	#if D3D_SDK_VERSION == DX_BETA_WEEKLY2
		hResult = fDI->CreateDevice(	GUID_SysMouse,
										&fMouse,
										NULL );
	#else
		hResult = fDI->CreateDeviceEx(	GUID_SysMouse,
										IID_IDirectInputDevice8,
										(LPVOID*) &fMouse,
										NULL );
	#endif
#else
	// (Previous versions) Create Mouse Device
	hResult = fDI->CreateDevice(	GUID_SysMouse,
									&fMouse,
									NULL);
#endif
	assert (SUCCEEDED(hResult));
	assert (fMouse != NULL);

	// set data format
	hResult = fMouse->SetDataFormat( &c_dfDIMouse );
	assert (SUCCEEDED(hResult));

	// set cooperative level
	hResult = fMouse->SetCooperativeLevel( ghWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND ); 
	assert (SUCCEEDED(hResult));
}
