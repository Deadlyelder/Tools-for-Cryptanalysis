/*!
  \file Parameters.cpp
  \author Tomislav Nad, Tomislav.Nad@iaik.tugraz.at
  \version 0.9
  \brief This file contains the implementation of the class Parameters.
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

#include "Parameters.h"

const std::string Parameters::SIGMA = "-s";
const std::string Parameters::ITER = "-i";
const std::string Parameters::MINIMUM = "-m";
const std::string Parameters::OUTPUT = "-o";
const std::string Parameters::PERMUTE = "-pc";
const std::string Parameters::CWFILE = "-cw";
const std::string Parameters::CMFILE = "-cm";
const std::string Parameters::DOUTPUT = "-d";

Parameters::Parameters(void) {

};

Parameters::~Parameters(void) {
};

void
Parameters::AddParameter(const std::string & sName, const uint64_t & dValue,
						 std::string sHelpText) {
	m_oIntegerParameters[sName] = dValue;
	m_oHelpText[sName] = sHelpText;
}

void
Parameters::AddParameter(const std::string &sName, const std::string & sValue,
						 std::string sHelpText) {
	m_oStringParameters[sName] = sValue;
	m_oHelpText[sName] = sHelpText;
}

uint64_t
Parameters::GetIntegerParameter(const std::string & sName){
	return m_oIntegerParameters[sName];
}

std::string
Parameters::GetStringParameter(const std::string & sName){
	return m_oStringParameters[sName];
}

std::string
Parameters::GetHelpText(const std::string & sName){
	return m_oHelpText[sName];
}

void
Parameters::SetParameter(const std::string & sName, const uint64_t & dValue) {
	m_oIntegerParameters[sName] = dValue;
}

void
Parameters::SetParameter(const std::string & sName, const std::string & sValue) {
	m_oStringParameters[sName] = sValue;
}

void
Parameters::SetHelpText(const std::string & sName, const std::string & sValue) {
	m_oHelpText[sName] = sValue;
}

std::map<std::string,uint64_t> &
Parameters::GetIntegerParameters(){
	return m_oIntegerParameters;
}

std::map<std::string,std::string> &
Parameters::GetStringParameters() {
	return m_oStringParameters;
}

std::map<std::string,std::string> &
Parameters::GetHelpTexts() {
	return m_oHelpText;
}

void Parameters::Print() {
	std::cout << "-------------------- Parameters --------------------" << std::endl;
	std::map<std::string,uint64_t>::iterator iIter = m_oIntegerParameters.begin();
	for(iIter = m_oIntegerParameters.begin(); iIter != m_oIntegerParameters.end(); iIter++ ) {
		std::cout << iIter->first << "\t" << iIter->second << std::endl;
	}
	std::map<std::string,std::string>::iterator sIter = m_oStringParameters.begin();
	for(sIter = m_oStringParameters.begin(); sIter != m_oStringParameters.end(); sIter++ ) {
		std::cout << sIter->first << "\t" << sIter->second << std::endl;
	}
	std::cout << "----------------------------------------------------" << std::endl;
}

//EOF
