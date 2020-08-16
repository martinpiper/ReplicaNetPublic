/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
#ifndef _DIInterface_H_
#define _DIInterface_H_

// buttons...

#define DIRECTINPUT8_VERSION 0x0701
#define DIRECTINPUT8_VERSION_BETA1 0x70e
#define DIRECTINPUT8_VERSION_WEEKLY2 0x70f

#define JOYSTICK_LEFT			0x00000001
#define JOYSTICK_RIGHT			0x00000002
#define JOYSTICK_UP				0x00000004
#define JOYSTICK_DOWN			0x00000008

#define JOYSTICK_ACCELERATE		0x00000010
#define JOYSTICK_BRAKE			0x00000020

#define JOYSTICK_MAP			0x00000040
#define JOYSTICK_PAUSE			0x00000080
#define JOYSTICK_USE1			0x00000100	  // Handbrake
#define JOYSTICK_USE2			0x00000200	  // Reset
#define JOYSTICK_JUMP			0x00000400
#define JOYSTICK_HORN			0x00000800
#define JOYSTICK_USE3			0x00001000	  // Weapon1
#define JOYSTICK_PULL_CAMERA	0x00002000

#define JOYSTICK_W				0x00004000
#define JOYSTICK_A				0x00008000
#define JOYSTICK_S				0x00010000
#define JOYSTICK_D				0x00020000


#define MAX_NUM_DEVICES 6


class TDIDevice;

class TJoystick 
{
public:
	TJoystick() {Clear();}

	DWORD fButtons;
	LONG  fstickX, fstickY;
	LONG  frightstickX, frightstickY;
	bool  bAnalogX, bAnalogY;

	void Clear() { fstickX = 0; fstickY = 0; fButtons = 0; frightstickX = 0; frightstickY = 0;}

	TDIDevice *fDIDevice; // TDIDevice this belongs to
};

class TDIDevice
{ 
	public:
		TDIDevice(void);
		~TDIDevice(void);

		void Read();
		void Acquire(int acquire); // true = acquire, false = unacquire

		virtual void ReadInputs()  = 0;


#if DIRECTINPUT_VERSION < DIRECTINPUT8_VERSION
		LPDIRECTINPUTDEVICE2 fDevice;
#else
		LPDIRECTINPUTDEVICE8 fDevice;
#endif
		IDirectInputEffect*  fEffect;
		int fBufSize;
		void* fBuffer;

		TJoystick fJoystick;


	void SetForce(float x, float y);
};



class TDIKeyboard : public TDIDevice
{
public:
	int  fDeBounce[256];

	void InitialiseKeyboard(REFGUID guid,LPCDIDATAFORMAT dataFormat,int bufferSize);
	virtual void ReadInputs();
};

class TDIJoystick : public TDIDevice
{
public:
	virtual void ReadInputs();
};



// scheduled object that reads the input devices
class TDIHandler 
{
	public:
		TDIHandler(void);
		virtual ~TDIHandler(void);

		void Initialise(void);

#if DIRECTINPUT_VERSION >= DIRECTINPUT8_VERSION
		LPDIRECTINPUT8 fDI;
#else
		LPDIRECTINPUT fDI;
#endif

		BOOL ReacquireJoystick(void);

		void ReadAllInputs();

		int		 fNumDevices;
		TDIDevice*  fDevice[MAX_NUM_DEVICES];

		TDIKeyboard* fDISysKeyboard;  // Points to last used entry in the above array,
									  // this is always the keyboard

		int fNextJoystick;

		TJoystick* GetJoystick(int theIndex);
		TJoystick* GetNextJoystick();
		void ResetJoystickAllocation();

#if DIRECTINPUT_VERSION >= DIRECTINPUT8_VERSION
		LPDIRECTINPUTDEVICE8 fMouse; 
#else
		LPDIRECTINPUTDEVICE	fMouse;
#endif
		bool fGrabbed;

		void ReleaseMouse();
		void GrabMouse();
		void InitMouse();


};

extern TDIHandler* gDIHandler;		  // All inputs live in here


inline int SysKeyDown(int key)
{
	assert (gDIHandler->fDISysKeyboard->fBuffer);
	return (int)((char*)gDIHandler->fDISysKeyboard->fBuffer)[key];
}


inline int SysKeyDownOnce(int key)
{
	assert (gDIHandler->fDISysKeyboard->fBuffer);
	bool down = false;
	if ((gDIHandler->fDISysKeyboard->fDeBounce[key] == 0) && (((char*)gDIHandler->fDISysKeyboard->fBuffer)[key] != 0))
	{
		down = true;
	}

	gDIHandler->fDISysKeyboard->fDeBounce[key] = (int)((char*)gDIHandler->fDISysKeyboard->fBuffer)[key];

	return down;
}

// Enumeration callback
BOOL PASCAL InitialiseJoystickEnum(LPCDIDEVICEINSTANCE pdinst, LPVOID pvRef);
BOOL PASCAL JoystickEffectsEnum(LPCDIEFFECTINFO pdei, LPVOID pvRef);

extern TJoystick		*joyStick;

#endif
