/*!
  \file CodeWord.cpp
  \author Tomislav Nad, Tomislav.Nad@iaik.tugraz.at
  \version 0.9
  \brief This file contains the implementation of the class CodeWord.
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

#include "CodeWord.h"

CodeWord::CodeWord(void) : m_dHammingWeight(0),m_dOffSet(0) {

}

CodeWord::~CodeWord(void) {
}


uint64_t
CodeWord::GetHammingWeight() const {
	return m_dHammingWeight;
}

uint64_t
CodeWord::GetHammingWeight(std::vector<uint64_t> & vWeights) const {
	uint64_t dWeight = 0;
	uint16_t dWordSize = sizeof(uint64_t)*8;

	for(uint64_t i = 0; i < m_oData.size(); i++)
		for(uint64_t j = 0; j < dWordSize; j++) {
			if( (m_oData[i] >> ((dWordSize-1)-j)) & 1 )
				dWeight += vWeights[i];
		}
	return dWeight;
}

uint64_t 
CodeWord::GetLength() const {
	return static_cast<uint64_t>(m_oData.size()*sizeof(uint64_t)*8-m_dOffSet);
}

uint64_t 
CodeWord::GetLength64() const {
	return m_oData.size();
}

std::vector<bool> 
CodeWord::GetDataBool() const {
	std::vector<bool> vReturn;
	for(uint32_t i = 0; i < m_oData.size(); i++)
		for(uint32_t j = 0; j < sizeof(uint64_t)*8; j++)
			vReturn.push_back((m_oData[i] >> ((sizeof(uint64_t)*8-1)-j)) & 1);
	return vReturn;
}

std::vector<uint32_t> 
CodeWord::GetDataUInt32() const {
	std::vector<uint32_t> vReturn;
	if(sizeof(uint64_t)==4)
		for(uint32_t i = 0; i < m_oData.size(); i++)
			vReturn.push_back(static_cast<uint32_t>(m_oData[i]));
	else {
		for(uint32_t i = 0; i < m_oData.size(); i++){
			vReturn.push_back(static_cast<uint32_t>(m_oData[i] >> (sizeof(uint32_t)*8)));
			vReturn.push_back(static_cast<uint32_t>(m_oData[i]));
		}
	}
	return vReturn;
}

std::vector<uint64_t>
CodeWord::GetDataUInt64() const {
	return m_oData;
}

bool
CodeWord::AtBool(uint64_t dIndex) const {
	assert(sizeof(uint64_t)*8*m_oData.size() > dIndex);
	uint64_t dWordSize = sizeof(uint64_t)*8;
	bool bReturn = (m_oData[dIndex/dWordSize] >> (dWordSize-1-dIndex%dWordSize)) & 1;
	return bReturn;
}
uint32_t
CodeWord::At32(uint64_t dIndex) const {
	assert(sizeof(uint64_t)/sizeof(uint32_t)*m_oData.size() > dIndex);

	uint32_t dReturn;

	if(sizeof(uint64_t)==4)
		return static_cast<uint32_t>(m_oData[dIndex]);
	else {
		if(dIndex%(sizeof(uint64_t)/sizeof(uint32_t)))
			dReturn = static_cast<uint32_t>(m_oData[dIndex/(sizeof(uint64_t)/sizeof(uint32_t))]);
		else
			dReturn = static_cast<uint32_t>(m_oData[dIndex/(sizeof(uint64_t)/sizeof(uint32_t))] >> sizeof(uint32_t)*8);
	}
	// if we are at the last word and there is a offset, remove the offset
	if( m_oData.size() == dIndex/sizeof(uint64_t) +1) {
		if(dIndex%(sizeof(uint64_t)/sizeof(uint32_t)))
			dReturn >>= m_dOffSet;
		else
			dReturn >>= m_dOffSet - sizeof(uint32_t)*8;
	}

	return dReturn;
}
uint64_t
CodeWord::At64(uint64_t dIndex) const {
	if( m_oData.size() == dIndex+1)
		return m_oData[dIndex] >> m_dOffSet;
	return m_oData[dIndex];
}

void
CodeWord::SetBool(uint64_t dIndex, bool dData) {
	uint32_t dWordSize = sizeof(uint64_t)*8;
	
	m_dHammingWeight -= HammingWeight(m_oData[dIndex/dWordSize]);

	uint64_t dMask = static_cast<uint64_t>(1) << (dWordSize-1-dIndex%dWordSize); ;
	if(dData)
		m_oData[dIndex/dWordSize] |= dMask;
	else
		m_oData[dIndex/dWordSize] &= ~dMask;

	m_dHammingWeight += HammingWeight(m_oData[dIndex/dWordSize]);
}

void
CodeWord::Set32(uint64_t dIndex, uint32_t dData) {

	uint32_t dWordSize = sizeof(uint64_t)*8;
	
	m_dHammingWeight -= HammingWeight(m_oData[dIndex/2]);
	if(dWordSize==32)
		m_oData[dIndex] = dData;
	else {
		uint64_t dMask = 0;
		if(dIndex%2) {
			dMask = static_cast<uint64_t>(0xFFFFFFFF);
			m_oData[dIndex/2] = (m_oData[dIndex/2] & dMask) | dData << (dWordSize/2);
		}
		else {
			dMask = static_cast<uint64_t>(0xFFFFFFFF) << (dWordSize/2);		
			m_oData[dIndex/2] = (m_oData[dIndex/2] & dMask) | dData;
		}
	}
	m_dHammingWeight += HammingWeight(m_oData[dIndex/2]);
}


void
CodeWord::Set64(uint64_t dIndex, uint64_t dData) {
	m_dHammingWeight -= HammingWeight(m_oData[dIndex]);
	m_oData[dIndex] = dData;
	m_dHammingWeight += HammingWeight(m_oData[dIndex]);
}

void
CodeWord::EraseBool(uint64_t dIndex) {
	assert(dIndex < m_oData.size()*(sizeof(uint64_t)*8));
	std::vector<uint64_t> vTemp = m_oData;
	uint64_t dOldOffSet = m_dOffSet;
	Clear();

	for(uint64_t i = 0; i < vTemp.size()-1; i++) {
		for(uint64_t j = 0; j < sizeof(uint64_t)*8; j++) {
			if(i*sizeof(uint64_t)*8+j==dIndex)
				continue;
			PushBool((vTemp[i] >> ((sizeof(uint64_t)*8-1)-j)) & 1);
		}
	}
	for(uint64_t j = 0; j < sizeof(uint64_t)*8-dOldOffSet; j++) {
		if((vTemp.size()-1)*sizeof(uint64_t)*8+j==dIndex)
			continue;
		PushBool((vTemp[vTemp.size()-1] >> ((sizeof(uint64_t)*8-1)-j)) & 1);
	}
}

void
CodeWord::Erase32(uint64_t dIndex) {
	// can be more efficient
	std::vector<uint64_t> vTemp = m_oData;
    Clear();
	
	if(sizeof(uint64_t)==4)
		Erase64(dIndex);
	else {
		for(uint64_t i = 0; i < vTemp.size(); i++) {
			Push64(vTemp[i]);

			if(i == dIndex/2 && dIndex%2 == 0) {
				Pop32();
				Pop32();
				Push32(static_cast<uint32_t>(vTemp[i]));
			}
			if(i == dIndex/2 && dIndex%2 == 1) {
				Pop32();
			}
		}
	}
}

void
CodeWord::Erase64(uint64_t dIndex) {
	m_dHammingWeight -= HammingWeight(m_oData[dIndex]);
	m_oData.erase(m_oData.begin()+dIndex);
}

void
CodeWord::PushBool(bool dData) {

	if(m_dOffSet > 0) {
		m_oData[m_oData.size()-1] = m_oData[m_oData.size()-1] 
		                                | (static_cast<uint64_t>(dData) << (m_dOffSet-1));
		m_dOffSet--;
	}
	else {
		m_dOffSet = sizeof(uint64_t)*8-1;
		m_oData.push_back(static_cast<uint64_t>(dData) << m_dOffSet);
	}

	if(dData)
		m_dHammingWeight++;
}

void
CodeWord::Push32(uint32_t dData) {

	// can be more efficient 
	if(m_dOffSet > 0)
		for(uint32_t i = 0; i < sizeof(uint32_t)*8; i++)
			this->PushBool((dData >> ((sizeof(uint32_t)*8-1)-i)) & 1);
	else {
		m_oData.push_back(static_cast<uint64_t>(dData) << (sizeof(uint32_t)*8));
		m_dOffSet = (sizeof(uint64_t)-sizeof(uint32_t))*8;
		m_dHammingWeight += HammingWeight(dData);
	}
}

void
CodeWord::Push64(uint64_t dData) {

	if(m_dOffSet > 0)
		for(uint32_t i = 0; i < sizeof(uint64_t)*8; i++)
			this->PushBool((dData >> ((sizeof(uint64_t)*8-1)-i)) & 1);
	else {
		m_oData.push_back(dData);
		m_dHammingWeight += HammingWeight(dData);
	}
}


void
CodeWord::PopBool() {
	m_dHammingWeight -= HammingWeight(m_oData[m_oData.size()-1]);
	m_oData[m_oData.size()-1] = m_oData[m_oData.size()-1] & ~(static_cast<uint64_t>(1) << m_dOffSet);
	m_dOffSet++;
	if(m_dOffSet == sizeof(uint64_t)*8)
		m_dOffSet = 0;
	m_dHammingWeight += HammingWeight(m_oData[m_oData.size()-1]);
}

void
CodeWord::Pop32() {

	if(m_dOffSet < (sizeof(uint32_t)*8)) {
		m_dHammingWeight -= HammingWeight(m_oData[m_oData.size()-1]);
		for(uint32_t i = m_dOffSet; i < m_dOffSet+sizeof(uint32_t)*8; i++)
			m_oData[m_oData.size()-1] = m_oData[m_oData.size()-1] & ~(static_cast<uint64_t>(1) << i);
		m_dOffSet += sizeof(uint32_t)*8;
		m_dHammingWeight += HammingWeight(m_oData[m_oData.size()-1]);
	}
	else {
		m_dHammingWeight -= HammingWeight(m_oData[m_oData.size()-1]);
		m_oData.pop_back();
		m_dHammingWeight -= HammingWeight(m_oData[m_oData.size()-1]);
		m_dOffSet -= sizeof(uint32_t)*8;
		for(uint32_t i = 0; i < m_dOffSet; i++)
			m_oData[m_oData.size()-1] = m_oData[m_oData.size()-1] & ~(static_cast<uint64_t>(1) << (m_dOffSet-1+i));
		m_dHammingWeight += HammingWeight(m_oData[m_oData.size()-1]);
	}
}

void
CodeWord::Pop64() {
	m_dHammingWeight -= HammingWeight(m_oData[m_oData.size()-1]);
	m_oData.pop_back();
	if(m_dOffSet > 0) {
		for(uint32_t i = 0; i < m_dOffSet; i++) {
			m_dHammingWeight -= HammingWeight(m_oData[m_oData.size()-1]);
			m_oData[m_oData.size()-1] = m_oData[m_oData.size()-1] & ~(static_cast<uint64_t>(1) << (m_dOffSet-1+i));
			m_dHammingWeight += HammingWeight(m_oData[m_oData.size()-1]);
		}
	}
}

void
CodeWord::Clear() {
	m_dHammingWeight = 0;
	m_oData.clear();

	m_dOffSet = 0;
}

void 
CodeWord::PrintBool(const std::string oOutput) const {

	std::vector<bool> vBool = GetDataBool();

	if(oOutput == "") {
		for(uint64_t i = 0; i < vBool.size(); i++)
			std::cout << vBool[i] << " ";
		std::cout << std::endl;
	}
	else {
		std::fstream f;
		f.open(oOutput.c_str(), std::ios::out);
		for(uint64_t i = 0; i < vBool.size(); i++ ) {
			f << vBool[i];
			if(i < vBool.size()-1)
				f << " ";
		}
		f << std::endl;
		f.close();
	}
}

void 
CodeWord::Print32(const std::string oOutput) const {

	std::vector<uint32_t> vUInt = GetDataUInt32();

	if(oOutput == "") {
		for(uint64_t i = 0; i < vUInt.size(); i++)
			std::cout << std::hex << vUInt[i] << " ";
		std::cout << std::dec << std::endl;
	}
	else {
		std::fstream f;
		f.open(oOutput.c_str(), std::ios::out);
		for(uint64_t i = 0; i < vUInt.size(); i++ ) {
			f << std::hex << vUInt[i];
			if(i < vUInt.size()-1)
				f << " ";
		}
		f << std::endl;
		f.close();
	}
}

void 
CodeWord::Print64(const std::string oOutput) const {

	std::vector<uint64_t> vUInt = GetDataUInt64();

	if(oOutput == "") {
		for(uint64_t i = 0; i < vUInt.size(); i++)
			std::cout << std::hex << vUInt[i] << " ";
		std::cout << std::dec << std::endl;
	}
	else {
		std::fstream f;
		f.open(oOutput.c_str(), std::ios::out);
		for(uint64_t i = 0; i < vUInt.size(); i++ ) {
			f << std::hex << vUInt[i];
			if(i < vUInt.size()-1)
				f << " ";
		}
		f << std::endl;
		f.close();
	}
}



CodeWord & 
CodeWord::operator=(const CodeWord & oCodeWord) {
	m_dHammingWeight = oCodeWord.m_dHammingWeight;
	m_oData = oCodeWord.m_oData;

	m_dOffSet = oCodeWord.m_dOffSet;
	return *this;
}

CodeWord & 
CodeWord::operator^=(const CodeWord & oCodeWord) {
	
	this->m_dHammingWeight = 0;
	for(uint64_t i = 0; i < oCodeWord.m_oData.size(); i++) {
		this->m_oData[i] ^= oCodeWord.m_oData[i];
		this->m_dHammingWeight += HammingWeight(this->m_oData[i]);
	}
	if( this->m_dOffSet < oCodeWord.m_dOffSet )
		this->m_dOffSet = oCodeWord.m_dOffSet;
	return *this;
}

const CodeWord 
CodeWord::operator^(const CodeWord & oOther) const {
    return CodeWord(*this) ^= oOther;
  }

const bool 
CodeWord::operator==(const CodeWord & oOther) const {
	assert(this->GetLength() == oOther.GetLength());
	for(uint32_t i = 0; i < oOther.GetLength64(); i++)
		if(oOther.At64(i) != this->At64(i))
			return false;
	return true;
 }

//EOF
