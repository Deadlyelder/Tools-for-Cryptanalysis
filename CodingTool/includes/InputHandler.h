/*!
  \file InputHandler.h
  \author Tomislav Nad, Tomislav.Nad@iaik.tugraz.at
  \version 0.9
  \brief This is the header file of the class InputHandler.
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


#ifndef INPUTHANDLER_H_
#define INPUTHANDLER_H_

#include <iostream>
#include <string>
#include <sstream>
#include <map>

#include "types.h"
#include "Parameters.h"

//! This class handles the arguments from the command line interface. 
/*! 
  This class provides the functionality of parsing command line
  arguments. The valid arguments are defined in the Parameters object.
  Several default parameters with respect to the low weight search
  algorithm are already included.\n\n
  
  The default parameters are:\n
  
  	- Parameters::SIGMA sigma
	- Parameters::ITER  number of iteration 
	- Parameters::MINIMUM minimum weight
	- Parameters::OUTPUT  code word file for the output
	- Parameters::DOUTPUT disable output
	- Parameters::PERMUTE permute the columns of a generator matrix
	- Parameters::CWFILE code word file for the input
	- Parameters::CMFILE file containing a code matrix

  \see Parameters
  
*/
class InputHandler {
public:
	//! Constructor
	/*!
	  The constructor needs a reference of a Parameters object.
	  Only parameters already included in this object are considered
	  as valid arguments. The default values are overwritten
	  if an argument was found by InputHandler::ParseSettings.
	  Several default parameters are added in the constructor.
	  \param oParameters A reference to the Parameters object.
	*/
	InputHandler(Parameters & oParameters);

	//! Destructor.
	/*!
	  Does nothing special.
	*/
	virtual ~InputHandler();

	//! Parses command line arguments and stores them in a Parameters object.
	/*!
      \param argc Number of arguments. 
	  \param argv List of arguments.
	*/
	bool ParseSettings(int argc, const char* argv[]);

	//! Prints the usage of a program.
	/*!
      Prints the help text of each parameter.
	*/
	void PrintUsage();

private:
	Parameters & m_oParameters;  //!< The parameters.
};

#endif /* INPUTHANDLER_H_ */
