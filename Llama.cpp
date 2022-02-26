
/******************************************************************************
 * File:    Llama.cpp
 * Project: CMSC 341 Project 1, Fall 2016
 * Author:  Nicholas Stommel
 * Date:    10/04/16
 * Section: 05
 * E-mail:  stommel1@gl.umbc.edu
 * 
 * Description: This is the source file containing the implementation of 
 * the Llama class.
 * 
 ******************************************************************************/

#ifndef _LLAMA_CPP_
#define _LLAMA_CPP_

#include <iostream>
#include <string>
#include "LlamaNode.h"
#include "Llama.h"

using namespace std;

template<class T, int LN_SIZE>
Llama<T,LN_SIZE>::Llama() : m_size(0), m_nodeCount(1), m_hasExtraNode(false)
{
	// Throw exception if LN_SIZE of zero is attempted to avoid unexpected behavior.
	if(LN_SIZE != 0) {
		m_topNode = new LlamaNode<T,LN_SIZE>;
	} else {
		throw LlamaUnderflow("Llama constructor ERROR: cannot store data with node size of zero.");
	}
}

template<class T, int LN_SIZE>
Llama<T,LN_SIZE>::Llama(const Llama<T,LN_SIZE>& other) {
	
	// Assign default values to initial empty Llama container
	m_size = 0;
	m_nodeCount = 1;
	m_hasExtraNode = false;
	m_topNode = new LlamaNode<T,LN_SIZE>;
	
	// Use the push() and peek() functions in a short, elegant manner to copy
	// and add necessary data elements. Note that the top-down order of the list
	// makes push extremely efficient for this operation. peek() is written such that
	// only m_numNodes need to be traversed to retrieve data at worst case, 
	// which is usually fractionally smaller for Llama lists with large node sizes 
	// and/or Llama lists with a relatively small number of nodes. 
	//
	// Note that other methods such as creating a copy of the other list and using 
	// the pop() function are many times more inefficient.
	// This solution was deemed the most reasonable, as manually filling by accessing 
	// each array from bottom-to-top involves using much more code with decreased
	// readability and a fairly similar cost. The good stability of the push and 
	// associated peek methods was verified after repeatedly invoking this copy 
	// constructor, tweaking code, and testing for memory leaks.
	for(int i = other.m_size - 1; i >= 0; --i) {
		push(other.getConstRef(i));
	}
	
	m_hasExtraNode = other.m_hasExtraNode;
	// If an extra node is present in other list, add extra node here as well
	if(m_hasExtraNode) {
		LlamaNode<T,LN_SIZE> *extraNode = new LlamaNode<T,LN_SIZE>;
		extraNode->m_next = m_topNode;
		m_topNode = extraNode;
	}
}

template<class T, int LN_SIZE>
Llama<T,LN_SIZE>::~Llama() {
	
	// Create temp LlamaNode pointers for traversing the nodes
	LlamaNode<T,LN_SIZE> *current, *next;
	
	// Traverse linked list and delete nodes from top to bottom of
	// stack
    current = m_topNode;
	while(current != NULL) {
		//cout << "Deleting node " << current << endl;
		next = current->m_next;
		delete current;
		current = next;
	}
}

template<class T, int LN_SIZE>
int Llama<T,LN_SIZE>::size() {
	return m_size;
}

template<class T, int LN_SIZE>
void Llama<T,LN_SIZE>::dump() {
	
	cerr << endl << "***** Llama Stack Dump ******" << endl;
	cerr << "LN_SIZE = " << LN_SIZE << endl;
	cerr << "# of items in the stack = " << m_size << endl;
	cerr << "# of nodes in the stack = " << m_nodeCount << endl;
	cerr << "# of nodes created   (total for same type) = " << LlamaNode<T, LN_SIZE>::newCount << endl;
	cerr << "# of nodes destroyed (total for same type) = " << LlamaNode<T, LN_SIZE>::deleteCount << endl;
	cerr << endl;
	
	if(m_hasExtraNode) {
		cerr << "This stack has an extra node: " << m_topNode << endl << endl;
	} else {
		cerr << "This stack does not have an extra node." << endl << endl;
	}

	cerr << "Stack contents, top to bottom" << endl << endl;
	
	
	// NOTE: no other member functions are used in dump, both to avoid the large
	// overhead of copying an entire Llama object and for debugging purposes
	
	LlamaNode<T,LN_SIZE>* currentNode = m_topNode;
	
	// If there is an extra node, skip to the next node.
	if(m_hasExtraNode) {
		currentNode = m_topNode->m_next;
	}
	
	if(m_size > 0) {
		
		// If top node is partially full, handle specially to avoid accessing memory
		// that has not yet been filled with data
		if(m_size % LN_SIZE != 0) {
			cerr << "----- " << currentNode << " -----" << endl;
			//cerr << "Starting index: " << (LN_SIZE - (m_size % LN_SIZE)) << endl;
			for(int i = (LN_SIZE - (m_size % LN_SIZE)); i < LN_SIZE; ++i) {
				cerr << currentNode->arr[i] << endl;
			}
			
			// Advance a node, if one does not exist, currentNode will be set to NULL
			// and no further printing will happen below
			currentNode = currentNode->m_next;
		}
		
		// Print remainder of the stack by node
		while (currentNode != NULL) {
			cerr << "----- " << currentNode << " -----" << endl;
			
			for(int i = 0; i < LN_SIZE; ++i) {
				cerr << currentNode->arr[i] << endl;
			}
			currentNode = currentNode->m_next;
		} 
	
	} else {
		
		cerr << "!!! Stack is empty !!!" << endl;
		cerr << "----- " << m_topNode << " -----" << endl;
	}
	
	cerr << "----- bottom of stack ----" << endl << endl;
	cerr << "*****************************" << endl;
	
}

