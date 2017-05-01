/*!
  \file RandomNumberGenerator.h
  \author Tomislav Nad, Tomislav.Nad@iaik.tugraz.at
  \version 0.9
  \brief This is the header file of the class RandomNumberGenerator.
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


#ifndef RANDOMNUMBERGENERATOR_H_
#define RANDOMNUMBERGENERATOR_H_

#include <ctime>
#include <vector>
#include <fstream>

#include "mtrand.h"
#include "types.h"

//! This class provides access to a random number generator.
/*!
  The CodingTool does not implement its own random number
  generator. Instead it is using the generator from
  Makoto Matsumoto and Takuji Nishimura which was ported to
  C++ by Jasper Bedaux (see http://www.bedaux.net/mtrand/).
  \n\n\n
  The purpose of this interface is to make the type
  of random number generator easy exchangeable.
*/
class RandomNumberGenerator {
public:

    //! Constructor.
	/*!
	  The constructor initializes the random number generator
	  using /dev/urandom on *nix machines and time(NULL) on
	  Windows machines.
	*/
	RandomNumberGenerator();
	
	//! Destructor.
	/*!
	  Does nothing special.
	*/
	virtual ~RandomNumberGenerator();

	//! Returns a positive integer.
	/*!
      This method returns a random integer between 0 and n.
	  \param n Maximum random number.
	  \return A random number.
	*/
	uint64_t getRandomPosInteger(uint64_t n);
	
	//! Returns a vector of positive integers.
	/*!
      This method returns a vector of random integers from
	  the closed interval [lb,ub]. The vector does not contain duplicates.

	  \param lb Lower bound for the random numbers.
	  \param ub Upper bound for the random numbers.
	  \param ln Number of random numbers.
	  \return A vector with random numbers.
	*/
	std::vector<uint64_t> getRandomPosVector(uint64_t lb,
											 uint64_t ub, uint64_t ln);
	
    //! Returns the seed of the generator.
	/*!
      \return The 32-bit word seed.
	*/
	uint32_t getSeed();
private:
		
	MTRand_int32 m_oGenerator; //!< The random number generator
	uint32_t	 m_dSeed;      //!< The seed.
};

#endif /*RANDOMNUMBERGENERATOR_H_*/
