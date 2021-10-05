
#pragma once

#include "SharedObject.h"

template<typename T>
class TSharedClass
{
public: // Typedefs

	typedef T ChildType;
	
public: // Constructors

	TSharedClass()
	{}

private: // Fields

	TWeakPtr<ChildType> Object;
	
};