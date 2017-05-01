/*!
  \file HammingWeight.cpp
  \author Tomislav Nad, Tomislav.Nad@iaik.tugraz.at
  \version 0.9
  \brief This file implements the Hamming weight functions.
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

#include "HammingWeight.h"

uint32_t
HammingWeight(uint64_t dWord) {
	uint32_t dBytes = sizeof(dWord);
	if(dBytes == 4)
		return ( (hwt[dWord & 0xFF] + hwt[(dWord>>8) & 0xFF] + hwt[(dWord>>16) & 0xFF] + hwt[(dWord>>24) & 0xFF]) );
	else if(dBytes ==8)
		return ( (hwt[dWord & 0xFF] + hwt[(dWord>>8) & 0xFF] + hwt[(dWord>>16) & 0xFF] + hwt[(dWord>>24) & 0xFF]) +
				 (hwt[dWord>>32 & 0xFF] + hwt[(dWord>>40) & 0xFF] + hwt[(dWord>>48) & 0xFF] + hwt[(dWord>>56) & 0xFF]));
	return 0;
}

uint64_t
HammingWeight(uint64_t dWord, std::vector<uint64_t> & vWeights) {
	uint32_t dBytes = sizeof(dWord);
	uint64_t dWeight = 0;

	for(uint64_t i = 0; i < dBytes*8; i++) {
		if( (dWord >> ((dBytes*8-1)-i)) & 1 )
			dWeight += vWeights[i];
	}
	return dWeight;
}

