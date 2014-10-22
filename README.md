## Mark Sweep Garbage Collector

A high level implementation of a mark-sweep garbage collector.

A model of a *stack based VM* is created where the stack holds references to all objects ever allocated. A *freeList* of memory is maintained from which all allocations are made. 

The parameters of the VM that can be configured are:
<ol>
<li>The threshold for GC invocation - The minimum number of objects needed to trigger a GC.</li>
<li> The heap size - The number of blocks available to the VM for allocation initially.</li>
</ol>
These parameters can be configured via the VM's constructor *VM(threshold, heapSize)*.
The VM supports interfaces to push and pop objects from the stack. 

The *reachable* objects are those which, starting from those on the stack, can be traced by following references. All unreachable objects are deemed *garbage* and are collected by the GC upon it's next invocation. Once an object is popped from the VM's stack it is unreachable and becomes garabge.

##GC Invocation

The GC is invoked in one of two situations:
<ol>
<li> The number of objects allocated has reached the threshold for GC invocation.</li>
<li> The heap is exhausted which causes the GC to be invoked in an attempt to free memory for a new allocation.</li>
</ol>

##Build & Run

*make*<br/>
*./marksweep*





