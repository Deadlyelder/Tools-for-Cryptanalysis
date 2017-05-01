/*!
  \file Parameters.h
  \author Tomislav Nad, Tomislav.Nad@iaik.tugraz.at
  \version 0.9
  \brief This is the header file of the class Parameters.
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

#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include <map>
#include <string>
#include <iostream>

#include "types.h"

//! This is a handler for parameters used in the CodingTool.
/*!
  This class handles parameters for the CodingTool. There are default
  parameters which are mainly used by LowWeightSearch::CanteautChabaud.
  The user has the possibility to add custom parameters, including
  default values, command line argument and help text.
  In combination with the InputHandler a custom parameter is automatically
  parsed from the command line.
  \n\n
  The default parameters are:\n
  
  	- Parameters::SIGMA sigma
	- Parameters::ITER  number of iteration 
	- Parameters::MINIMUM minimum weight
	- Parameters::OUTPUT  code word file for the output
	- Parameters::DOUTPUT disable output
	- Parameters::PERMUTE permute the columns of a generator matrix
	- Parameters::CWFILE code word file for the input
	- Parameters::CMFILE file containing a code matrix

  \see InputHandler
  \see LowWeightSearch
*/
class Parameters {
public:

	//! Constructor.
	/*!
	  Does nothing special.
	*/
	Parameters(void);

	//! Destructor.
	/*!
	  Does nothing special.
	*/
	virtual ~Parameters(void);

	//! Adds an integer parameter.
	/*!
      This method adds a new integer parameter with a default
	  value and a optional help text. The help text is printed
	  by InputHandler::PrintUsage.
	  \param sName Parameter name.
	  \param dValue Default value.
	  \param sHelpText Help text.
	*/
	void AddParameter(const std::string & sName, const uint64_t & dValue,
					  std::string sHelpText = "");


	//! Adds a string parameter.
	/*!
      This method adds a new string parameter with a default
	  value and a optional help text. The help text is printed
	  by InputHandler::PrintUsage.
	  \param sName Name of the parameter.
	  \param sValue Default value.
	  \param sHelpText Help text.
	*/
	void AddParameter(const std::string & sName, const std::string & sValue,
		              std::string sHelpText = "");

	//! Returns the value of a string parameter.
	/*!
      This method returns the value for the given parameter.
	  \param sName Name of the parameter.
	  \return Value of the parameter.
	*/
	std::string GetStringParameter(const std::string & sName);

	//! Returns the value of an integer parameter.
	/*!
      This method returns the value for the given parameter.
	  \param sName Name of the parameter.
	  \return Value of the parameter.
	*/
	uint64_t GetIntegerParameter(const std::string & sName);

	//! Returns the help text of a parameter.
	/*!
      This method returns the help text for the given parameter.
	  \param sName Name of the parameter.
	  \return Help text of the parameter.
	*/
	std::string GetHelpText(const std::string & sName);

	//! Changes the value of an integer parameter.
	/*!
      \param sName Name of the parameter.
	  \param dValue New value of the parameter.
	*/
	void SetParameter(const std::string & sName, const uint64_t & dValue);

	//! Changes the value of a string parameter.
	/*!
      \param sName Name of the parameter.
	  \param sValue New value of the parameter.
	*/
	void SetParameter(const std::string & sName, const std::string & sValue);

	//! Changes the help text of a parameter.
	/*!
      \param sName Name of the parameter.
	  \param sValue New help text of the parameter.
	*/
	void SetHelpText(const std::string & sName, const std::string & sValue);

	//! Returns all integer parameters.
	/*!
      \return All integer parameters.
	*/
	std::map<std::string,uint64_t> & GetIntegerParameters();
	
	//! Returns all string parameters.
	/*!
      \return All string parameters.
	*/
	std::map<std::string,std::string> & GetStringParameters();

	//! Returns all help texts.
	/*!
      \return All help texts.
	*/
	std::map<std::string,std::string> & GetHelpTexts();

	//! Outputs parameters to the console.
	/*!
      This method writes all parameter names and values
	  to the console.
	*/
	void Print();

	static const std::string SIGMA;   //!< sigma parameter of LowWeightSearch::CanteautChabaud.
	static const std::string ITER;    //!< Number of iterations for LowWeightSearch::CanteautChabaud.
	static const std::string MINIMUM; //!< Minimum weight for LowWeightSearch::CanteautChabaud.
	static const std::string OUTPUT;  //!< Code word file for LowWeightSearch::CanteautChabaud.
	static const std::string PERMUTE; //!< Flag for random permutation in LowWeightSearch::CanteautChabaud.
	static const std::string CWFILE;  //!< Code word file for input.
	static const std::string CMFILE;  //!< File containing a code matrix.
	static const std::string DOUTPUT; //!< Flag to disable the output.

private:

	std::map<std::string,uint64_t>    m_oIntegerParameters; //!< Contains all integer parameters
	std::map<std::string,std::string> m_oStringParameters;  //!< Contains all string parameters
	std::map<std::string,std::string> m_oHelpText;          //!< Contains help text of each parameter

};

#endif
