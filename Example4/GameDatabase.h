/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
#ifndef _GAMEDATABASE_H_
#define _GAMEDATABASE_H_

#include <vector>

// These classes are accessed via their pointers
class GameObject;

/**
 * A collection of variables used by the example application
 */
class GameDatabase
{
public:
	static std::vector<GameObject *> mGameObjects;
	static float mGameFrameTick;

private:
	GameDatabase(){};			// Not intended for allocation, ever. ;)
};

#endif
