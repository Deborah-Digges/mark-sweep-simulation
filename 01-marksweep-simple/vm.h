#ifndef VM_H_ 
#define VM_H_

#include <deque>
#include <queue>

/* An object allocated can be either an integer(atom) or a pair */
typedef enum
{
	OBJ_ATOM,
	OBJ_PAIR
} ObjectType;

/* Every object allocated by the VM belongs to this class */
struct Object
{
	/* A discriminator indicating the kind of object being stored */
	ObjectType type;

	/* Used by the mark procedure in marking reachable objects*/
	int marked;

	/* The next object in the linked list of heap allocated objects */
	Object* next;

	/* A given object can either be an atom or a pair */
	union
	{
		/* OBJ_ATOM */
		int value;

		/* OBJ_PAIR */
		struct 
		{
			Object* left;
			Object* right;
		};
	};

};

class VM
{
	public:
		explicit VM(int maxObjects=10, int heapSize=100);

		~VM();

		/* Removes and returns the top-most object on the stack */		
		Object* pop();

		/* Creates an object representing an integer on the stack */
		Object* pushAtom(int intValue);

		/* Pops two elements off the stack, with pointers to them stored
		*  in an object which is then pushed onto the stack */
		Object* pushPair();

		/* Returns the number of objects currently allocated on the VM 
		*  which includes both reachable and unreachable objects
		*/
		int getNumberAllocated();

		void gc();

	private:
		/* The VM's exection stack */
		std::deque<Object*> stack;

		/* A pointer to the link list of all objects ever allocated */
		Object * allocated;

		/* The memory from which the VM allocates objects */
		std::queue<Object*> freeList;

		/* The total number of objects currently allocated */
		int numObjects;

		/* The number of objects required to trigger a GC */
		int maxObjects;

		/* The maximum number of blocks available for allocation */
		int heapSize;

		/* Creates the free list from which the VM allocates objects */
		void buildFreeList();

		/* Releases the VM's memory just before the VM is destroyed */
		void destroyFreeList();

		/* Pops all active objects off the VM's stack*/
		void clearStack();

		/* Returns the next free block on the freeList */
		Object* allocateCell();

		/* Creates an object whose type is specified by the caller */
		Object* createObject(ObjectType type);

		/* Check if the number of allocated objects has reached 
		*  the permissible threshold -- if so call GC */
		void checkAllocationThreshold();

		/* --- GC ROUTINES --- */

		void mark(Object* object);

		void markAll();

		void sweep();

	
};

#endif
