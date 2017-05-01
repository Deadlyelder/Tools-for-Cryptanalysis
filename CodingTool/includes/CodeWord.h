/*!
  \file CodeWord.h
  \author Tomislav Nad, Tomislav.Nad@iaik.tugraz.at
  \version 0.9
  \brief This is the header file of the class CodeWord.
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

#ifndef CODEWORD_H_
#define CODEWORD_H_

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <cassert>

#include "types.h"
#include "HammingWeight.h"

//! This class represents a binary code word.
/*! 
  The data structure consists of a vector from STL containing either
  32-bit or 64-bit words, depending on the target architecture.
  Data can be added and accessed in different ways. A code word 
  can be considered as a bit, 32-bit word or 64-bit word (little endian). 
  The length of a code word refers to the bit representation.
*/
class CodeWord {
public:
	//! Constructor.
	/*!
	  Does nothing special.
	*/
	CodeWord(void);

	//! Destructor.
	/*!
	  Does nothing special.
	*/
	virtual ~CodeWord(void);

	//! Returns the Hamming weight of the code word.
	/*!
      \return The Hamming weight of the code word.
	*/
	uint64_t GetHammingWeight() const;

	//! Returns the Hamming weight where bits are weighted differently.
	/*!
      \param vWeights Contains the weight for each bit of the code word.
	  \return The weighted Hamming weight.
	*/
	uint64_t GetHammingWeight(std::vector<uint64_t> & vWeights) const;

	//! Returns the length of the code word.
	/*!
      \return The length of the code word with respect to its binary
	          representation.
	*/
	uint64_t GetLength() const;

	//! Returns the length of the code word.
	/*!
      \return The length of the code word with respect to its 64-bit words
	          representation.
	*/
	uint64_t GetLength64() const;

	//! Returns the code word as vector of bits.
	/*!
      \return The vector containing the bits of the code word.
	*/
	std::vector<bool> GetDataBool() const;

	//! Returns the code word as a vector of 32-bit words.
	/*!
	  The method returns the code word considered as 32-bit words
	  in little endian format.
      \return The 32-bit words of the code word.
	*/
	std::vector<uint32_t> GetDataUInt32() const;

	//! Returns the code word as a vector of 64-bit words.
	/*!
	  The method returns the code word considered as 64-bit words
	  in little endian format.
      \return The 64-bit words of the code word.
	*/
	std::vector<uint64_t> GetDataUInt64() const;

	//! Returns the bit from the given position.
	/*!
      This method returns the bit from the given position with
	  respect to the bit representation of the code word.
	  \param dIndex The position of the bit.
	  \return The value at dIndex.
	*/
	bool AtBool(uint64_t dIndex) const;
	
	//! Returns the 32-bit word from the given position.
	/*!
      This method returns a 32-bit word from the given position with
	  respect to the 32-bit word representation of the code word.
	  \param dIndex The position of the element.
	  \return The value at dIndex.
	*/
	uint32_t At32(uint64_t dIndex) const;

	//! Returns the 64-bit word from the given position.
	/*!
      This method returns a 64-bit word from the given position with
	  respect to the 64-bit word representation of the code word.
	  \param dIndex The position of the element.
	  \return The value at dIndex.
	*/
	uint64_t At64(uint64_t dIndex) const;

	//! Sets the bit at the given position.
	/*!
      This method sets the bit at the given position with
	  respect to the bit representation of the code word.
	  \param dIndex The position of the bit.
	  \param dData The new value.
	*/
	void SetBool(uint64_t dIndex, bool dData);

	//! Sets the 32-bit word at the given position.
	/*!
      This method sets the 32-bit word at the given position with
	  respect to the 32-bit word representation of the code word.
	  \param dIndex The position of the element.
	  \param dData The new value.
	*/
	void Set32(uint64_t dIndex, uint32_t dData);

	//! Sets the 64-bit word at the given position.
	/*!
      This method sets the 64-bit word at the given position with
	  respect to the 64-bit word representation of the code word.
	  \param dIndex The position of the element.
	  \param dData The new value.
	*/
	void Set64(uint64_t dIndex, uint64_t dData);

