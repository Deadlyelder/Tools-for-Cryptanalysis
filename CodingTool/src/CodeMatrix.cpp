/*!
  \file CodeMatrix.cpp
  \author Tomislav Nad, Tomislav.Nad@iaik.tugraz.at
  \version 0.9
  \brief This file contains the implementation of the class CodeMatrix.
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

#include "CodeMatrix.h"


CodeMatrix::CodeMatrix(void) {
}

CodeMatrix::~CodeMatrix(void) {
}

void
CodeMatrix::Build(CodeWord (*pBuildFunction)(uint64_t&), uint64_t dDim) {
	m_oData.clear();
	for(uint64_t i = 0; i < dDim; i++)
		this->AddRow(pBuildFunction(i));
}

void
CodeMatrix::AddRow(CodeWord oRow) {
	if(!m_oData.empty())
		if(m_oData[0].GetLength() != oRow.GetLength()) {
			std::cout << "Error: Adding code word to matrix with wrong length" << std::endl;
			return;
		}
	m_oData.push_back(oRow);
}

CodeMatrix
CodeMatrix::GetSubMatrix(std::vector<uint64_t> & vRows, std::vector<uint64_t> & vCols) {

	CodeWord oTempWord;
	CodeMatrix oSubMatrix;

	// copy specified rows and columns
	for(uint64_t i = 0; i < vRows.size(); i++) {
		for(uint64_t j = 0; j < vCols.size(); j++)
			oTempWord.PushBool(m_oData[vRows[i]].AtBool(vCols[j]));
		oSubMatrix.AddRow(oTempWord);
		oTempWord.Clear();
	}
	return oSubMatrix;
}

bool
CodeMatrix::AtBool(uint64_t dRow, uint64_t dCol) {
	return m_oData[dRow].AtBool(dCol);
}

uint32_t
CodeMatrix::At32(uint64_t dRow, uint64_t dCol) {
	return m_oData[dRow].At32(dCol);
}

uint64_t
CodeMatrix::At64(uint64_t dRow, uint64_t dCol) {
return m_oData[dRow].At64(dCol);
}

void 
CodeMatrix::SetBool(uint64_t dRow, uint64_t dCol, bool dData) {
	m_oData[dRow].SetBool(dCol,dData);
}

void 
CodeMatrix::Set32(uint64_t dRow, uint64_t dCol, uint32_t dData) {
	m_oData[dRow].Set32(dCol,dData);
}

void 
CodeMatrix::Set64(uint64_t dRow, uint64_t dCol, uint64_t dData) {
	m_oData[dRow].Set64(dCol,dData);
}

void
CodeMatrix::DeleteRow(uint64_t dRow) {
	assert(dRow < m_oData.size());
	m_oData.erase(m_oData.begin()+dRow);
}

void
CodeMatrix::DeleteColumn(uint64_t dColumn) {
	assert(dColumn < m_oData[0].GetLength());
	for(uint32_t i = 0; i < m_oData.size(); i++)
		m_oData[i].EraseBool(dColumn);
}

uint64_t
CodeMatrix::GetRows() const {
	return m_oData.size();
}
	
uint64_t
CodeMatrix::GetColumns() const {
	if(m_oData.size() > 0)
		return m_oData[0].GetLength();
	else
		return 0;
}

uint64_t
CodeMatrix::GetColumns64() const {
	if(m_oData.size() > 0)
		return m_oData[0].GetLength64();
	else
		return 0;
}

void
CodeMatrix::PrintMatrix(const std::string & sFileName) {
	if(sFileName == "") {
		for(uint64_t i = 0; i < m_oData.size(); i++)
			m_oData[i].PrintBool(sFileName);
		return;
	}

	std::fstream f;
	f.open(sFileName.c_str(), std::ios::out);

	for(uint64_t i = 0; i < m_oData.size(); i++ ) {
		for(uint64_t j = 0; j < m_oData[0].GetLength(); j++ ) {
			bool t = m_oData[i].AtBool(j);
			f << t;
			if ( j == m_oData[0].GetLength()-1)
				f << std::endl;
			else
				f << " ";
		}
	}
	f.close();
}

void
CodeMatrix::ReadFromFile(const std::string & sFileName) {

	std::fstream oFileStream;

	oFileStream.open(sFileName.c_str(), std::ios::in);
	if( oFileStream.fail()) {
		std::cout << "Error: The file " << sFileName << " does not exist." << std::endl;
		return;
	}

	CodeWord oCodeWord;
	std::stringstream sStream;
	std::string sLine;
	uint16_t dValue;

	while( !oFileStream.eof() ) {
		getline(oFileStream,sLine);

		if( sLine.compare("") != 0 ) {
			sStream << sLine;// << std::endl;
			while( sStream.good()) {
				sStream >> dValue;
				oCodeWord.PushBool(static_cast<bool>(dValue));
			}
			m_oData.push_back(oCodeWord);
			oCodeWord.Clear();
			sStream.clear();
		}
	}
	
	oFileStream.close();
}

bool
CodeMatrix::IsSystematic() {

	if(GetRows() > GetColumns())
		return false;

	for(uint64_t i = 0; i < GetRows(); i++)
		for(uint64_t j = 0; j < GetRows(); j++) {
			if(m_oData[i].AtBool(i) != 1)
				return false;
			if(m_oData[i].AtBool(j) != 0 && i != j){
				return false;
			}
		}
	return true;
}

CodeMatrix
CodeMatrix::Transpose() {

	CodeMatrix oReturn;
	for(uint64_t i = 0; i < GetColumns(); i++) {
		CodeWord oTempWord;
		oReturn.AddRow(oTempWord);
	}
	for(uint64_t i = 0; i < GetRows(); i++)
		for(uint64_t j = 0; j < GetColumns(); j++)
			oReturn[j].SetBool(i,m_oData[i].AtBool(j));
	return oReturn;

}

CodeMatrix &
CodeMatrix::operator=(const CodeMatrix& oLeftSide) {
	m_oData = oLeftSide.m_oData;
	return *this;
}

CodeWord &
CodeMatrix::operator[](uint64_t dIndex) {
	assert(dIndex < m_oData.size());
	return m_oData[dIndex];
}

//EOF
