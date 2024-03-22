#pragma once
typedef unsigned int U32;
class StackAllocator
{
public:

	//Stack marker: Represent the current top of the stack
	//You can only roll back to a marker
	//not to an arbitrary locations within the stack
	typedef U32 Marker;

	//Construct a stack allocator with the given total size
	explicit StackAllocator(U32 stackSize_bytes);

	//Allocate a new block of memory of the given size from the stack top
	void* alloc(U32 size_bytes);

	//Returns the marker to the current top of the stack
	Marker getMarker();

	//Roll back the stack to athe previous marker
	void freeToMarker(Marker marker);

	//Clear the stack
	void clear();
};

