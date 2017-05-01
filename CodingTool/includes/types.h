/*!
  \file types.h
  \author Tomislav Nad, Tomislav.Nad@iaik.tugraz.at
  \version 0.9
  \brief This file contains type definitions used through the library.
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

#ifndef TYPES_H_
#define TYPES_H_

#ifdef __unix__
#include <stdint.h>
#endif

#ifdef _MSC_VER

typedef signed char			int8_t;
typedef short int			int16_t;
typedef int					int32_t;
typedef long long int		int64_t;

typedef unsigned char		uint8_t;
typedef unsigned short int	uint16_t;
typedef unsigned int		uint32_t;
//typedef unsigned long		uint32_t;
typedef unsigned long long int	uint64_t;


#endif

#endif
