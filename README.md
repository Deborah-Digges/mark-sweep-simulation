## Simulation of A Mark Sweep Garbage Collector

A high level implementation of a mark-sweep garbage collector.

A model of a *stack based VM* is created where the stack holds references to all objects ever allocated. A *freeList* of memory is maintained from which all allocations are made. 

The parameters of the VM that can be configured are:

1. The threshold for GC invocation - The minimum number of objects needed to trigger a GC.
2. The heap size - The number of blocks available to the VM for allocation initially.

These parameters can be configured via the VM's constructor *VM(threshold, heapSize)*.
The VM supports interfaces to push and pop objects from the stack. 

The *reachable* objects are those which, starting from those on the stack, can be traced by following references. All unreachable objects are deemed *garbage* and are collected by the GC upon it's next invocation. Once an object is popped from the VM's stack it is unreachable and becomes garbage.

## GC Invocation

The GC is invoked in one of two situations:

1. The number of objects allocated has reached the threshold for GC invocation.
2. The heap is exhausted which causes the GC to be invoked in an attempt to free memory for a new allocation.


## Directory Structure
* 01 - Mark phase is made recursive and uses the [implicit stack of recursion for finding reachable objects](http://github.com/Deborah-Digges/mark-sweep-simulation/blob/master/01-marksweep-simple/vm.cpp/#L151-174)
* 02 - Mark phase uses an [explicit stack for finding reachable objects](http://github.com/Deborah-Digges/mark-sweep-simulation/blob/master/02-marksweep-Explicit-Stack/vm.cpp/#L151-192)
* 03 - Mark phase uses the Deutsch-Schorr-Waite Pointer Reversal algorithm which [traverses the data structure without using an unbounded amount of linear space](http://github.com/Deborah-Digges/mark-sweep-simulation/blob/master/03-marksweep-Pointer-Reversal/vm.cpp/#L152-205). This is possible because the algorithm keeps track of paths it has already visited within the nodes of the data structure it is traversing! Essentially, we are traversing the data structure using the data strure itself.


##Build & Run

*make*<br/>
*./marksweep*





