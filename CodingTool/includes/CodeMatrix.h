/*!
  \file CodeMatrix.h
  \author Tomislav Nad, Tomislav.Nad@iaik.tugraz.at
  \version 0.9
  \brief This is the header file of the class CodeMatrix.
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

#ifndef CODEMATRIX_H_
#define CODEMATRIX_H_

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <cassert>

#include "types.h"
#include "CodeWord.h"

//! This class represents a binary code matrix.
/*! 
  The data structure consists of a STL vector containing instances of the 
  class CodeWord. Each row of the matrix is one code word. Therefore,
  the data is stored in 64-bit or 32-bit words, depending on 
  the target architecture. Data can be added and accessed in different ways
  (see CodeWord). The matrix can be considered as a bit, 32-bit word or
  64-bit word matrix (little endian). 
  The number of rows or columns refers to the bit representation of the matrix,
  since it represents code dimension and length.

  \see CodeWord
*/
class CodeMatrix {
public:
	//! Constructor.
	/*!
	  Does nothing special.
	*/
	CodeMatrix(void);

	//! Destructor.
	/*!
	  Does nothing special.
	*/
	virtual ~CodeMatrix(void);

	//! Creates a code matrix.
	/*!
      This method creates a matrix with dimension dDim, by calling
	  the function pBuildFunction dDim times. The provided function
	  should take as argument an integer and return a code word, which
	  represents one row the code matrix.
	  \param pBuildFunction Pointer to the function which returns a code word.
	  \param dDim Dimension of the code (=number of rows).
	*/
	void Build(CodeWord (*pBuildFunction)(uint64_t&), uint64_t dDim);

	//! Adds a row to the matrix.
	/*!
      If the matrix is not empty the length of the added code word
	  has to be the same as the current length of the matrix. If it is not
	  an error message is printed to the console and the code word is not
	  added.
	  \param oRow The code word which should be added.
	*/
	void AddRow(CodeWord oRow);

	//! Returns a submatrix of the current matrix.
	/*!
      This method returns a new code matrix defined by the inputs
	  vRows and vCols.
	  \param vRows The indices of the rows which should be included
	               in the new matrix.
      \param vCols The indices of the columns which should be included
	               in the new matrix.
      \return The submatrix fo the current matrix.
	*/
	CodeMatrix GetSubMatrix(std::vector<uint64_t> & vRows, std::vector<uint64_t> & vCols);

	//! Returns the bit at the given position.
	/*!
      This method returns the element at the given position with
	  respect to the binary version of the matrix.
	  \param dRow The row index of the matrix.
	  \param dCol The column index of the matrix with respect to
	              the binary representation.
	  \return The value at (dRow,dCol).
	*/
	bool     AtBool(uint64_t dRow, uint64_t dCol);

	//! Returns the 32-bit value at the given position.
	/*!
      This method considers the matrix consisting of 32-bit words. It
	  returns the element at the given position. The indices should be given
	  with respect to the 32-bit representation of the matrix.
	  \param dRow The row index of the matrix.
	  \param dCol The column index of the matrix with respect to
	              the 32-bit representation.
	  \return The value at (dRow,dCol).
	*/
	uint32_t At32(uint64_t dRow, uint64_t dCol);

	//! Returns the 64-bit value at the given position.
	/*!
      This method considers the matrix consisting of 64-bit words. It
	  returns the element at the given position. The indices should be given
	  with respect to the 64-bit representation of the matrix.
	  \param dRow The row index of the matrix.
	  \param dCol The column index of the matrix with respect to
	              the 64-bit representation.
	  \return The value at (dRow,dCol).
	*/
	uint64_t At64(uint64_t dRow, uint64_t dCol);

	//! Changes the bit at the given position.
	/*!
      This method sets the element at the given position with
	  respect to the binary version of the matrix.
	  \param dRow The row index of the matrix.
	  \param dCol The column index of the matrix with respect to
	              the binary representation.
	  \param dData The new value for the element.
	*/
	void SetBool(uint64_t dRow, uint64_t dCol, bool dData);

	//! Changes the element at the given position.
	/*!
      This method considers the matrix consisting of 32-bit words. It
	  sets the element at the given position. The indices should be given
	  with respect to the 32-bit representation of the matrix.
	  \param dRow The row index of the matrix.
	  \param dCol The column index of the matrix with respect to
	              the 32-bit representation.
	  \param dData The new 32-bit value.
	*/
	void Set32(uint64_t dRow, uint64_t dCol, uint32_t dData);

	//! Changes the element at the given position.
	/*!
      This method considers the matrix consisting of 64-bit words. It
	  sets the element at the given position. The indices should be given
	  with respect to the 64-bit representation of the matrix.
	  \param dRow The row index of the matrix.
	  \param dCol The column index of the matrix with respect to
	              the 64-bit representation.
	  \param dData The new 64-bit value.
	*/
	void Set64(uint64_t dRow, uint64_t dCol, uint64_t dData);

	//! Deletes a row of the matrix.
	/*!
	  This method deletes the specified row.
	  \param dRow The index of the row.
	*/
	void DeleteRow(uint64_t dRow);

	//! Deletes a column of the matrix.
	/*!
	  This method deletes the specified column.
	  The matrix is considered as binary.
	  \param dColumn The index of the row with respect to
	              the binary representation of the matrix.
	*/
	void DeleteColumn(uint64_t dColumn);

	//! Returns the number of rows.
	/*!
	  \return The number of rows.
	*/
	uint64_t GetRows() const;

	//! Returns the number of columns.
	/*!
	  \return The number of columns.
	*/
	uint64_t GetColumns() const;

	//! Returns the number of columns.
	/*!
	  This method considers the matrix as a 64-bit word
	  matrix.

	  \return The number of columns.
	*/
	uint64_t GetColumns64() const;

	//! Outputs the matrix.
	/*!
      If no filename is given the matrix is written to
	  the console. Otherwise it is written to the given file.
	  The given file will be overwritten. This method
	  considers the matrix as a binary matrix.
	  \param sFileName The filename.
	*/
	void PrintMatrix(const std::string & sFileName="");

    //! Creates matrix from the given data.
	/*!
      This method reads data from the given file and
	  creates the matrix. The matrix has to be
	  in the same format as specified by PrintMatrix.
	  \param sFileName The filename containing data.
	*/
	void ReadFromFile(const std::string & sFileName);

	//! Returns true if the matrix is a systematic generator matrix.
	/*!
	  \return Returns true if matrix is systematic, otherwise false.
	*/
	bool IsSystematic();

	//! Returns the transposed matrix.
	/*!
	  This method considers the matrix as binary.
      \return The transposed matrix.
	*/
	CodeMatrix Transpose();	

	//! Overloads the assignment operator.
	/*!
      The overloaded operator does a deep copy of the object.
	  \param oM The right hand side of the assignment.
	  \return The left hand side of the assignment.
	*/
	CodeMatrix & operator=(const CodeMatrix& oM);

	//! Returns one row of the matrix.
	/*!
      This method returns a reference of the specified
	  code word, which represents a row of the matrix.
	  \param dIndex The row index.
	  \return The reference to the code word.
	*/
	CodeWord & operator[](uint64_t dIndex);

private:

	std::vector<CodeWord> m_oData;  //!< The matrix data.

};

#endif
