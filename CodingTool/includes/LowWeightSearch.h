/*!
  \file LowWeightSearch.h
  \author Tomislav Nad, Tomislav.Nad@iaik.tugraz.at
  \version 0.9
  \brief This is the header file of the class LowWeightSearch.
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



#ifndef LOWWEIGHTSEARCH_H_
#define LOWWEIGHTSEARCH_H_

#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <cstring>
#include <cmath>

#include "types.h"
#include "RandomNumberGenerator.h"
#include "CodeWord.h"
#include "CodeMatrix.h"
#include "CodeWordFile.h"
#include "Parameters.h"


//! The main part of the CodingTool library.
/*!
  Finding code words in a linear code with low Hamming weights is
  very useful in cryptanalysis.
  This class provides several functionality for cryptanalysts.
  Combined with the other classes it simplifies the usage of coding
  theory in cryptanalysis.\n
  The main function is LowWeightSearch::CanteautChabaud, which
  implements an algorithm for finding low Hamming word code words.
  The user has only to provide a generator matrix (CodeMatrix). The
  algorithm returns a code word with the lowest Hamming weight found.
  \n\n
  Other functionalities are:

  - LowWeightSearch::GaussMod2 : takes as input a code matrix and returns the matrix in reduced
                row echelon form.

  - LowWeightSearch::RandomPermuteColumns : randomly permutes columns of a code matrix.

  - LowWeightSearch::SetCheckFunction : sets a user defined function which checks the validity of a code
                       word during the search.

  - LowWeightSearch::CheckToGenerator : transforms a check matrix to a generator matrix.

  - LowWeightSearch::SetWeightVector : sets weights for specific bits of the code, if not each bit
                      should be weighted equally.

  - LowWeightSearch::CodeShortening :shortens the linear code to eliminate specific columns. This
                       can be useful for linearized Hash functions to find only code words which produce
					   a collision.
*/
class LowWeightSearch {
public:
	//! Constructor.
	/*!
	  Does nothing special.
	*/
	LowWeightSearch();
	
	//! Destructor.
	/*!
	  Does nothing special.
	*/
	virtual ~LowWeightSearch();

    //! An algorithm to find code words with low Hamming weight.
	/*!
	  This method implements the low weight search algorithm by
	  Canteaut and Chabaud. The user has to provide a code
	  matrix, which represents the generator matrix. If the matrix
	  is not systematic, it is transformed into a systematic
	  generator matrix. The Parameters object contains the
	  search parameters like sigma, number of iterations or minimum
	  weight (see default parameters in InputHandler). The algorithm
	  returns the code word with lowest Hamming weight found. The
	  algorithm stops after the given number of iterations or the
	  given minimum weight is reached.\n\n
	  The user can specify a check function using LowWeightSearch::SetCheckFunction.
	  This functions is applied to each code word found and determines if the
	  code word is stored or discarded.\n\n
      By using LowWeightSearch::SetWeightVector one can define a weight vector
	  if different bits of the code word should be weighted differently.\n\n
	  If Parameters::PERMUTE is set the columns of the generator matrix
	  are permuted using LowWeightSearch::RandomPermuteColumns.\n\n
	  \n\n
	  This class offers a lot of possible improvements. Additional (faster) search algorithms or
	  faster implementations can easily be added.


      \param oGenerator The generator matrix.
	  \param oParameters The parameters for the search.
	  \return The code word with the lowest Hamming weight found.
	*/
	CodeWord CanteautChabaud(CodeMatrix oGenerator, Parameters  & oParameters);

    //! Transforms the matrix to reduced row echelon form.
	/*!
      During the operations the columns maybe permuted.
	  The method returns the permutation of the columns.
	  \param oMatrix The code matrix.
	  \return The permutation of the columns.
	*/
	std::vector<uint64_t> GaussMod2(CodeMatrix & oMatrix);

    //! Randomly permutes the columns of the given matrix.
	/*!
      This method can be used to increase the randomness of the
	  search algorithm.
	  \param oMatrix The code matrix.
	  \return The permutation of the columns.
	*/
	std::vector<uint64_t> RandomPermuteColumns(CodeMatrix & oMatrix);

    //! Sets the check function for the code words.
	/*!
      The specified check function is applied on each found
	  code word and determines if the code word is discarded or
	  stored.
	  \param pCheckFunction Pointer to the check function.
	*/
	void SetCheckFunction(bool (*pCheckFunction)(CodeWord&));

    //! Returns the indices of the combined rows.
	/*!
	  The final code word is usually a linear combination of
	  up to 4 rows of the generator matrix. This method
	  returns the indices of the combined rows.
	  \return The indices of the combined rows.
	*/
	std::vector<uint64_t> GetCombinedRows();

    //! Returns the performed Gaussian combinations during the search.
	/*!
      After each iteration Delta Gauss is applied on the generator matrix.
	  In parallel the same operations are applied on an identity matrix.
	  This matrix is returned by this method.
	  \return A reference to the matrix representing the performed Gaussian
	          operations.
	*/
	CodeMatrix & GetGaussCombinations();

