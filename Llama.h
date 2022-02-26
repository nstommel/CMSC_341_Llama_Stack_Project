
/******************************************************************************
 * File:    Llama.h
 * Project: CMSC 341 Project 0, Fall 2016
 * Author:  Nicholas Stommel
 * Date:    10/04/16
 * Section: 05
 * E-mail:  stommel1@gl.umbc.edu
 * 
 * Description: This is the header file containing the declaration of 
 * the Llama class and its fields, methods and overloaded operators. The Llama
 * class is on the outside a stack that can only be modified from the top
 * in a last-in first-out manner. Underneath, the Llama stack is actually a 
 * linked list where each node contains at maximum a user-specified LN_SIZE 
 * elements. This is useful for maintaining a dynamic stack, but limiting calls
 * to new and delete.
 * 
 ******************************************************************************/

#ifndef _LLAMA_H_
#define _LLAMA_H_

#include <stdexcept>
#include <string>
#include "LlamaNode.h"

using namespace std;

class LlamaUnderflow : public std::out_of_range {

public:

	//-------------------------------------------------------
	// Name: LlamaUnderflow constructor
	// PreCondition: a single string parameter is required
	// PostCondition: constructs a basic exception class for use
	// in the Llama class
	//---------------------------------------------------------
	LlamaUnderflow(const string& what) : std::out_of_range(what) { }

};


template <class T, int LN_SIZE>
class Llama {

public:

	//-------------------------------------------------------
	// Name: default Llama stack constructor
	// PreConditions: LN_SIZE cannot be zero or negative
	// PostConditions: constructs a basic default Llama stack
	// with one node
	//---------------------------------------------------------
	Llama();
	
	//-------------------------------------------------------
	// Name: Llama stack copy constructor
	// PreConditions: a valid Llama stack must be passed for copying
	// PostConditions: constructs a 'deep copy' of a Llama stack
	//---------------------------------------------------------
	Llama(const Llama<T,LN_SIZE>& other);
	
	//-------------------------------------------------------
	// Name: Llama stack destructor
	// PreConditions: none
	// PostConditions: deletes all dynamically allocated nodes in
	// the stack (and thereby the associated data)
	//---------------------------------------------------------
	~Llama();

	//-------------------------------------------------------
	// Name: size
	// PreConditions: none
	// PostConditions: returns current number of elements in stack
	//---------------------------------------------------------
	int size(); 
	
	//-------------------------------------------------------
	// Name: dump
	// PreConditions: none
	// PostConditions: prints a detailed representation of the
	// stack and the memory locations of its nodes, as well
	// as several properties of the stack
	//---------------------------------------------------------
	void dump();
	
	//-------------------------------------------------------
	// Name: push
	// PreConditions: data passed in must be of type T
	// PostConditions: adds data to the top of the stack, 
	// allocating memory for and linking a new top node if necessary
	//---------------------------------------------------------
	void push(const T& data);
	
	//-------------------------------------------------------
	// Name: pop
	// PreConditions: the stack must have at least one element to pop
	// PostConditions: effectively removes item from stack, deleting
	// an extra node if the node containing the top data elements 
	// reaches half of LN_SIZE or below. 
	//---------------------------------------------------------
	T pop();

	//-------------------------------------------------------
	// Name: dup
	// PreConditions: the stack must have at least one element to duplicate
	// PostConditions: the topmost element is copied and the copy is pushed
	// to the stack
	//---------------------------------------------------------
	void dup();    //  (top) A B C D -> A A B C D
	
	//-------------------------------------------------------
	// Name: swap
	// PreConditions: the stack must contain at least two elements to 
	// perform a swap
	// PostConditions: swaps the top two items in the stack
	//---------------------------------------------------------
	void swap();   //  (top) A B C D -> B A C D 
	
	//-------------------------------------------------------
	// Name: rot
	// PreConditions: the stack must contain at least three elements
	// to perform a rot() operation
	// PostConditions: takes the third item in the stack (from the top)
	// and places it on the top of the stack, rearranging it
	//---------------------------------------------------------
	void rot();    //  (top) A B C D -> C A B D

	//-------------------------------------------------------
	// Name: peek
	// PreConditions: the stack must contain at least one element to access,
	// the offset cannot be negative, and the offset cannot be >= the total
	// number of elements in the stack
	// PostConditions: returns by value a copy of data of type T at a 
	// given place in the stack
	//---------------------------------------------------------
	T peek(int offset) const;

	//-------------------------------------------------------
	// Name: overloaded assignment operator
	// PreConditions: none (check for self assignment is performed)
	// PostConditions: deletes lhs Llama stack and fills with a copy
	// of the rhs stack during assignment
	//---------------------------------------------------------
	const Llama<T,LN_SIZE>& operator=(const Llama<T,LN_SIZE>& rhs);
	
private:
	
	// long type used in place of int hereafter to accommodate potentially large Llama lists

	// Function used internally to obtain a non-modifiable const reference to
	// any data on the stack. This is much better for stacks that contain
	// complex and large objects, where returning by value and invoking the
	// copy constructor in peek() would be an expensive operation. Follows
	// peek() implementation.
	//
	//-------------------------------------------------------
	// Name: getConstRef
	// PreConditions: the stack must contain at least one element to access,
	// the offset cannot be negative, and the offset cannot be >= the total
	// number of elements in the stack
	// PostConditions: returns a const reference to data at a given place
	// in the stack
	//---------------------------------------------------------
	const T& getConstRef(long offset) const;
	
	
	// Function used internally to obtain a reference to any data on the stack,
	// useful in rot() and swap(). Implementation follows stable peek() function,
	// but instead returns a non const reference so assignment can be made with
	// getRef(num) as an l-value. Because this function is private, the interior
	// of the stack is still not directly accessible to the user, which is
	// good for security reasons and for encapsulation.
	//
	//-------------------------------------------------------
	// Name: getRef
	// PreConditions: the stack must contain at least one element to access,
	// the offset cannot be negative, and the offset cannot be >= the total
	// number of elements in the stack
	// PostConditions: returns a non-const reference to data at a given place
	// in the stack
	//---------------------------------------------------------
	T& getRef(long offset);	
	
	long m_size;                     // total number elements in stack
	long m_nodeCount;                // total number of nodes in stack
	bool m_hasExtraNode;             // boolean for if stack has extra node
	LlamaNode<T,LN_SIZE>* m_topNode; // pointer to the top node of the stack

};

#include "Llama.cpp"

#endif
