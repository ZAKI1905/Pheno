/*
  PrapCut class

  Last Updated by Zaki on July 6, 2019

*/

#include "../include/PrapCut.h"

//==============================================================

//--------------------------------------------------------------
// Default Constructor
PHENO::CUTS::PrapCut::PrapCut()
{
  // logger.SetUnit("PrapCut");
  SetName("PrapCut") ;
}

// Constructor
PHENO::CUTS::PrapCut::PrapCut(ExEvent* ev) : Cut(ev) 
{
  // logger.SetUnit("PrapCut");
  SetName("PrapCut") ;
}

//--------------------------------------------------------------
// Copy Constructor
PHENO::CUTS::PrapCut::PrapCut(const PrapCut& old_obj)
  : e_prap_cut(old_obj.e_prap_cut), 
    mu_prap_cut(old_obj.mu_prap_cut),
    tau_prap_cut(old_obj.tau_prap_cut)
{
  name = old_obj.name ;
}

//--------------------------------------------------------------
// Overriding the input method from base class "cut".
void PHENO::CUTS::PrapCut::Input(std::string property)
{
  // Parsing the command
  std::vector<std::string> inp = Zaki::String::Pars(property, "=") ;
  
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
    Z_LOG_ERROR((inp[0] + " is invalid option for PRap cut!").c_str()) ;
  } 

}

//--------------------------------------------------------------
// Virtual method from cut class:
void PHENO::CUTS::PrapCut::CutCond(ParticleLST& in_parlst)
{
  PROFILE_SCOPE("PHENO::CUTS::PrapCut::CutCond") ;

    for(size_t i=0 ; i< in_parlst.size() ; ++i)
    {   
        // eta_e < e_prap_cut
        //-----------------------------------------------
        if( in_parlst[i].idAbs()==ID_ELECTRON )
        {  
          if(  abs( in_parlst[i].GetVisMom().eta() ) > e_prap_cut  )
            Zaki::Vector::Add(rm_list, (int) i) ;            
        }

        //-----------------------------------------------
        // eta_mu < mu_prap_cut
        if( in_parlst[i].idAbs()==ID_MUON )
        {  
          if(  abs( in_parlst[i].GetVisMom().eta() ) > mu_prap_cut  )
            Zaki::Vector::Add(rm_list, (int) i) ;           
        }

        //-----------------------------------------------
        // eta_tau_h < tau_prap_cut
        if( in_parlst[i].idAbs()==ID_TAU && in_parlst[i].IsHadDec() )
        {  
          if(  abs( in_parlst[i].GetVisMom().eta() ) > tau_prap_cut  )
            Zaki::Vector::Add(rm_list, (int) i) ;           
        }
        //-----------------------------------------------
    }
}

//--------------------------------------------------------------
// Overriding the clone method
std::shared_ptr<PHENO::CUTS::Cut> PHENO::CUTS::PrapCut::Clone() 
{
  return std::shared_ptr<PrapCut>(new PrapCut(*this)) ;
}

//==============================================================