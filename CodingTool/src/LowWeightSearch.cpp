/*!
  \file LowWeightSearch.cpp
  \author Tomislav Nad, Tomislav.Nad@iaik.tugraz.at
  \version 0.9
  \brief This file contains the implementation of the class LowWeightSearch.
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


#include "LowWeightSearch.h"


LowWeightSearch::LowWeightSearch()
  : m_pCheckFunction(NULL) {
	
}

LowWeightSearch::~LowWeightSearch() {
}

CodeWord
LowWeightSearch::CanteautChabaud(CodeMatrix oGenerator, Parameters  & oParameters) {

	CodeMatrix   oZ, oZ1,oZ2;
	std::vector<uint64_t> vI1, vI2, vSigma;
	std::vector<uint64_t> vRandPerm, vGaussPerm, vColsPerm;
	std::vector<uint64_t> vMinimum;
	CodeWord  oReturn;
	CodeWord  oTempReturn;
	CodeWord  oTempWord;

	uint64_t dMaxTableSize		  = 1 << oParameters.GetIntegerParameter(Parameters::SIGMA);
	HashTableRecord** aHashTable  = new HashTableRecord*[dMaxTableSize];
	HashTableRecord * pTempRecord = NULL;
	uint64_t dTempCombination     = 0;
	uint64_t dTempHW              = 0;
	uint64_t dMinWeight           = 1000000;
	uint64_t dIterations          = 0;
	uint64_t i = 0, j = 0;

	bool bMinWeightChanged = false;
	bool bCheckFunctionResult = true;

	memset(aHashTable, NULL, dMaxTableSize * sizeof(HashTableRecord*));

	m_oOutputFile.SetParameters(oParameters);
	m_oOutputFile.Write(oParameters.GetStringParameter(Parameters::OUTPUT));
	CreateGaussMatrix(oGenerator.GetRows());

	oParameters.Print();
	std::cout << "Info: Dimension = " << oGenerator.GetRows() << ", length = " << oGenerator.GetColumns() << std::endl;
		
	if( oGenerator.GetRows() == 0 || oGenerator.GetColumns() == 0 ) {
		std::cout << "Error: Code length and/or dimension is zero." << std::endl;
		return oReturn;
	}

	if( !CheckParameters(oParameters) )
		return oReturn; // return empty code word

	// Prepare permutation vector
	for(i = 0; i < oGenerator.GetColumns(); i++)
			vColsPerm.push_back( i );

	if( oParameters.GetIntegerParameter(Parameters::PERMUTE) ) {
		vRandPerm  = RandomPermuteColumns(oGenerator);
	}

	if(oGenerator.IsSystematic())
		std::cout << "Info: Generator matrix is systematic" << std::endl;
	else {
		std::cout << "Info: Generator matrix is not systematic" << std::endl;
		vGaussPerm = GaussMod2(oGenerator);
	}


	// Extract Z
	std::vector<uint64_t> vRowsZ,vColsZ;
	for(i = 0; i < oGenerator.GetRows(); i++)
		vRowsZ.push_back( i );
	for(i = oGenerator.GetRows(); i < oGenerator.GetColumns(); i++)
		vColsZ.push_back( i );

	oZ = oGenerator.GetSubMatrix(vRowsZ,vColsZ);
	
	for(i = 0; i < floor(oGenerator.GetRows()/2.0); i++) {
		vI1.push_back(i);
		vI2.push_back(static_cast<uint64_t>(floor(oGenerator.GetRows()/2.0)+i));
	}
	
	std::cout << std::endl << "iteration" << "\t" << "current minimum" << "\t" << std::endl;
	while(dIterations < oParameters.GetIntegerParameter(Parameters::ITER)) {

		vSigma.clear();
		
		// select sigma subset
		vSigma = m_oRnGen.getRandomPosVector(0, oZ.GetColumns()-1,oParameters.GetIntegerParameter(Parameters::SIGMA));
		
		// build Z1 and Z2
		oZ1 = oZ.GetSubMatrix(vI1,vSigma);
		oZ2 = oZ.GetSubMatrix(vI2,vSigma);
	
		// compute hash table for p=2, Z1
		for(i = 0; i< oZ1.GetRows()-1; i++) {
			// NOTE: CodeMatrix has < sigma columns, which is max 30
			dTempCombination = oZ1.At64(i,0);

			pTempRecord = new HashTableRecord;
			pTempRecord->dRow1 = i;
			pTempRecord->dRow2 = 0;
			pTempRecord->dRows = 1;
			pTempRecord->pNextRecord = aHashTable[dTempCombination];
			aHashTable[dTempCombination] = pTempRecord;

			for(j = i+1; j< oZ1.GetRows(); j++) {

				dTempCombination = oZ1.At64(i,0) ^ oZ1.At64(j,0);

				pTempRecord = new HashTableRecord;
				pTempRecord->dRow1 = i;
				pTempRecord->dRow2 = j;
				pTempRecord->dRows = 2;
				pTempRecord->pNextRecord = aHashTable[dTempCombination];
				aHashTable[dTempCombination] = pTempRecord;

			}
		}
		
		// compare Z1 and Z2 online
		for(i = 0; i< oZ2.GetRows()-1; i++)
		{
			dTempCombination = oZ2.At64(i,0);

			// check if value is in table
			if( aHashTable[dTempCombination] != NULL) {
				pTempRecord = aHashTable[dTempCombination];

				// compute HW
				oTempWord  = oZ[vI2[i]];
				oTempWord ^= oZ[vI1[pTempRecord->dRow1]];
				if( pTempRecord->dRows == 2)
					oTempWord ^= oZ[vI1[pTempRecord->dRow2]];
					
				if( m_vWeights.size() == 0 )
					dTempHW = oTempWord.GetHammingWeight();
				else
					dTempHW = oTempWord.GetHammingWeight(m_vWeights);
					
				dTempHW += pTempRecord->dRows + 1;

				if( dTempHW < dMinWeight ) {

					bMinWeightChanged = true;
					vMinimum.clear();
					vMinimum.push_back(vI1[pTempRecord->dRow1]);
					if( pTempRecord->dRows == 2)
						vMinimum.push_back(vI1[pTempRecord->dRow2]);
					vMinimum.push_back(vI2[i]);

					oTempReturn = BuildMinVector(vMinimum,oZ,vColsPerm, vGaussPerm, vRandPerm);
					if(m_pCheckFunction != NULL)
						bCheckFunctionResult = m_pCheckFunction(oTempReturn);

					if(bCheckFunctionResult) {
						m_vCombinedRows = vMinimum;
						dMinWeight = dTempHW;
						oReturn = oTempReturn;
						m_oOutputFile.WriteCodeWord(oReturn);
						// stop the search if given minimum is reached
						if(dMinWeight <= oParameters.GetIntegerParameter(Parameters::MINIMUM)) {
							FreeHashTable(aHashTable,dMaxTableSize);
							return oReturn;
						}
					}
				}
			}
			

			for(j = i+1; j< oZ2.GetRows(); j++) {
				dTempCombination = oZ2.At64(i,0) ^ oZ2.At64(j,0);

				if( aHashTable[dTempCombination] != NULL) {
					pTempRecord = aHashTable[dTempCombination];
					// compute HW
					oTempWord  = oZ[vI2[i]] ^ oZ[vI2[j]] ^ oZ[vI1[pTempRecord->dRow1]];
					if( pTempRecord->dRows == 2)
						oTempWord ^= oZ[vI1[pTempRecord->dRow2]];
					
					if( m_vWeights.size() == 0 )
						dTempHW = oTempWord.GetHammingWeight();
					else
						dTempHW = oTempWord.GetHammingWeight(m_vWeights);
					dTempHW += pTempRecord->dRows + 2;

					if( dTempHW < dMinWeight ) {

						bMinWeightChanged = true;
						vMinimum.clear();
						vMinimum.push_back(vI1[pTempRecord->dRow1]);
						if( pTempRecord->dRows == 2)
							vMinimum.push_back(vI1[pTempRecord->dRow2]);
						vMinimum.push_back(vI2[i]);
						vMinimum.push_back(vI2[j]);

						oTempReturn = BuildMinVector(vMinimum,oZ,vColsPerm, vGaussPerm, vRandPerm);

						if(m_pCheckFunction != NULL)
							bCheckFunctionResult = m_pCheckFunction(oTempReturn);

						if(bCheckFunctionResult) {
							m_vCombinedRows = vMinimum;
							dMinWeight = dTempHW;
							oReturn = oTempReturn;
							m_oOutputFile.WriteCodeWord(oReturn);

							if(dMinWeight <= oParameters.GetIntegerParameter(Parameters::MINIMUM)) {
								FreeHashTable(aHashTable,dMaxTableSize);
								return oReturn;
							}
						}
					}
				}
			}
		}
		
		DeltaGauss(oZ,vColsPerm);
		FreeHashTable(aHashTable,dMaxTableSize);
		

		if ( oParameters.GetIntegerParameter(Parameters::DOUTPUT) == 0 && bMinWeightChanged) {
			std::cout << dIterations  << "\t\t" << dMinWeight << "\t\t" << std::endl;
		}
		bMinWeightChanged = false;
		dIterations++;

		if(dMinWeight == 0)
			break;
	}

	// if not one match was found return minimum row of the code matrix
	if( oReturn.GetLength() == 0 ) {
		for(i = 0; i < oGenerator.GetRows(); i++)
			if(oGenerator[i].GetHammingWeight() < dMinWeight) {
				dMinWeight = oGenerator[i].GetHammingWeight(); 
				oReturn = oGenerator[i];
				m_oOutputFile.WriteCodeWord(oReturn);
			}
	}
	FreeHashTable(aHashTable,dMaxTableSize);
	delete[] aHashTable;
	return oReturn;
}

void
LowWeightSearch::DeltaGauss(CodeMatrix & oZ, std::vector<uint64_t> & vColsPerm) {
	
	uint64_t lambda = m_oRnGen.getRandomPosInteger(oZ.GetRows());
	uint64_t mu     = m_oRnGen.getRandomPosInteger(oZ.GetColumns());
	uint64_t dIsOne = oZ.AtBool(lambda,mu);

	while(dIsOne == 0)
	{
		lambda = m_oRnGen.getRandomPosInteger(oZ.GetRows());
		mu     = m_oRnGen.getRandomPosInteger(oZ.GetColumns());
		dIsOne = oZ.AtBool(lambda,mu);
	}
	
	for(uint64_t i = 0; i < oZ.GetRows(); i++) {

		dIsOne = oZ.AtBool(i,mu);
		if(dIsOne > 0 && i != lambda) {
			CodeWord oTemp = oZ[i] ^ oZ[lambda];
			bool bTemp = oZ.AtBool(i,mu);
			
			oZ[i] = oTemp;
			oZ.SetBool(i,mu,bTemp);

			m_oGaussCombinations[i] ^= m_oGaussCombinations[lambda];
		}
	}
	uint64_t temp = vColsPerm[lambda];
	vColsPerm[lambda] = vColsPerm[mu+oZ.GetRows()];
	vColsPerm[mu+oZ.GetRows()] = temp;
}

CodeWord
LowWeightSearch::BuildMinVector(std::vector<uint64_t> & vMinimum, CodeMatrix & oZ,
		std::vector<uint64_t> & vColsPerm, std::vector<uint64_t> & vGaussPerm,
		std::vector<uint64_t> & vRandPerm) {

	CodeWord oTempWord = oZ[vMinimum[0]], oFront, oReturn;
	
	for(uint32_t i=0; i< oZ.GetRows();i++)
		oFront.PushBool(0);
	for(uint32_t i=0; i< vMinimum.size();i++)
		oFront.SetBool(vMinimum[i], 1);
	
	for(uint32_t i=1; i< vMinimum.size();i++)
		oTempWord ^= oZ[vMinimum[i]];

	for(uint32_t i=0; i< oTempWord.GetLength();i++)
		oFront.PushBool(oTempWord.AtBool(i));

	// permute back
	oReturn = oFront;

	for(uint32_t j= 0; j< vColsPerm.size() ;j++)
		oReturn.SetBool(vColsPerm[j], oFront.AtBool(j));
	
	oFront = oReturn;
	for(uint32_t j= 0; j< vGaussPerm.size() ;j++)
		oReturn.SetBool(vGaussPerm[j], oFront.AtBool(j));

	oFront = oReturn;
	for(uint64_t j= 0; j < vRandPerm.size(); j++)
		oReturn.SetBool(vRandPerm[j], oFront.AtBool(j));

	return oReturn;
}


std::vector<uint64_t>
LowWeightSearch::RandomPermuteColumns(CodeMatrix & oGenerator) {

	std::vector<uint64_t> vPerm;
	CodeMatrix oTempMatrix = oGenerator;

	vPerm = m_oRnGen.getRandomPosVector(0, oGenerator.GetColumns()-1,oGenerator.GetColumns());

	for(uint64_t i = 0; i < oGenerator.GetColumns(); i++) {
		for(uint64_t j = 0; j < oGenerator.GetRows(); j++) {
			oTempMatrix[j].SetBool(i, oGenerator[j].AtBool(vPerm[i]));
		}
	}
	oGenerator = oTempMatrix;
	return vPerm;

}

bool
LowWeightSearch::CheckParameters(Parameters & oParameters) {
	bool bError = true;

	if( oParameters.GetIntegerParameter("-s") > 30 ) {
		std::cout << "Error: sigma is too large. The value should not exceed 30." << std::endl;
		bError = false;
	}

	return bError;

}

std::vector<uint64_t>
LowWeightSearch::GaussMod2(CodeMatrix & oMatrix) {
	
	CodeWord oTempRow;
	std::vector<uint64_t> vColChange;
	bool bFoundPivot = false;
	bool bTest = false;
	uint64_t MSB = 0x8000000000000000;
	uint16_t dWordSize = sizeof(uint64_t)*8;

	std::cout << "Info: Applying Gauss-Jordan algorithm." << std::endl;

	if( dWordSize == 64 )
		MSB = 0x8000000000000000;
	else
		MSB = 0x80000000;

	for(uint64_t i = 0; i < oMatrix.GetColumns(); i++)
			vColChange.push_back( i );

	for(uint64_t j = 0; j < oMatrix.GetRows(); j++) {

		bFoundPivot = false;
		for(uint64_t i = j; i < oMatrix.GetRows(); i++) {

			if( (oMatrix[i].At64(j/dWordSize) << (j%dWordSize)) & MSB ) {

				oTempRow = oMatrix[i];
				oMatrix[i] = oMatrix[j];
				oMatrix[j] = oTempRow;

				bFoundPivot = true;
				break;
			}
		}
	
		// swap columns, slow
		if ( !bFoundPivot ) {
			bTest = false;
	    	for(uint64_t k = j; k < oMatrix.GetColumns(); k++) {

	    		if( oMatrix[j].AtBool(k) ) {
					for(uint64_t l = 0; l < oMatrix.GetRows(); l++) {
						bool dTemp = oMatrix[l].AtBool(k);
						oMatrix[l].SetBool(k, oMatrix[l].AtBool(j));
						oMatrix[l].SetBool(j, dTemp);
					}
					uint64_t dTemp2 = vColChange[k];
					vColChange[k] = vColChange[j];
					vColChange[j] = dTemp2;
					bTest = true;
					break;
	    		}
	    	}
	    	if(bTest == false){
				std::cout << "Error: Matrix has not full rank." << std::endl;
	    	}
		}
		
		for(uint64_t i =0; i < oMatrix.GetRows(); i++) {

			if(i==j)
				continue;
			if( (oMatrix[i].At64(j/dWordSize) << (j%dWordSize)) & MSB ) {
				oMatrix[i] ^= oMatrix[j];
				m_oGaussCombinations[i] ^= m_oGaussCombinations[j];
			}
		}
	}

	return vColChange;
}


CodeMatrix
LowWeightSearch::CodeShortening(CodeMatrix & oMatrix, std::vector<uint64_t> & vColumns){

	CodeMatrix oTempMatrix = oMatrix;

	bool bPivotFound = false;
	uint64_t dPivotRow = 0;

	for(unsigned int j = 0; j < vColumns.size(); j++) {
		bPivotFound = false;
		for(unsigned int i = 0; i < oTempMatrix.GetRows(); i++) {
			if(!bPivotFound) {
				if(oTempMatrix[i].AtBool(vColumns[j]) == 1) {
					bPivotFound = true;
					dPivotRow = i;
					i = 0;
				}
			}
			else {
				if( i != dPivotRow ) {
					if(oTempMatrix[i].AtBool(vColumns[j])) {
						oTempMatrix[i] ^= oTempMatrix[dPivotRow];
					}
				}
			}

		}
		if(bPivotFound)
			oTempMatrix.DeleteRow(dPivotRow);
		
	}
	if( oTempMatrix.GetRows() == 0 ) {
		std::cout << "Error: To much columns forced to zero. Resulting matrix is empty" << std::endl;
	}
	else {
		for(unsigned int i = 0; i < vColumns.size(); i++)
			oTempMatrix.DeleteColumn(vColumns[i]-i);
	}

	return oTempMatrix;
}


void 
LowWeightSearch::AddInformation(const std::string & sInfo) {
	m_oOutputFile.AddText(sInfo);
}


void
LowWeightSearch::SetCheckFunction(bool (*pCheckFunction)(CodeWord&)) {
	m_pCheckFunction = pCheckFunction;
}


std::vector<uint64_t>
LowWeightSearch::GetCombinedRows() {

	return m_vCombinedRows;
}

void
LowWeightSearch::CreateGaussMatrix(uint64_t dDim) {

	CodeMatrix oTemp;

	for(uint64_t i = 0; i < dDim; i++){
		CodeWord oTempWord;
		for(uint64_t j = 0; j < ceil(static_cast<double>(dDim)/(sizeof(uint64_t)*8)); j++)
			oTempWord.Push64(0);
		oTempWord.SetBool(i,1);
		oTemp.AddRow(oTempWord);
	}

	m_oGaussCombinations = oTemp;

}

CodeMatrix &
LowWeightSearch::GetGaussCombinations() {
	return m_oGaussCombinations;
}

CodeMatrix
LowWeightSearch::CheckToGenerator(CodeMatrix & oCheckMatrix) {

	CodeMatrix oGenerator;

	for(uint64_t i = 0; i < oCheckMatrix.GetColumns()-oCheckMatrix.GetRows(); i++) {
		CodeWord oTempWord;
		for(uint64_t j = 0; j < oCheckMatrix.GetColumns(); j++)
			oTempWord.PushBool(0);
	}
	for(uint64_t i = 0; i < oGenerator.GetRows(); i++)
		oGenerator[i].SetBool(i, 1);
		
	for(uint64_t i = 0; i < oGenerator.GetRows(); i++) {
		for(uint64_t j = oGenerator.GetRows(); j < oGenerator.GetColumns(); j++)
			oGenerator[i].SetBool(j, oCheckMatrix[j-oGenerator.GetRows()].AtBool(i));
	}

	return oGenerator;
}

void
LowWeightSearch::FreeHashTable(HashTableRecord** aHashTable,uint64_t dMaxTableSize) {
	HashTableRecord * pTempRecord = NULL;
	for(uint64_t i = 0; i < dMaxTableSize; i++) {
		pTempRecord = aHashTable[i];
		while(pTempRecord != NULL) {
			aHashTable[i] = pTempRecord->pNextRecord;
			delete pTempRecord;
			pTempRecord = aHashTable[i];
		}
		delete aHashTable[i];
	}
	memset(aHashTable, NULL, dMaxTableSize * sizeof(HashTableRecord*));
}

void
LowWeightSearch::SetWeightVector(std::vector<uint64_t> & vWeights) {
	m_vWeights = vWeights;
}

// EOF
