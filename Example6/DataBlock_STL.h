/* START_LICENSE_HEADER

Copyright (C) 2000 Martin Piper, original design and program code
Copyright (C) 2001 Replica Software

This program file is copyright (C) Replica Software and can only be used under license.
For more information visit: http://www.replicanet.com/
Or email: info@replicanet.com

END_LICENSE_HEADER */
#ifndef __DATABLOCK_STL_H__
#define __DATABLOCK_STL_H__

#include "DataBlock_GenericSTLVirtual.h"

/**
 * This DataBlock derived class scans the registered area for changes and if needed creates a packet detailing those changes<br>
 * This DataBlock does not do any lag compensation or prediction. It just informs each replica of a change when one is made.
 */
template <class T> class DataBlock_STL : public DataBlock_GenericSTLVirtual
{
public:
	/**
	 * The ctor performs some basic initialisation
	 */
	DataBlock_STL(){}

	/**
	 * The dtor makes sure everything is tidy
	 */
	virtual ~DataBlock_STL(){}

	/**
	 * This registers the template class area with this data block type and returns a new pointer
	 * \param data the start of the memory to check
	 * \return Returns a pointer that can be used to attach to a ReplicaObject
	 */
	static DataBlock_STL *Register(T *const data)
	{
		DataBlock_STL<T> *object = new DataBlock_STL<T>();

		object->mData = data;

		return object;
	}

	/**
	 * This registers the memory area with this data block type. This is used when the memory is in datablocks that are allocated as part of a bigger class.
	 * \param data the start of the memory to check
	 * \param size the size of the data block
	 */
	void RegisterInClassPointer(T *const data)
	{
		mData = (void *) data;
	}

	bool IsAttached(T *const data)
	{
		if (!data)
		{
			return false;
		}
		if (mData == data)
		{
			return true;
		}
		return false;
	}

protected:
	/**
	 * This gets the number of elements in the STL container
	 * \return The number of elements in the STL container
	 */
	size_t GetNumberOfElements(void)
	{
		T *theData = (T *) mData;
		return (int) theData->size();
	}

	void SetNumberOfElements(const size_t numElements)
	{
		T *theData = (T *) mData;
		theData->resize(numElements);
	}

	/**
	 * Gets the element size
	 * \return The element size
	 */
	size_t GetElementSize(void) const
	{
		return sizeof(T::value_type);
	}

	void *GetElementPointerAt(const size_t offset)
	{
		int copyOffset = offset;
		T::iterator st,en;
		T *theData = (T *) mData;

		st = theData->begin();
		en = theData->end();

		// Count up...
		while (copyOffset > 0)
		{
			st++;
			copyOffset--;
		}

		T::value_type *data = &(*st);

		return data;
	}

	void SetElementDataAt(const void *data,const size_t offset)
	{
		T *theData = (T *) mData;

		T::value_type tempData;
		tempData = ((T::value_type *)data)[0];

		((T::value_type *)GetElementPointerAt(offset))[0] = tempData;
	}

private:
};

/* _RO_DO_REGISTERBLOCK_STL_VAR macro starts here */
#define _RO_DO_REGISTERBLOCK_STL_VAR(x,y)	\
	{\
		DataBlock_STL< y > *datablock = 0;	\
		datablock = DataBlock_STL< y >::Register(&(basepoint->x));	\
		_RO_DO_SETDATABLOCKVARIABLENAME(x);
/* _RO_DO_REGISTERBLOCK_NDATA_VAR macro ends here */

#endif
