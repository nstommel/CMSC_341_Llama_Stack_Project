
/******************************************************************************
 * File:    Driver.cpp
 * Project: CMSC 341 Project 1, Fall 2016
 * Author:  Nicholas Stommel
 * Date:    10/04/16
 * Section: 05
 * E-mail:  stommel1@gl.umbc.edu
 * 
 * Description: This driver file is a general test file for Llama Stacks of various
 * types. All functions are tested, and some exceptions are also tested.
 * 
 ******************************************************************************/

#include "Llama.h"
#include <iostream>
using namespace std;

int main() {

	cout << "FLOAT TEST... " << endl;
	
	Llama<float,3> floatTest;
	
	// Test dump on empty list
	floatTest.dump();
	
	// Test push operation
	floatTest.push(3.4);
	floatTest.push(4.8);
	floatTest.push(5.6);
	floatTest.push(8.2);
	floatTest.push(6.2);
	floatTest.push(7.8);
	floatTest.push(8.3);
	floatTest.push(2.7);
	floatTest.push(4.6);
	floatTest.push(9.2);
	floatTest.push(3.2);
	floatTest.push(8.5);
	
	// Test dump for the first time
	floatTest.dump();
	
	
	// Test copy constructor
	Llama<float,3> copyTest(floatTest);
	copyTest.dump();
	
	Llama<float,3> assignTest;
	assignTest.push(8.2);
	assignTest.push(2.4);
	assignTest.push(5.8);
	assignTest.push(4.1);
	assignTest.push(1.7);
	assignTest.dump();
	
	// Test Assignment operator
	assignTest = floatTest;
	assignTest.dump();
	
	// Test peek function on float list
	cout << endl << "PEEK TEST" << endl;
	for(int i = 0; i < floatTest.size(); i++) {
		cout << "Peeked " << floatTest.peek(i) 
				<< " at offset " << i << endl;
	}
	
	// Test pop function on float list
	cout << endl << "POP TEST" << endl;
	for(int i = 0; floatTest.size() > 0; ++i) {
		cout << "Popped " << floatTest.pop() << endl;
	}
	floatTest.dump();
	
	cout << "CHAR TEST..." << endl;
	
	// Test different type
	Llama<char,4> charTest;
	charTest.push('i');
	charTest.push('h');
	charTest.push('g');
	charTest.push('f');
	charTest.push('e');
	charTest.push('d');
	charTest.push('c');
	charTest.push('b');
	charTest.push('a');
	
	charTest.dump();
	
	cout << "TESTING ROT()..." << endl;
	charTest.rot();
	charTest.dump();
	cout << "TESTING ROT()..." << endl;
	charTest.rot();
	charTest.dump();
	cout << "TESTING SWAP()..." << endl;
	charTest.swap();
	charTest.dump();
	cout << "TESTING DUP()..." << endl;
	charTest.dup();
	charTest.dump();
	
	cout << "LN_SIZE = 1 TEST..." << endl;
	// Specifically test list of LN_SIZE one for verification of 
	// correct behavior in dump, push, peek, and pop functions
	Llama<int,1> oneTest;
	oneTest.push(5);
	oneTest.push(4);
	oneTest.push(3);
	oneTest.push(2);
	oneTest.push(1);
	oneTest.push(0);
	
	oneTest.dump(); 
	
	// Test peek function on int list
	cout << endl << "PEEK TEST" << endl;
	for(int i = 0; i < oneTest.size(); i++) {
		cout << "Peeked " << oneTest.peek(i) 
				<< " at offset " << i << endl;
	}
	
	// Test pop function on int list
	cout << endl << "POP TEST" << endl;
	cout << "Popped " << oneTest.pop() << endl;
	cout << "Popped " << oneTest.pop() << endl;
	cout << "Popped " << oneTest.pop() << endl;
	oneTest.dump();
	
	cout << "TESTING ROT()..." << endl;
	oneTest.rot();
	oneTest.dump();
	cout << "TESTING SWAP()..." << endl;
	oneTest.swap();
	oneTest.dump();
	
	cout << "PUSH TEST" << endl;
	oneTest.push(8);
	cout << "Pushed " << 8 << endl;
	oneTest.push(9);
	cout << "Pushed " << 9 << endl;
	oneTest.dump();
	
	cout << "FURTHER POP TEST" << endl;
	cout << "Popped " << oneTest.pop() << endl;
	cout << "Popped " << oneTest.pop() << endl;
	oneTest.dump();
	
	cout << "Popped " << oneTest.pop() << endl;
	cout << "Popped " << oneTest.pop() << endl;
	oneTest.dump();
	cout << "Popped " << oneTest.pop() << endl;
	oneTest.dump();
	
	
	cout << "TESTING FOR EXCEPTIONS..." << endl << endl;
	
	try {
		Llama<double,0> invalidZeroSizeNode;
		
	} catch(exception& e) {
		cout << "Caught exception: " << e.what() << endl << endl;
	}
	
	try {
		cout << oneTest.pop() << endl;
		
	} catch(exception& e) {
		cout << "Caught exception: " << e.what() << endl << endl;
	}
	
	try{
		cout << charTest.peek(charTest.size()) << endl;
		
	} catch(exception& e) {
		cout << "Caught exception: " << e.what() << endl << endl;
	}
	
	// Uncomment just for fun if you want to test
	// filling and dumping absolutely massive Llama containers.
	// If you pipe the dump() cerr output of Driver.out to a file 
	// using 2> you will end up with a file over 100MB of just text, 
	// containing a printout of all 10 million items and their corresponding 
	// 1 million node addresses!
	/*
	Llama<long,10> tenMillionNodeTest;
	// 1'000'000'000
	for(long i = 0; i < 1000000000; ++i) {
		tenMillionNodeTest.push(i);
	}
	MillionNodeTest.dump();
	*/
       	return 0;

}
