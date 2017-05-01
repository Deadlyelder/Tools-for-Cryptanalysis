/*!
  \file RandomNumberGenerator.cpp
  \author Tomislav Nad, Tomislav.Nad@iaik.tugraz.at
  \version 0.9
  \brief This file contains the implementation of the class RandomNumberGenerator.
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

#include "RandomNumberGenerator.h"


RandomNumberGenerator::RandomNumberGenerator()
{
	m_dSeed = static_cast<uint32_t>(time(NULL));
#ifdef __unix__
	std::fstream oRandFile;
    oRandFile.open("/dev/urandom");
    oRandFile.read(reinterpret_cast<char *>(&m_dSeed), sizeof(m_dSeed));
    oRandFile.close();
#endif
	m_oGenerator.seed(m_dSeed);
}

RandomNumberGenerator::~RandomNumberGenerator()
{
	
}

uint64_t 
RandomNumberGenerator::getRandomPosInteger(uint64_t n)
{
	uint64_t dTemp = m_oGenerator();
	dTemp = (dTemp << 32) | m_oGenerator();
	uint64_t random = dTemp % n;
	return random;
}

std::vector<uint64_t>
RandomNumberGenerator::getRandomPosVector(uint64_t lb,
		uint64_t ub, uint64_t ln)
{
	uint64_t rn = 0;
	
	std::vector<uint64_t> vI, vReturn;
	
	for(uint64_t i = lb; i <= ub; i++)
		vI.push_back(i);
	
	for(uint32_t i = 0; i < ln; i++)
	{
		if( ub-lb-i <=0 ) {
			vReturn.push_back(vI.at(0));
			break;
		}
		rn = getRandomPosInteger((ub-lb)-i);
		vReturn.push_back(vI.at(rn));
		vI.erase(vI.begin()+rn);
	}
	
	return vReturn;
}

uint32_t 
RandomNumberGenerator::getSeed()
{
	return m_dSeed;
}
