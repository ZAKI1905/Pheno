/*
  M4Cut class

  Last Updated by Zaki on July 6, 2019

*/

#include <iostream>
#include <vector>
#include "Pythia8/Pythia.h"
#include "../include/EV.h"
#include "../include/M4Cut.h"
#include "../include/Basics.h"
#include "../include/Cut.h"

using namespace Pythia8 ;

//==============================================================

//--------------------------------------------------------------
// Constructor
M4Cut::M4Cut(EV& ev) : Cut(ev) {}

//--------------------------------------------------------------
// Overriding the input method from base class "cut".
void M4Cut::input(std::string property)
{
  // Parsing the command
  std::vector<std::string> inp = pars(property, "=") ;
  
  if(inp[0] == "M4_Cut_Min")
  {
    M4_cut_min = std::stof (inp[1]) ;
  }
  else if(inp[0] == "M4_Cut_Max")
  {
    M4_cut_max= std::stof (inp[1]) ;
  }
  else
  { std::cerr<<endl<<inp[0]<<" is invalid option for M4 cut!"<<std::flush ;
  } 
}

//--------------------------------------------------------------
// Virtual method from cut class:
void M4Cut::cut_cond(vector<ExParticle>& in_parlst)
{

  // Calculate the invariant mass of the 
  // 4 leptons
  double tmp_m = invM(in_parlst) ;

  // For now we get rid of the events with size > 4.
  if ( in_parlst.size() > 4 ||  tmp_m < M4_cut_min || tmp_m > M4_cut_max )
  {
    for(size_t i=0 ; i < in_parlst.size() ; ++i )
    {
      add_elem(rm_list, (int) i) ;
    }
  } else return ;

  char special_message_char[100] ;
  if(report_cut)
  {
    sprintf(special_message_char,
    "\n | - This event with invariant mass %2.2f GeV fails this cut.  |",
     tmp_m) ;

    string somestring(special_message_char) ;
    special_message += somestring ;

    // Adding the top frame
    sprintf(special_message_char," +%s+", pr(62, '-').c_str() ) ;
    somestring = special_message_char ;
    special_message = somestring + special_message  ;

    // Adding the bottom frame
    sprintf(special_message_char,"\n +%s+\n", pr(62, '-').c_str() ) ;
    somestring = special_message_char ;
    special_message += somestring ;
  }

}

//--------------------------------------------------------------
// Sorts particles according to pT
void M4Cut::pT_sort(vector<ExParticle>& in_list)
{
  std::sort(in_list.begin(), in_list.end(),
            [](ExParticle& a, ExParticle& b) 
            {return a.mom().pT() > b.mom().pT() ; } ) ;
}

//==============================================================
