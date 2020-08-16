/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
#ifndef __TEST1_H__
#define __TEST1_H__

#include "_RO_Test1.h"

class Test1 : _RO_DO_PUBLIC_RO(Test1)
{
public:
	Test1();
	virtual ~Test1();

	void OwnerChange(void);
	void ReportTest1();

private:
_RO_DO_ALLOW_FRIEND_RO(Test1)
};

#endif