template<class T, int LN_SIZE>
void Llama<T,LN_SIZE>::push(const T& data) {
	
	const int END_INDEX = LN_SIZE - 1;
	
	// If all nodes are full, either add to end of extra node or link new node
	if((m_size > 0) && (m_size % LN_SIZE == 0)) {
		
		// If the stack has an extra node and all others are full,
		// add data to end of array in extra node and un-flag the m_hasExtraNode boolean.
		if(m_hasExtraNode) {
			
			m_topNode->arr[END_INDEX] = data;
			m_hasExtraNode = false;
			
		// If an extra node is not present, allocate memory for new node and push data
		// back to last index of storage array
		} else {

			LlamaNode<T,LN_SIZE> *newNode = new LlamaNode<T,LN_SIZE>;
			
			++m_nodeCount;
			
			// Link new node to top of stack
			newNode->m_next = m_topNode;
			m_topNode = newNode;
			// Add stack data to very end of data array in new Node
			m_topNode->arr[END_INDEX] = data;
		}
		
	// Otherwise, if there is space in the topmost node, push back data to 
	// last available index.
	} else {
		
		LlamaNode<T,LN_SIZE>* currentNode = m_topNode;
		
		// If there is an extra node, skip it and set the current node to the next one.
		if(m_hasExtraNode) {
			currentNode = m_topNode->m_next;
		} 
		
		currentNode->arr[END_INDEX - (m_size % LN_SIZE)] = data;
	}
	
	++m_size;
}

template<class T, int LN_SIZE>
T Llama<T,LN_SIZE>::pop() {
	if(m_size == 0) {
		throw LlamaUnderflow("pop() ERROR: empty stack!");
	}
	--m_size;
	
	// Calculate index of top element in node
	int topIndex = (LN_SIZE - 1) - (m_size % LN_SIZE);
	
	T* data;
	
	// If stack has extra node, skip the extra node and return data or delete the extra
	// node and return data if the number of elements in the node reaches half or less.
	// A data pointer is used to avoid expensive copy operation on data within function
	// and for a cleaner, single return statement.
	if(m_hasExtraNode) {
		
		// Save local reference to data in case extra node is deleted
		data = &(m_topNode->m_next->arr[topIndex]);
		
		if(m_size % LN_SIZE == LN_SIZE / 2) {
			
			LlamaNode<T,LN_SIZE>* tempPtr;
			tempPtr = m_topNode->m_next;
			delete m_topNode;
			--m_nodeCount;
			m_topNode = tempPtr;
			
			// Specially account for lists with an LN_SIZE of one
			// so that popping is performed correctly.
			if(LN_SIZE == 1 && m_nodeCount > 1) {
				m_hasExtraNode = true;
			} else {
				m_hasExtraNode = false;
			}
		}

	} else {
		
		// If node storage size reaches zero, mark that node as extra
		if(m_size % LN_SIZE == 0 && m_nodeCount > 1) {
			m_hasExtraNode = true;
		}
		
		data = &(m_topNode->arr[topIndex]);
	}
	
	return *data;
}

template<class T, int LN_SIZE>
void Llama<T,LN_SIZE>::dup() {
	
	if(m_size == 0) {
		throw LlamaUnderflow("dup() ERROR: stack is empty!");
	} 
	
	// Single operation used to first peek at the very top of the stack (at most two
	// steps because of a possible extra node) and then push that value back onto the
	// top once more. Makes use of existing functions following the DRY principle.
	push(getConstRef(0));
}