    //! Sets the weight vector.
	/*!
      Sets the weight vector if bits of the code words
	  should be weighted differently. The length of the vector
	  has to be the same as the length of the code.
	  \param vWeights The weights.
	*/
	void SetWeightVector(std::vector<uint64_t> & vWeights);

    //! Transforms a check matrix to a generator matrix.
	/*!
      A cryptanalyst may face scenarios where she/he has rather
	  a check matrix than a generator matrix. This method
	  transforms the given check matrix to a generator matrix.
	  May be not necessary in the future if search algorithms
	  are implemented which work directly with check matrices.
	  \param oCheckMatrix The check matrix.
	*/
	static CodeMatrix CheckToGenerator(CodeMatrix & oCheckMatrix);

    //! Forces specific columns of the given matrix to zero.
	/*!
      This method may have different purposes. One purpose is
	  to force collisions for a linearized Hash function.
	  If the given generator matrix was created from a linearized
	  hash function one can use to force the last n bits of the
	  matrix to zero by code shortening, where n is the output size of the hash function.
	  In that way, the code words found by the search algorithm produce
	  a collision for the linearized hash function.\n\n\n
	  This method uses Gaussian elimination to fulfill its task. The code
	  dimension and code length is reduced. If the dimension is reduced to
	  zero, i.e. forcing to much columns to zero may not have a solution,
	  an error is printed.

	  \param oMatrix The generator matrix.
	  \param vColumns Indices of the columns which should be forced to zero.
	  \return The reduce matrix.
	*/
	static CodeMatrix CodeShortening(CodeMatrix & oMatrix, std::vector<uint64_t> & vColumns);

	//! Adds user defined text to the code word file.
	/*!
		Adds the text to the output file which is written by
		LowWeightSearch::CanteautChabaud.

		\param sInfo Informative text.
	*/
	void AddInformation(const std::string & sInfo);

private:
	//! An entry for the hash table used in LowWeightSearch::CanteautChabaud.
	/*!
	   This struct is used fo build the hash table for the
	   search algorithm.
	*/
	struct HashTableRecord {
		uint64_t dRow1;  //!< Index of row one.
		uint64_t dRow2;  //!< Index of row two.
		uint32_t dRows;  //!< Number of rows used for the record.
		struct HashTableRecord * pNextRecord; //!< Pointer to the next record.
	};

	//! Type definition for a hash table entry.
	typedef struct HashTableRecord HashTableRecord;
	

    //! Builds the code word.
	/*!
      During the search algorithm the generator matrix changes.
	  This method builds the code word which corresponds to the
	  original matrix, i.e. linear combination of the specific rows
	  is computed and all permutations are reversed.
	  \param vMinimum The indices of the rows which will be combined.
	  \param oZ The Z part of the generator matrix.
	  \param vColsPerm The permutation done by LowWeightSearch::DeltaGauss.
	  \param vGaussPerm The permutation done by LowWeightSearch::GaussMod2.
	  \param vRandPerm The permutation done by LowWeightSearch::RandomPermuteColumns.
	*/
	CodeWord BuildMinVector(std::vector<uint64_t> & vMinimum, CodeMatrix & oZ,
							std::vector<uint64_t> & vColsPerm,
							std::vector<uint64_t> & vGaussPerm,
							std::vector<uint64_t> & vRandPerm);

    //! Performs Delta Gauss on a given matrix.
	/*!
      After each iteration of LowWeightSearch::CanteautChabaud this method
	  is called. It exchanges randomly two columns and applies Delta Gauss
	  on the generator matrix.
	  \param[in] oZ The Z part of the systematic generator matrix.
	  \param[out] vColsPerm The permutation of the columns.
	*/
	void DeltaGauss(CodeMatrix & oZ, std::vector<uint64_t> & vColsPerm);

    //! Checks the values of the parameters.
	/*!
      This method checks if the values for the given parameters
	  are valid.
	  \param oParameters A reference to the parameters.
	*/
	bool CheckParameters(Parameters & oParameters);

    //! Creates an identity matrix.
	/*!
	  \param dDim The dimension of the matrix.
      \see LowWeightSearch::GetGaussCombinations.
	*/
	void CreateGaussMatrix(uint64_t dDim);

    //! Deletes the hash table.
	/*!
      \param aHashTable Pointer to the table.
	  \param dMaxTableSize Table size.
	*/
	void FreeHashTable(HashTableRecord** aHashTable,uint64_t dMaxTableSize);

	CodeMatrix            m_oGaussCombinations;  //!< Represents the performed Delta Gauss operations.
	std::vector<uint64_t> m_vCombinedRows;       //!< Indices of the combined rows.
	std::vector<uint64_t> m_vWeights;            //!< Weights for the bits of the code word.
	RandomNumberGenerator m_oRnGen;              //!< Random number generator.
	CodeWordFile          m_oOutputFile;         //!< Code word file object.

	bool (*m_pCheckFunction)(CodeWord&);         //!< Pointer to the check function.


};

#endif /*LOWWEIGHTSEARCH_H_*/

