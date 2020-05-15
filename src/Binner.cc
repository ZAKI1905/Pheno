/*
  Binner class

  Last Updated by Zaki on July 6, 2019

*/

#include "../include/Binner.h"

//==============================================================
//--------------------------------------------------------------
// Constructor
PHENO::Binner::Binner() {}

//--------------------------------------------------------------
// Virtual destructor
PHENO::Binner::~Binner() { }

//--------------------------------------------------------------
// Setting the binner label
void PHENO::Binner::SetName(const char* in_name) 
{
  name = in_name ;
}

//--------------------------------------------------------------
// Setting the binner label
std::string PHENO::Binner::GetName() const 
{
  return name;
}

//--------------------------------------------------------------
std::unique_ptr<PHENO::Binner> PHENO::Binner::Clone() const 
{
  return std::unique_ptr<Binner>(IClone());
}
//==============================================================