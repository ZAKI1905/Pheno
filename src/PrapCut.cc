/*
  PrapCut class

  Last Updated by Zaki on July 6, 2019

*/

#include "../include/PrapCut.h"

//==============================================================

//--------------------------------------------------------------
// Default Constructor
PrapCut::PrapCut()
{
  logger.SetUnit("PrapCut");
  SetName("PrapCut") ;
}

// Constructor
PrapCut::PrapCut(ExEvent* ev) : Cut(ev) 
{
  logger.SetUnit("PrapCut");
  SetName("PrapCut") ;
}

//--------------------------------------------------------------
// Overriding the input method from base class "cut".
void PrapCut::Input(std::string property)
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
    LOG_ERROR((inp[0] + " is invalid option for PRap cut!").c_str()) ;
  } 

}

//--------------------------------------------------------------
// Virtual method from cut class:
void PrapCut::CutCond(ParticleLST& in_parlst)
{
  PROFILE_SCOPE("PrapCut::CutCond") ;

    for(size_t i=0 ; i< in_parlst.size() ; ++i)
    {   
        // eta_e < e_prap_cut
        //-----------------------------------------------
        if( in_parlst[i].idAbs()==ID_ELECTRON )
        {  
          if(  abs( in_parlst[i].GetVisMom().eta() ) > e_prap_cut  )
            add_elem(rm_list, (int) i) ;            
        }

        //-----------------------------------------------
        // eta_mu < mu_prap_cut
        if( in_parlst[i].idAbs()==ID_MUON )
        {  
          if(  abs( in_parlst[i].GetVisMom().eta() ) > mu_prap_cut  )
            add_elem(rm_list, (int) i) ;           
        }

        //-----------------------------------------------
        // eta_tau_h < tau_prap_cut
        if( in_parlst[i].idAbs()==ID_TAU && in_parlst[i].IsHadDec() )
        {  
          if(  abs( in_parlst[i].GetVisMom().eta() ) > tau_prap_cut  )
            add_elem(rm_list, (int) i) ;           
        }
        //-----------------------------------------------
    }
}

//--------------------------------------------------------------
// Overriding the clone method
Cut* PrapCut::Clone() 
{
  return new PrapCut(*this) ;
}

//==============================================================