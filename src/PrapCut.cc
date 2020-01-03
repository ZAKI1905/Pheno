/*
  PrapCut class

  Last Updated by Zaki on July 6, 2019

*/

// #include <iostream>
// #include <vector>
// #include "Pythia8/Pythia.h"

// #include "../inc/EV.h"
#include "../include/PrapCut.h"
// #include "../inc/Basics.h"
// #include "../inc/Cut.h"

using namespace Pythia8 ;

//==============================================================

//--------------------------------------------------------------
// Constructor
PrapCut::PrapCut(EV& ev) : Cut(ev) {logger.SetUnit("PrapCut");}

//--------------------------------------------------------------
// Overriding the input method from base class "cut".
void PrapCut::input(std::string property)
{
  // Parsing the command
  std::vector<std::string> inp = pars(property, "=") ;
  
  if(inp[0] == "e")
  {
    e_prap_cut = std::stof (inp[1]) ;
  }
  else if(inp[0] == "mu")
  {
    mu_prap_cut = std::stof (inp[1]) ;
  }
  else if(inp[0] == "had_tau")
  {
    tau_prap_cut = std::stof (inp[1]) ;
  }

  else
  {
    std::cerr<<endl<<inp[0]<<" is invalid option for PRap cut!"<<std::flush ;
  } 

}

//--------------------------------------------------------------
// Virtual method from cut class:
void PrapCut::cut_cond(vector<ExParticle>& in_parlst)
{

    for(size_t i=0 ; i< in_parlst.size() ; ++i)
    {   
        // eta_e < e_prap_cut
        //-----------------------------------------------
        if( in_parlst[i].idAbs()==ID_ELECTRON )
        {  
          if(  abs( in_parlst[i].visMom().eta() ) > e_prap_cut  )
            add_elem(rm_list, (int) i) ;            
        }

        //-----------------------------------------------
        // eta_mu < mu_prap_cut
        if( in_parlst[i].idAbs()==ID_MUON )
        {  
          if(  abs( in_parlst[i].visMom().eta() ) > mu_prap_cut  )
            add_elem(rm_list, (int) i) ;           
        }

        //-----------------------------------------------
        // eta_tau_h < tau_prap_cut
        if( in_parlst[i].idAbs()==ID_TAU && in_parlst[i].isHadDec() )
        {  
          if(  abs( in_parlst[i].visMom().eta() ) > tau_prap_cut  )
            add_elem(rm_list, (int) i) ;           
        }
        //-----------------------------------------------
    }
}

//--------------------------------------------------------------

//==============================================================