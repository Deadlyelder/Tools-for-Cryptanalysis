/*!
  \file CodeWordFile.cpp
  \author Tomislav Nad, Tomislav.Nad@iaik.tugraz.at
  \version 0.9
  \brief  This file contains the implementation of the class CodeWordFile.
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

#include "CodeWordFile.h"

CodeWordFile::CodeWordFile() : m_sFileName(""){
}

CodeWordFile::CodeWordFile(std::string & sFileName) : m_sFileName(sFileName){
}

CodeWordFile::~CodeWordFile(void){
}

bool
CodeWordFile::Read(const std::string & sFileName) {
	
	std::fstream oStream;

	m_sFileName = sFileName;
	oStream.open(sFileName.c_str(), std::ios::in);
	if( oStream.fail()) {
		std::cout << "Error: The file " << sFileName << " does not exist." << std::endl;
		return false;
	}

	ReadHeader(oStream);
	ReadCodeWords(oStream);

	oStream.close();
	return true;
}

bool
CodeWordFile::Write(const std::string & sFileName) {
	std::fstream oStream;

	m_sFileName = sFileName;
	oStream.open(sFileName.c_str(), std::ios::out);
	if( oStream.fail()) {
		std::cout << "Error: The file " << sFileName << " could not be written." << std::endl;
		return false;
	}

	WriteHeader(oStream);
	//WriteCodeWords(oStream);

	oStream.close();
	return true;
}

bool 
CodeWordFile::WriteCodeWord(const CodeWord & oCodeWord) {
	std::fstream oFileStream;

	if(m_sFileName.compare("") == 0) {
		std::cout << "Error: Missing file name." << std::endl;
		return false;
	}

	oFileStream.open(m_sFileName.c_str(), std::ios::app | std::ios::out);
	if( oFileStream.fail()) {
		std::cout << "Error: The file " << m_sFileName << " could not be written." << std::endl;
		return false;
	}

	// write HW on first position
	oFileStream << oCodeWord.GetHammingWeight() << " ";
	for (uint32_t i = 0; i < oCodeWord.GetLength(); i++) {
		if(i == (oCodeWord.GetLength()-1))
			oFileStream << oCodeWord.AtBool(i);
		else
			oFileStream << oCodeWord.AtBool(i) << " ";
	}
	oFileStream << std::endl;
	return true;
}

void
CodeWordFile::AddText(const std::string & sText) {
	m_sText = sText;
}

bool
CodeWordFile::ReadHeader(std::fstream & oFileStream) {
	std::string sLine, sParameter;
	uint32_t dValue;
	std::stringstream sStream;
	std::string sValue;
	
	// begin header
	getline(oFileStream,sLine);
	if(sLine.compare("%BEGIN") != 0) {
		std::cout << "Error: the file has no valid header." << std::endl;
		return false;
	}
	
	// skip the "Info:" text
	while( !oFileStream.eof() ) {
		getline(oFileStream,sLine);
		// structure: "parameter value"
		if( sLine.compare("%INTEGER") != 0 ) {

		}
		else
			break;
		sStream.clear();
		
	}
	while( !oFileStream.eof() ) {
		getline(oFileStream,sLine);
		// structure: "parameter value"
		if( sLine.compare("%STRING") != 0 ) {
			sStream << sLine << std::endl;
			sStream >> sParameter >> dValue;
			m_oParameters.AddParameter(sParameter,dValue);
		}
		else
			break;
		sStream.clear();
		
	}
	while( !oFileStream.eof() ) {
		getline(oFileStream,sLine);
		// structure: "parameter value"
		if( sLine.compare("%END") != 0 ) {
			sStream << sLine << std::endl;
			sStream >> sParameter >> sValue;
			m_oParameters.AddParameter(sParameter,sValue);
		}
		else
			return true;
		sStream.clear();
		
	}
	return false;
}

void
CodeWordFile::WriteHeader(std::fstream & oFileStream) {

	oFileStream << "%BEGIN" << std::endl;
	oFileStream << "Info: " << m_sText << std::endl;
	oFileStream << "%INTEGER" << std::endl;
	for(std::map<std::string,uint64_t>::iterator it = m_oParameters.GetIntegerParameters().begin(); 
		it != m_oParameters.GetIntegerParameters().end(); it++) {
			oFileStream << it->first << " " << it->second << std:: endl;
	}
	oFileStream << "%STRING" << std::endl;
	for(std::map<std::string,std::string>::iterator it = m_oParameters.GetStringParameters().begin(); 
		it != m_oParameters.GetStringParameters().end(); it++) {
			oFileStream << it->first << " " << it->second << std:: endl;
	}
	oFileStream << "%END" << std::endl;

}

void
CodeWordFile::ReadCodeWords(std::fstream & oFileStream) {

	CodeWord oCodeWord;
	std::stringstream sStream;
	std::string sLine;
	uint16_t dValue;
	bool bFirst = true;

	while( !oFileStream.eof() ) {
		getline(oFileStream,sLine);

		if( sLine.compare("") != 0 ) {
			sStream << sLine;// << std::endl;
			bFirst = true;
			while( sStream.good()) {
				sStream >> dValue;
				// first number is weight
				if(bFirst)
					bFirst = false;
				else
					oCodeWord.PushBool(static_cast<bool>(dValue));
			}
			m_oCodeWordList.push_back(oCodeWord);
			oCodeWord.Clear();
			sStream.clear();
		}
	}
}

void
CodeWordFile::SetFileName(const std::string & sName) {
	m_sFileName = sName;
}

void
CodeWordFile::SetParameters(const Parameters & oParameters) {
	m_oParameters = oParameters;
}

const Parameters & 
CodeWordFile::GetParameters() {
	return m_oParameters;
}

const std::string &
CodeWordFile::GetFileName() {
	return m_sFileName;
}

const CodeWordFile::CodeWordList &
CodeWordFile::GetCodeWords() {
	return m_oCodeWordList;
}

//EOF