template<class T, int LN_SIZE>
void Llama<T,LN_SIZE>::swap() {

	if(m_size < 2) {
		throw LlamaUnderflow("swap() ERROR: stack too small!");
	}
	
	// Perform swap, first saving temporary value of item on top of stack.
	// 
	// Using peek()'s related methods getRef and getConstRef involves very few steps,
	// as the required data is for most every case on the top one or two nodes,
	// which is easily accessible from the m_topNode pointer.
	// Using these functions, which reliably account for the use of an extra node,
	// makes this implementation far more understandable and particularly useful
	// for when data is split across nodes. Also see rot() for a 
	// slightly more complex example of this logic.
	T temp = getConstRef(0);
	getRef(0) = getConstRef(1);
	getRef(1) = temp;
}

template<class T, int LN_SIZE>
void Llama<T,LN_SIZE>::rot() {
	
	if(m_size < 3) {
		throw LlamaUnderflow("rot() ERROR: stack too small!");
	}
	
	// Note that pop() is not used in order to ensure the original node structure
	// remains unmodified and unnecessary node deletion and allocation does not occur.
	//
	// Save original first and second item values, then replace accordingly
	// with the third element in the first place and the original first 
	// and second elements following.
	T firstItem  = getConstRef(0);
	T secondItem = getConstRef(1);
	getRef(0) = getConstRef(2);
	getRef(1) = firstItem;
	getRef(2) = secondItem;
}

template<class T, int LN_SIZE>
T Llama<T,LN_SIZE>::peek(int offset) const {
	
	// Handle cases where function should throw an exception
	if(offset < 0) {
		throw LlamaUnderflow("peek() ERROR: offset cannot be negative!");
	}
	if(m_size == 0) {
		throw LlamaUnderflow("peek() ERROR: empty stack!");
	}
	if (offset >= m_size) {
		throw LlamaUnderflow("peek() ERROR: offset too large!");
	}
	
	// Save index of top element in stack
	int topIndex = (LN_SIZE - 1) - (m_size - 1) % LN_SIZE;
	int currNodeIndex = 0;
	
	LlamaNode<T,LN_SIZE>* currentNode = m_topNode;
	// If stack has extra node, skip the extra node
	if(m_hasExtraNode) {
		currentNode = currentNode->m_next;
	}
	
	// Advance nodes of linked list until data at relative offset is found
	bool advanceNode = true;
	while(advanceNode) {
		
		// The following inequality is used to test whether a node contains the relative offset:
		// (LN_SIZE * currNodeIndex) <= (offset + topIndex) <= (LN_SIZE * (currNodeIndex + 1) - 1)
		// Basically, if the offset plus the topIndex falls within the absolute index range 
		// for that node, stop advancing nodes and return the data.
		// Otherwise, keep advancing nodes until the offset plus the top index falls within 
		// the correct range. Worst case, only m_numNodes steps are required to access data
		// on the last node, which is the least frequently used (being the bottom of the stack).
		// For large Llama nodes, this cost is comparatively small as m_numNodes is
		// often a fraction of m_size.
		if((offset + topIndex) >= (LN_SIZE * currNodeIndex) && 
				(offset + topIndex) <= (LN_SIZE * (currNodeIndex + 1) - 1)) {
			
			// Once relative offset for node where data stored is found, break loop
			advanceNode = false;
			
		} else {
			++currNodeIndex;
			// Move to next node
			currentNode = currentNode->m_next;
		}
	}
	
	return currentNode->arr[offset + topIndex - (LN_SIZE * currNodeIndex)];
}

template<class T, int LN_SIZE>
const T& Llama<T,LN_SIZE>::getConstRef(long offset) const {
	
	// ***NOTE*** The implementation of this function is identical to that of peek(),
	// however, a const reference is returned for efficiency so that the copy
	// constructor (and then destructor for the copy) of the contained objects in the 
	// stack is NOT invoked every time this function is called internally from the 
	// Llama copy constructor, assignment operator, dup(), swap(), and rot().
	//
	// The use of a long type offset allows for access of elements in a very large stack.
	
	// Handle cases where function should throw an exception
	if(offset < 0) {
		throw LlamaUnderflow("getConstRef() ERROR: offset cannot be negative!");
	}
	if(m_size == 0) {
		throw LlamaUnderflow("getConstRef() ERROR: empty stack!");
	}
	if (offset >= m_size) {
		throw LlamaUnderflow("getConstRef() ERROR: offset too large!");
	}
	
	// Save index of top element in stack
	long topIndex = (LN_SIZE - 1) - (m_size - 1) % LN_SIZE;
	long currNodeIndex = 0;
	
	LlamaNode<T,LN_SIZE>* currentNode = m_topNode;
	// If stack has extra node, skip the extra node
	if(m_hasExtraNode) {
		currentNode = currentNode->m_next;
	}
	
	// Advance nodes of linked list until data at relative offset is found
	bool advanceNode = true;
	while(advanceNode) {
		
		// (LN_SIZE * currNodeIndex) <= (offset + topIndex) <= (LN_SIZE * (currNodeIndex + 1) - 1)
		// Basically if the offset plus the topIndex falls within the absolute index range 
		// for that node, stop advancing nodes and return the data.
		// Otherwise, keep advancing nodes until the offset plus the top index 
		// falls within the correct range.
		if((offset + topIndex) >= (LN_SIZE * currNodeIndex) && 
				(offset + topIndex) <= (LN_SIZE * (currNodeIndex + 1) - 1)) {
			
			advanceNode = false;
			
		} else {
			++currNodeIndex;
			// Move to next node
			currentNode = currentNode->m_next;
		}
	}
	
	return currentNode->arr[offset + topIndex - (LN_SIZE * currNodeIndex)];
}


