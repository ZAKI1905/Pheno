/*
  M2Cut class

  Last Updated by Zaki on July 6, 2019

*/

#include <iostream>
#include <vector>
#include "Pythia8/Pythia.h"
#include "../include/EV.h"
#include "../include/M2Cut.h"
#include "../include/Basics.h"
#include "../include/Cut.h"

using namespace Pythia8 ;

//==============================================================

//--------------------------------------------------------------
// Constructor
M2Cut::M2Cut(EV& ev) : Cut(ev) {}

//--------------------------------------------------------------
// Overriding the input method from base class "cut".
void M2Cut::input(std::string property)
{
  // Parsing the command
  std::vector<std::string> inp = pars(property, "=") ;
  
  if(inp[0] == "M2_Cut_Value")
  {
    M2_Cut_Value = std::stof (inp[1]) ;
  }
  else
  { std::cerr<<endl<<inp[0]<<" is invalid option for M2 cut!"<<std::flush ;
  } 
}

//--------------------------------------------------------------
// Virtual method from cut class:
void M2Cut::cut_cond(vector<ExParticle>& in_parlst)
{
  char special_message_char[100] ;
  bool special_message_on = false ;

    for(size_t i=0 ; i < in_parlst.size() ; ++i )
    {
      for(size_t j=i ; j < in_parlst.size() ; ++j)
      {
        vector<ExParticle> tmp_lst = {in_parlst[i],in_parlst[j]} ;

          if(in_parlst[i].id()==-in_parlst[j].id() && 
             invM(tmp_lst) < M2_Cut_Value)
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