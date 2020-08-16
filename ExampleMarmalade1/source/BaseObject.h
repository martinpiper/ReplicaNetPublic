#ifndef _BASEOBJECT_H_
#define _BASEOBJECT_H_

/**
 * A typical type of game base object with several virtual access members
 */
class BaseObject
{
public:
	BaseObject();
	virtual ~BaseObject();

	virtual void Tick(float timeDelta);
	virtual void Render(void);
};

#endif