template<class T, int LN_SIZE>
T& Llama<T,LN_SIZE>::getRef(long offset) {
	
	// ***NOTE*** The implementation of this function is identical to that of peek(),
	// however, a non-const reference is returned so assignment can be made, using
	// this->getRef(offset) as an l-value.
	//
	// This is extremely efficient for most swap() and rot() uses, as worst case, 
	// only 3 steps (and nodes) will be needed to access any of the top 3 data 
	// members. This function can, however, be extended to perform assignment 
	// on any value within the stack privately.
	//
	// Note that this operator is private, and can ONLY be used internally, which
	// prevents the user from dangerously modifying the inner stack.
	// It is particularly useful for swap() and rot(), where using pop() and push()
	// instead could have unintended side effects (namely deleting the extra node).
	// 
	// The use of a long type offset allows for access of elements in a very large stack.
	
	// Handle cases where function should throw an exception
	if(offset < 0) {
		throw LlamaUnderflow("getRef() ERROR: offset cannot be negative!");
	}
	if(m_size == 0) {
		throw LlamaUnderflow("getRef() ERROR: empty stack!");
	}
	if (offset >= m_size) {
		throw LlamaUnderflow("getRef() ERROR: offset too large!");
	}
	
	// Save index of top element in stack
	long topIndex = (LN_SIZE - 1) - (m_size - 1) % LN_SIZE;
	long currNodeIndex = 0;
	
	LlamaNode<T,LN_SIZE>* currentNode = m_topNode;
	// If stack has extra node, skip the extra node
	if(m_hasExtraNode) {
		currentNode = currentNode->m_next;
	}
	
	// Advance nodes of linked list until data at relative offset is found
	bool advanceNode = true;
	while(advanceNode) {
		
		// (LN_SIZE * currNodeIndex) <= (offset + topIndex) <= (LN_SIZE * (currNodeIndex + 1) - 1)
		// Basically if the offset plus the topIndex falls within the absolute index range 
		// for that node, stop advancing nodes and return the data.
		// Otherwise, keep advancing nodes until the offset plus the top index 
		// falls within the correct range.
		if((offset + topIndex) >= (LN_SIZE * currNodeIndex) && 
				(offset + topIndex) <= (LN_SIZE * (currNodeIndex + 1) - 1)) {
			
			advanceNode = false;
			
		} else {
			++currNodeIndex;
			// Move to next node
			currentNode = currentNode->m_next;
		}
	}
	
	return currentNode->arr[offset + topIndex - (LN_SIZE * currNodeIndex)];
}

template<class T, int LN_SIZE>
const Llama<T,LN_SIZE>& Llama<T,LN_SIZE>::operator=(const Llama<T, LN_SIZE>& rhs) {
	
	// Guard against self-assignment
	if(this != &rhs) {
		
		// Use destructor on old Llama class
		// Saving previous node structures may potentially be more efficient,
		// but introduces significant complications that cause unpredictable
		// and unstable behavior.
		this->~Llama();
		
		// Assign default values to initial empty Llama container
		m_size = 0;
		m_nodeCount = 1;
		m_hasExtraNode = false;
		m_topNode = new LlamaNode<T,LN_SIZE>;

		// Use the push and peek functions in conjunction to copy
		// and add necessary data elements to new stack.
		// For more detailed explanation, see copy constructor.
		for(int i = rhs.m_size - 1; i >= 0; --i) {
			push(rhs.getConstRef(i));
		}

		m_hasExtraNode = rhs.m_hasExtraNode;
		// If an extra node is present in other list, add extra node here as well
		if(m_hasExtraNode) {
			LlamaNode<T,LN_SIZE> *extraNode = new LlamaNode<T,LN_SIZE>;
			extraNode->m_next = m_topNode;
			m_topNode = extraNode;
		}
	}
	
	return *this;
}

#endif
