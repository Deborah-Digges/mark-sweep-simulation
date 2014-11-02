#include <cstdlib>
#include <iostream>

#include "vm.h"


void assert(int condition, const char* message)
{
	if(!condition)
	{
		std::cout << message << std::endl;
		exit(1);
	}
} 


void testPreservesNonGarbage()
{
	VM* vm = new VM();

	/* Create two integer "variables" on the VM's execution stack*/
	vm->pushAtom(1);
	vm->pushAtom(2);
	
	/* Explicitly invoke the GC -- nothing is collected */
	/* because everything is reachable */
	
	assert(vm->getNumberAllocated() == 2, "Push failed");
	vm->gc();
	assert(vm->getNumberAllocated() == 2, "GC failed");

	delete vm;
}


void testCollectsGarbage()
{
	VM* vm = new VM();

	/* Two Integers are created from the available blocks of memory */
	/* As long as pointers to these objects are present on the 
	   VM's stack, these objects are reachable */
	/* When the objects are popped from the VM's stack, they become garbage*/
	/* Our definition of garbage - objects present in the VM's list of allocated
	   objects that are *not* reachable from the VM's stack */
	/* These are the objects that will remain unmarked during the mark phase 
	   And will be collected during the sweep phase */

	vm->pushAtom(1);
	vm->pushAtom(2);
	vm->pop();
	vm->pop();

	assert(vm->getNumberAllocated() == 2, "Push/Pop failed");
	vm->gc();
	assert(vm->getNumberAllocated() == 0, "GC failed");

	delete vm;
}


void testMarksNestedReachable()
{
	VM* vm = new VM();

	/*	
	*	pointer to pair  => present on the stack
	*	/		\
	*	pair 		pair
	*	/  \		/  \		
	*      1   2           3   4
	*
	*/


	vm->pushAtom(1);
	vm->pushAtom(2);
	vm->pushPair();
	vm->pushAtom(3);
	vm->pushAtom(4);
	vm->pushPair();
	vm->pushPair();
	
	/* Nothing is collected as all objects are reachable starting from the roots */
	
	assert(vm->getNumberAllocated() == 7, "Push(Atom/Int) Failed");	
  	vm->gc();
	assert(vm->getNumberAllocated() == 7, "GC failed");

  	delete vm;
}


void testCollectsNestedUnreachable()
{
	VM* vm = new VM();

	/*	
	*	pointer to pair => present on the stack
	*	/	   \
	*	pair       pair
	*	/  \	   /  \		
	       1   2      3   4
	*
	*/

	vm->pushAtom(1);
	vm->pushAtom(2);
	vm->pushPair();
	vm->pushAtom(3);
	vm->pushAtom(4);
	vm->pushPair();
	vm->pushPair();

	/* This detaches the entire structure from the VM's stack */
	/* 7 unreachable objects are created */
	vm->pop();
	
	assert(vm->getNumberAllocated() == 7, "Push/Pop failed");
	vm->gc();
	assert(vm->getNumberAllocated() == 0, "GC failed");
	delete vm;
}


void testCollectsCycles()
{
	VM* vm = new VM();
	
	vm->pushAtom(1);
	vm->pushAtom(2);
	Object* a = vm->pushPair();
	vm->pushAtom(3);
	vm->pushAtom(4);
	Object* b = vm->pushPair();

	a->right = b;
	b->right = a;
	vm->pushPair();

	/* Detatch the entire structure from the stack */
	vm->pop();
	
	assert(vm->getNumberAllocated() == 7, "Push/Pop Failed");
	vm->gc();
	assert(vm->getNumberAllocated() == 0, "GC Failed");

	delete vm;
}


void testThresholdGCInvocation()
{
	int threshold = 5;

	VM* vm = new VM(threshold);
	
	for(int i=1; i <= threshold;  ++i)	
	{	
		vm->pushAtom(i);
		vm->pop();
	}
 	
	/* During the allocation of the 5th object
	*  The 5th object's creation invokes GC
	*  There are *4* unreachable objects at this time
	*  These 4 objects are collected by the GC, leaving behind 1 object
	*  The GC is *implicitly* invoked at this time */
	assert(vm->getNumberAllocated() == 1, "GC failed");
	delete vm;
}

void testMemoryExhaustion()
{
	int heapSize = 5;
	int threshold = 5;
	VM* vm = new VM(threshold, heapSize);
	
	for(int i=1; i <= heapSize; ++i)
	{
		vm->pushAtom(i);
	}
	
	assert(vm->getNumberAllocated() == heapSize, "Push failed");
		
	/* There are *heapSize* allocated objects all of which are reachable 
	*  Another allocation will try to free memory by invoking the GC 
	*  The GC fails to release memory as everything is reachable
	*  The allocation fails with an out of memory error
	*/
	vm->pushAtom(1);	
	
	delete vm;
		
}


void runTests()
{
	testPreservesNonGarbage();
	testCollectsGarbage();
	testMarksNestedReachable();
	testCollectsNestedUnreachable();
	testCollectsCycles();
	testThresholdGCInvocation();
	testMemoryExhaustion();
}
