#include <cstdlib>
#include <cstdio>
#include <deque>
#include <iostream>
#include <stack>

#include "vm.h"
#include "tests.h"


VM::VM(int maxObjects, int heapSize)
{
	allocated = NULL;

	numObjects = 0;
	this->maxObjects = maxObjects;
	this->heapSize = heapSize;

	buildFreeList();
}


VM::~VM()
{
	clearStack();
	gc();
	destroyFreeList();
}


Object* VM::pushAtom(int intValue)
{
	Object* object = createObject(OBJ_ATOM);
	object->value = intValue;
	stack.push_back(object);
	checkAllocationThreshold();
	return object;
}


Object* VM::pushPair()
{
	assert(numObjects >= 2, "Need atleast 2 objects to make a pair");
	Object* object = createObject(OBJ_PAIR);
	object->right = stack.back(); stack.pop_back();
	object->left = stack.back();  stack.pop_back();
	stack.push_back(object);
	checkAllocationThreshold();
	return object;
}


Object* VM::pop()
{
	assert(stack.size() > 0, "No variables on the VM's stack");
	Object* returnObj = stack.back();
	stack.pop_back();
	checkAllocationThreshold();
	return returnObj;
}


void VM::gc()
{
	markAll();
	sweep();
	//std::cout << "Collected:" << beforeGC - numObjects << ", Remaining:" << numObjects << std::endl;
}


int VM::getNumberAllocated()
{
	return numObjects;
}


void VM::buildFreeList()
{
	for (int i = 0; i < heapSize; ++i)
	{
		freeList.push((Object*) malloc(sizeof(Object)));  
	}	
}


void VM::destroyFreeList()
{
	for (int i = 0; i < heapSize; ++i)
	{
		free(freeList.front());
		freeList.pop();  
	}	
}


void VM::clearStack()
{
	while(!stack.empty())
	{
		stack.pop_back();
	}
}


Object* VM::allocateCell()
{
	Object* object = NULL;

	/* Get the next available object on the free list */
	object = freeList.front();
	freeList.pop();

	return object;
}


Object* VM::createObject(ObjectType type)
{
	Object* object = NULL;

	if(freeList.size() > 0)
	{
		object = allocateCell();
	}
	else
	{
		gc();
		assert(freeList.size() > 0, "Memory Exhausted");
		object = allocateCell();
	}
	object->type = type;
	object->marked = 0;

	/* Add the object to the head of the list of allocated objects */
  	object->next = allocated;
	allocated = object;

	numObjects++;

  	return object;
}


void VM::checkAllocationThreshold()
{
	if(numObjects == maxObjects)
	{
		gc();
	}
}


void VM::markAll()
{
	for (int i = 0; i < stack.size(); ++i)
	{
		mark(stack.at(i));		
	}
}


void VM::mark(Object* object)
{
	if(object -> marked)
	{
		return;
	}

	object->marked = 1;

	if(object->type == OBJ_PAIR)
	{
		mark(object->left);
		mark(object->right);
	}	
}


void VM::sweep()
{
	/* Go through all objects ever allocated */
	Object** object = &allocated;
	while (*object) 
	{
		if (!(*object)->marked)
		{
			/* This object wasn't reached, so remove it from the list and free it. */
			/* from the list of objects allocated by the VM */
			Object* unreached = *object;
			*object = unreached->next;

			/* Add it to the free list -- "Collect it" */
			/* This block can now be used for future allocations */
			freeList.push(unreached);

			numObjects--;
		}
	    else
	    {
	      /* This object was reached, so unmark it (for the next GC) and move on to
	       the next. */
			(*object)->marked = 0;
			object = &(*object)->next;
	    }
	}
}





