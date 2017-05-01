/*!
  \file sha1me.cpp
  \author Tomislav Nad, Tomislav.Nad@iaik.tugraz.at
  \version 0.9
  \brief Example for creating a code matrix.
*/
// Copyright (c) 2010 Graz University of Technology (IAIK) <http://www.iaik.tugraz.at>
//  
// This file is part of the CodingTool.
//
// The CodingTool is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// CodingTool is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with CodingTool.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>


#include "LowWeightSearch.h"
#include "InputHandler.h"
#include "types.h"

using namespace std;

//! Rotate a 32-bit word to the right.
# define ROTR(w,x) ((((w) & 0xFFFFFFFF) >> (x))|((w) << (32 - (x))))
//! Rotate a 32-bit word to the left.
# define ROTL(w,x) (((w) << (x))|(((w) & 0xFFFFFFFF) >> (32 - (x)))) 

CodeWord BuildFunction(uint64_t & dIter);
void SHA1ME(uint32_t * m);

/*! \example sha1me.cpp

    This is an example for creating a code matrix.
	This example uses the SHA-1 message expansion
	to create a linear code. It is known that the
	SHA-1 message expansion code has minimum weight
	25 in the last 60 words. Therefore, we create
	such a linear code and will use it in the other
	examples.    
	The size of the code is 512x1920.
	
	\see search.cpp
	\see shortening.cpp
*/
int main(int argc, const char* argv[]) {

	// create an empty generator matrix
	CodeMatrix oGenerator;
	
	// use the build function to create the generator matrix
	// with dimenstion 512
	oGenerator.Build(&BuildFunction,512);
	// save to file
	oGenerator.PrintMatrix("sha1me.cm");

	exit(1);
}

CodeWord BuildFunction(uint64_t & i) {

	CodeWord oCodeWord;
	uint32_t m[60];
	uint32_t unitv = 1;

	// 512 bit message block
	for(uint32_t j = 0; j<16; j++)
		m[j] = 0;

	// create i-th unit vector for the input
	unitv = ROTR(unitv , i+1);
	
	// set input to i-th unit vector
	m[i/32] = unitv;

	// call the message expansion
	SHA1ME(m);

	// add message to the code
	for(uint32_t j = 0; j<60; j++)
		oCodeWord.Push32(m[j]);
	
	return oCodeWord;
}


void SHA1ME(uint32_t * m) {

	// SHA-1 message expansion for the last 60 words
	for(uint32_t j = 16; j<60; j++)
		m[j] = ROTL((m[j-3] ^ m[j-8] ^ m[j-14] ^ m[j-16]),1);
}