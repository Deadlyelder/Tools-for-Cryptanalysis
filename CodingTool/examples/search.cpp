/*!
  \file search.cpp
  \author Tomislav Nad, Tomislav.Nad@iaik.tugraz.at
  \version 0.9
  \brief Example for finding low Hamming weight words.
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

/*! \example search.cpp

    This is an example how to read a code matrix from 
	a file and applies a low Hamming weight search.

	If the matrix was previously created from
	the SHA1 message expansion example, the algorithm
	should find the Hamming weight of 25 after few iterations.
	Using the argument "-pc 1" which enables random permutaion
	of the columns, results in feewer needed iterations.

	\see sha1me.cpp
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

	string sCMFile = "";
	
	// parse the command line arguments
	// example: ./search -i 100 -cm matrix.cm -pc 1
	if(oInputHandler.ParseSettings(argc, argv))
		exit(-1);
	
	// get the file name of the code matrix
	sCMFile = oParameters.GetStringParameter(Parameters::CMFILE);

	// read data from the file
	oGenerator.ReadFromFile(sCMFile);
	// start the search
	oCodeWord = oLowWS.CanteautChabaud(oGenerator,oParameters);

	// print the code word and the Hamming weight
	oCodeWord.Print64();
	cout << "Hamming weight is " << oCodeWord.GetHammingWeight() << endl;

	exit(1);
}