	//! Deletes a bit at the given position.
	/*!
      This method deletes the bit at the given position with
	  respect to the bit representation of the code word.
	  \param dIndex The position of the bit.
	*/
	void EraseBool(uint64_t dIndex);

	//! Deletes the 32-bit word at the given position.
	/*!
      This method deletes the 32-bit word at the given position with
	  respect to the 32-bit word representation of the code word.
	  \param dIndex The position of the element.
	*/
	void Erase32(uint64_t dIndex);

	//! Deletes the 64-bit word at the given position.
	/*!
      This method deletes the 64-bit word at the given position with
	  respect to the 64-bit word representation of the code word.
	  \param dIndex The position of the element.
	*/
	void Erase64(uint64_t dIndex);

	//! Adds a bit at the end of the code word.
	/*!
	  \param dData The new bit.
	*/
	void PushBool(bool dData);

	//! Adds 32 bits at the end of the code word.
	/*!
	  \param dData The new bits.
	*/
	void Push32(uint32_t dData);

	//! Adds 64 bits at the end of the code word.
	/*!
	  \param dData The new bits.
	*/
	void Push64(uint64_t dData);

	//! Removes the last bit of the code word.
	/*!
	  Removes the last bit of the code word.
	*/
	void PopBool();

	//! Removes the last 32 bits of the code word.
	/*!
	  Removes the last 32 bits of the code word.
	*/
	void Pop32();

	//! Removes the last 64 bits of the code word.
	/*!
	  Removes the last 64 bits of the code word.
	*/
	void Pop64();

	//! Deletes all data.
	/*!
      Deletes all data.
	*/
	void Clear();

	//! Outputs the code word bit-wise
	/*!
      This method writes the code word to the given
	  file or if no filename is given to the console.
	  \param sOutput The filename.
	*/
	void PrintBool(const std::string sOutput="") const;

	//! Outputs the code word as 32-bit words.
	/*!
      This method writes the code word to the given
	  file or if no filename is given to the console.
	  The data is written as hexadecimal numbers.
	  \param sOutput The file name.
	*/
	void Print32(const std::string sOutput="") const;

	//! Outputs the code word as 64-bit words.
	/*!
      This method writes the code word to the given
	  file or if no filename is given to the console.
	  The data is written as hexadecimal numbers.
	  \param sOutput The file name.
	*/
	void Print64(const std::string sOutput="") const;

	//! Overloads the assignment operator.
	/*!
      The overloaded operator does a deep copy of the object.
	  \param oCodeWord The right hand side of the assignment.
	  \return The left hand side of the assignment.
	*/
	CodeWord & operator=(const CodeWord & oCodeWord);

	//! Overloads the XOR operator.
	/*!
      The overloaded operator xors each elements of the code words.
	  \param oCodeWord The right hand side of the assignment.
	  \return The left hand side of the assignment.
	*/
	CodeWord & operator^=(const CodeWord & oCodeWord);

	//! Overloads the XOR operator.
	/*!
      The overloaded operator xors each elements of the code words.
	  \param oCodeWord The right hand side of the assignment.
	  \return The left hand side of the assignment.
	*/
	const CodeWord operator^(const CodeWord & oCodeWord) const;

	//! Overloads the == operator.
	/*!
      The overloaded operator checks if each element of both
	  objects are equal and returns true, otherwise false.
	  Both code words have to have the same length.
	  The check is donoe by an assert call.
	  \param oCodeWord The right hand side of the equality.
	  \return The left hand side of the equality.
	*/
	const bool operator==(const CodeWord & oCodeWord) const;

private:
	uint64_t              m_dHammingWeight;   //!< The Hamming weight of the code word.
	std::vector<uint64_t> m_oData;            //!< The data.
	uint8_t               m_dOffSet;          //!< Amount of free bits of the last word in m_oData.
};

#endif
