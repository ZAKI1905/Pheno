/*
  OffZCut class

  Last Updated by Zaki on August 2, 2019

*/

#include <iostream>
#include <vector>
#include "Pythia8/Pythia.h"
#include "../inc/EV.h"
#include "../inc/OffZCut.h"
#include "../inc/Basics.h"
#include "../inc/Cut.h"
#include "../inc/GenJet.h"

using namespace Pythia8 ;

//==============================================================

//--------------------------------------------------------------
// Constructor
OffZCut::OffZCut(EV& ev) : Cut(ev) {  }

//--------------------------------------------------------------
// Overriding the input method from base class "cut".
void OffZCut::input(std::string property)
{
  // Parsing the command
  std::vector<std::string> inp = pars(property, "=") ;
  
  if(inp[0] == "OffZ_Cut_Min")
  {
    OffZ_cut_min = std::stof (inp[1]) ;
  }
  else if(inp[0] == "OffZ_Cut_Max")
  {
    OffZ_cut_max = std::stof (inp[1]) ;
  }
  else
  { std::cerr<<endl<<inp[0]<<" is invalid option for OffZ cut!"<<std::flush ;
  } 
}

//--------------------------------------------------------------
// Virtual method from cut class:
void OffZCut::cut_cond(vector<ExParticle>& in_parlst)
{
  char special_message_char[100] ;
  bool special_message_on = false ;

  for(size_t i=0 ; i < in_parlst.size() ; ++i )
  {
    for(size_t j=i ; j < in_parlst.size() ; ++j)
    {
      vector<ExParticle> tmp_lst = {in_parlst[i],in_parlst[j]} ;

        if(in_parlst[i].id()==-in_parlst[j].id() && 
            OffZ_cut_min < invM(tmp_lst) && invM(tmp_lst) < OffZ_cut_max)
        {
          add_elem(rm_list, (int) i) ;
          add_elem(rm_list, (int) j) ;

          if(report_cut)
          {
            sprintf(special_message_char,
            "\n | -(%2d,%2d) pair with invariant mass %2.3f fail this cut.  |",
            (int)i+1, (int)j+1, invM(tmp_lst)) ;

            string somestring(special_message_char) ;
            special_message += somestring           ;
            special_message_on = true               ;
          }
        }
    }
  

  }
    
  if(special_message_on)
  {
    // Adding the top frame
    sprintf(special_message_char," +%s+", pr(57, '-').c_str() ) ;
    string somestring(special_message_char) ;
    special_message = somestring + special_message ;

    // Adding the bottom frame
    sprintf(special_message_char,"\n +%s+\n", pr(57, '-').c_str() ) ;
    somestring = special_message_char ;
    special_message += somestring ;
  }

}

//--------------------------------------------------------------

//==============================================================