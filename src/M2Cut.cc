/*
  M2Cut class

  Last Updated by Zaki on July 6, 2019

*/

#include <iostream>

#include "../include/M2Cut.h"

//==============================================================

//--------------------------------------------------------------
// Default Constructor
PHENO::CUTS::M2Cut::M2Cut()
{
  // logger.SetUnit("M2Cut");
  SetName("M2Cut") ;
}

// Constructor
PHENO::CUTS::M2Cut::M2Cut(PHENO::ExEvent* ev) : Cut(ev) 
{
  // logger.SetUnit("M2Cut");
  SetName("M2Cut") ;
}

//--------------------------------------------------------------
// Copy Constructor
PHENO::CUTS::M2Cut::M2Cut(const CUTS::M2Cut& old_obj)
  : M2_Cut_Value(old_obj.M2_Cut_Value)
{
  name = old_obj.name ;
}

//--------------------------------------------------------------
// Overriding the input method from base class "cut".
void PHENO::CUTS::M2Cut::Input(std::string property)
{
  // Parsing the command
  std::vector<std::string> inp = Zaki::String::Pars(property, "=") ;
  
  if(inp[0] == "M2_Cut_Value")
  {
    M2_Cut_Value = std::stof (inp[1]) ;
  }
  else
  { Z_LOG_ERROR((inp[0] + " is invalid option for M2 cut!").c_str()) ;
  } 
}

//--------------------------------------------------------------
// Virtual method from cut class:
void PHENO::CUTS::M2Cut::CutCond(ParticleLST& in_parlst)
{
  PROFILE_SCOPE("PHENO::CUTS::M2Cut::CutCond") ;

  char special_message_char[100] ;
  bool special_message_on = false ;

    for(size_t i=0 ; i < in_parlst.size() ; ++i )
    {
      for(size_t j=i ; j < in_parlst.size() ; ++j)
      {
        ParticleLST tmp_lst = {in_parlst[i],in_parlst[j]} ;

          if(in_parlst[i].id()==-in_parlst[j].id() && 
             invM(tmp_lst) < M2_Cut_Value)
          {
            Zaki::Vector::Add(rm_list, (int) i) ;
            Zaki::Vector::Add(rm_list, (int) j) ;

            if(report_cut)
            {
              sprintf(special_message_char,
              "\n | -(%2d,%2d) pair with invariant mass %2.3f fail this cut.  |",
              (int)i+1, (int)j+1, invM(tmp_lst)) ;

              std::string somestring(special_message_char) ;
              special_message += somestring           ;
              special_message_on = true               ;
            }
          }
      }
    

    }
    
  if(special_message_on)
  {
    // Adding the top frame
    sprintf(special_message_char," +%s+", Zaki::String::Multiply('-', 57).c_str() ) ;
    std::string somestring(special_message_char) ;
    special_message = somestring + special_message ;

    // Adding the bottom frame
    sprintf(special_message_char,"\n +%s+\n", Zaki::String::Multiply('-', 57).c_str() ) ;
    somestring = special_message_char ;
    special_message += somestring ;
  }

}

//--------------------------------------------------------------
// Overriding the clone method
std::shared_ptr<PHENO::CUTS::Cut> PHENO::CUTS::M2Cut::Clone() 
{
  return std::shared_ptr<M2Cut>(new M2Cut(*this)) ;
}

//==============================================================
