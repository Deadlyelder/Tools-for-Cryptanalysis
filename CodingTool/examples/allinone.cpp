/*!
  \file allinone.cpp
  \author Tomislav Nad, Tomislav.Nad@iaik.tugraz.at
  \version 0.9
  \brief Example creating a code and searchin for code words.
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

/*! \example allinone.cpp

	This is an example which shows how one can do
	all the stuff in one program, instead of
	splitting it up like in the other examples.
	Also it is shown how one can add his own parameters.

	Again the SHA1 message expansion is used
	for demonstration.
*/
int main(int argc, const char* argv[]) {

	// create an empty generator matrix
	CodeMatrix oGenerator;
	// create an empty code word
	CodeWord oCodeWord;
	// create parameters
	Parameters oParameters;
	// create an input handler
	InputHandler oInputHandler(oParameters);
	// create the low weight search object
	LowWeightSearch oLowWS;

	// add a custom parameter
	bool bShortening = false;
	oParameters.AddParameter("-f",0,"enable code shortening");

	// parse the command line arguments
	// example: ./allinone -i 100 -o sha1me.cw -f 1
	if(oInputHandler.ParseSettings(argc, argv))
		exit(-1);

	bShortening = oParameters.GetIntegerParameter("-f");
		
	// use the build function to create the generator matrix
	// for the last 60 words of the SHA1 m.e.
	oGenerator.Build(&BuildFunction,512);

	// if shortening is enabled...
	if(bShortening) {
		vector<uint64_t> vForceZero;
		for(uint32_t i = 0; i < 32; i++)
			vForceZero.push_back(oGenerator.GetColumns()-32+i);
		oGenerator = LowWeightSearch::CodeShortening(oGenerator,vForceZero);
	}

	oCodeWord = oLowWS.CanteautChabaud(oGenerator,oParameters);
	oCodeWord.Print64();
	cout << "Hamming weight is " << oCodeWord.GetHammingWeight() << endl;
	
	// the last word should only be zero with "-f 1"
	uint32_t m[60];
	for(uint32_t j = 0; j<16; j++)
		m[j] = oCodeWord.At32(j);
	SHA1ME(m);
	
	cout << "last word = " << m[59] << endl;

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