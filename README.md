# CMSC_341_Llama_Stack_Project
This project implements a data structure called a Linked-list Array Mixed Amalgamated Stack containing dynamically allocated nodes. Details are provided in the prompt below. The objective of this programming assignment is to review C++ programming using following features: object-oriented design, dynamic memory allocation, pointer manipulation, exceptions and templates. Driver.cpp is the primary test file for the project. driver1.cpp tests the llama stack structure with strings, driver2.cpp tests it with float numbers, and driver3.cpp tests the stack with an arbitrary data type called OvCoInt (for overly complicated integer class). To compile one of the numbered driver files with an external class like driver3.cpp using the OvCoInt class, change the optional EXT_* contansts in the Makefile.

# Assignment Prompt:
The premise of this assignment is that you are using a system where calls to new and delete for dynamic memory allocation and de-allocation are very slow. They are slow enough that it is worth your while to consider data structures that reduce the number of calls to new and delete.

The data structure we are considering is a stack. This can be implemented as an array or as a linked list with advantages and disadvantages to both. However, your application does not allow you to determine the maximum size of the stack. If you implemented the stack using an array, you may run out of storage if the stack exceeds the size of the array. You can dynamically allocate a bigger array and copy the old stack, but the time for copying is prohibitively slow. Using a standard linked list would involve a call to new or delete for each push or pop instruction. This is also too slow.

Instead of a simple linked list, we will use a linked list where each node contains an array of items. The array will be of fixed size, say for example, 100. This linked list will start out with a single node that holds an array of 100 items. So, a single node can hold up to 100 items of the stack. That means we can push items on the stack and pop items off the stack without calling new or delete until the stack grows larger than 100. If that happens, we will add a node to the linked list. The result is a stack that holds 200 items. Adding space for 100 more items involved just one call to new.

There's a catch. Suppose our stack has 101 items and we do a pop. Now we only have 100 items, what do we do with the linked list? We can remove and deallocate the node that used to hold items 101 through 200. If we do that, what happens if we push right after the pop? We would have to add a node to the linked list again. If there's a sequence of push and pop instructions that causes the stack size to flip between 100 and 101, then we are left with the situation that every push and pop instruction results in a call to new or delete. Then there would be no advantage to our new data structure.

The solution is that we should not immediately remove a node when our stack size drops from 101 to 100. Instead, we should wait until the stack size drops much further, until it drops to 50 items (which requires at least 50 pop instructions to achieve). When the stack size is between 100 and 51, we will keep the "extra" node around in case the stack size grows to 101 again. Then we can use the space in the extra node and bypass the need to call new.

To recap, if our stack size reaches 101 for the first time, we add a node to the linked list. The stack size can increase and decrease, but we do not deallocate a node until the stack size drops to 50. Also, we won't have to allocate a new node until the stack size increases past 200. In general, we follow this scheme whenever a node is filled --- i.e., when the stack size grows past 200, 300, 400, ... This strategy prevents us from having to call new or delete very often. It also allows our stack to grow and shrink without having to copy the entire stack to a new location.

Let's call this data structure a Llama Stack (short for Linked-List Array Mixed Amalgamated Stack).

