/*!
  \file InputHandler.cpp
  \author Tomislav Nad, Tomislav.Nad@iaik.tugraz.at
  \version 0.9
  \brief This file contains the implementation of the class InputHandler.
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

#include "InputHandler.h"

InputHandler::InputHandler(Parameters & oParameters) : m_oParameters(oParameters) {
	m_oParameters.AddParameter(Parameters::SIGMA,20,
		"\t -s \t width for submatrices Z1 and Z2 (default is 20)");
	m_oParameters.AddParameter(Parameters::ITER,20,
		"\t -i \t number of iterations (default is 20)");
	m_oParameters.AddParameter(Parameters::MINIMUM,0,
		"\t -m \t minimum weight as stopping criteria (default is 0)");
	m_oParameters.AddParameter(Parameters::OUTPUT,
		"default.cw","\t -o \t output file for minimum weight vector (default is default.cw)");
	m_oParameters.AddParameter(Parameters::DOUTPUT,0,
		"\t -d \t disable output (default is enabled)");
	m_oParameters.AddParameter(Parameters::PERMUTE,0,
		"\t -pc \t enable random permutation of the columns (default is disabled)");
	m_oParameters.AddParameter(Parameters::CWFILE,"",
		"\t -cw \t read a code word file");
	m_oParameters.AddParameter(Parameters::CMFILE,""
		,"\t -cm \t read a file containing a code matrix");
}

InputHandler::~InputHandler() {
}


bool
InputHandler::ParseSettings(int argc, const char* argv[]) {
	std::istringstream sStream;
	bool bError = false;

	for(int i = 1; i < argc; i++) {
		std::string sPara(argv[i]);
		int dValue = -1;
		sStream.clear();
		if(sPara.compare("-h") == 0 || argc <= 1) {
			bError = true;
			PrintUsage();
			}
		else {
			std::map<std::string,uint64_t>::iterator iIter =
				m_oParameters.GetIntegerParameters().begin();
			for(iIter = m_oParameters.GetIntegerParameters().begin();
				iIter != m_oParameters.GetIntegerParameters().end(); iIter++ ) {
				if(sPara.compare((*iIter).first) == 0) {
					if( ++i < argc) {
						sStream.str(argv[i]);
						sStream >> dValue;
						(*iIter).second = dValue;
						break;
					}
				}
			}
			std::map<std::string,std::string>::iterator sIter = 
				m_oParameters.GetStringParameters().begin();
			for(sIter = m_oParameters.GetStringParameters().begin();
				sIter != m_oParameters.GetStringParameters().end(); sIter++ ) {
				if(sPara.compare((*sIter).first) == 0) {
					if( ++i < argc) {
						(*sIter).second = argv[i];
						break;
					}
				}
			}
			if( sIter == m_oParameters.GetStringParameters().end() && 
				iIter == m_oParameters.GetIntegerParameters().end() ) {
				bError = true;
				PrintUsage();
			}
		}
	}

	return bError;
}

void
InputHandler::PrintUsage() {
	std::cout << "Usage: ./program [-p n] [-s n] [-i n] [-m n] [-o ouput] [-d] " << std::endl;

	std::map<std::string,std::string>::iterator it = m_oParameters.GetHelpTexts().begin();
	for(it = m_oParameters.GetHelpTexts().begin(); it != m_oParameters.GetHelpTexts().end(); it++) {
			std::cout << "\t" << it->first << "\t" << it->second << std:: endl;
	}
}

//EOF
