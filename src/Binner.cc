/*
  Binner class

  Last Updated by Zaki on July 6, 2019

*/

#include "../include/Binner.h"

//==============================================================
//--------------------------------------------------------------
// Constructor
Binner::Binner() {}

//--------------------------------------------------------------
// Virtual destructor
Binner::~Binner() { }

//--------------------------------------------------------------
// Setting the binner label
void Binner::SetName(const char* in_name) 
{
  name = in_name ;
}

//--------------------------------------------------------------
// Setting the binner label
std::string Binner::GetName() const 
{
  return name;
}

//--------------------------------------------------------------

//==============================================================