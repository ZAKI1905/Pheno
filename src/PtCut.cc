/*
  PtCut class

  Last Updated by Zaki on July 6, 2019

*/

#include "../include/PtCut.h"

//==============================================================

//--------------------------------------------------------------
// Constructor
PtCut::PtCut(ExEvent& ev) : Cut(ev) {logger.SetUnit("PtCut");}

//--------------------------------------------------------------
// Overriding the input method from base class "cut".
void PtCut::Input(std::string property)
{
  // Parsing the command
  std::vector<std::string> inp = pars(property, "=") ;

  if(inp[0] == "lead")
  {
    leading_pT_cut = std::stof (inp[1]) ;
  }
  else if(inp[0] == "sub_lead")
  {
    sub_leading_pT_cut = std::stof (inp[1]) ;
  }
  else if(inp[0] == "extra")
  {
    extra_pT_cut = std::stof (inp[1]) ;
  }
  else if(inp[0] == "had_tau")
  {
    had_tau_pT_cut = std::stof (inp[1]) ;
  }
  else
  {
    LOG_ERROR((inp[0] + " is invalid option for pT cut!").c_str()) ;
  } 
  
}

//--------------------------------------------------------------
// Virtual method from cut class:
void PtCut::CutCond(std::vector<ExParticle>& in_parlst)
{

  pT_Sorter(in_parlst)                    ;
  size_t emu_fail_idx = in_parlst.size()  ;
  size_t tau_fail_idx = in_parlst.size()  ;
  size_t tau_head_idx = in_parlst.size()  ;

  for(size_t i=0 ; i < in_parlst.size() ; ++i )
  { 
    // If pT is less than the cut
    if( in_parlst[i].GetVisMom().pT() < pT_CutVal(i+1, in_parlst[i]) )
    {
      /* It's possible that it's a tau that first fails
       the cut (unlikely), but still no problem. */
      emu_fail_idx = i ; 

      // Fastforward to taus (unless index i is already a tau):
      while (in_parlst[i].idAbs() != ID_TAU && i < in_parlst.size() )
      {
        i++ ;
      }
      // tau_h beginning index
      tau_head_idx = i ;

      // Check the pT condition this time for taus
      while (i < in_parlst.size())
      {
        if( in_parlst[i].GetVisMom().pT() < pT_CutVal(i+1, in_parlst[i]) )
        {
          tau_fail_idx = i ;
          break ;
        }
        i++ ;
      }

    }

  }

  for(size_t i=emu_fail_idx ; ( i < tau_head_idx || i >= tau_fail_idx )
       && i <  in_parlst.size() ; ++i )
  {
    /* add the indices between 
      1) the index where e-mu failed the cut to the beginning of tau_h index
      2) and from the index where tau_h fails the pT cut onwards
      to the removal list
    */
    add_elem(rm_list, (int) i) ;
  } 

}

//--------------------------------------------------------------
// Comparison function for the p_T values
// bool pT_cut::comp_pT_value(const Particle& p1, const Particle& p2)
bool PtCut::operator()(const ExParticle& p1, const ExParticle& p2)
{
    bool comp_pT_value_bool = true ;

  // If one of the particles is tau push it down
    if(p1.idAbs()==ID_TAU && p2.idAbs()!=ID_TAU)
    {
      comp_pT_value_bool = false ;

    } else if(p1.idAbs()!=ID_TAU && p2.idAbs()==ID_TAU)
    {
      comp_pT_value_bool = true ;

    } 
  // If none of the particles are tau, push the one with lower pT to higher index
    else if(p1.idAbs()!=ID_TAU && p2.idAbs()!=ID_TAU)
    {
       // const issue which might be fixed by overloading
      ExParticle tmp_p1 = p1, tmp_p2 = p2 ;
      comp_pT_value_bool = (tmp_p1.GetMom().pT() > tmp_p2.GetMom().pT()) ;
           
    } 
  // If both of the particles are tau, push the one with lower pT to higher index
    else if(p1.idAbs()==ID_TAU && p2.idAbs()==ID_TAU)
    {       
      // const issue which might be fixed by overloading
      ExParticle tmp_p1 = p1, tmp_p2 = p2 ;
      comp_pT_value_bool = (tmp_p1.GetVisMom().pT() > tmp_p2.GetVisMom().pT() ) ;
    }

    return comp_pT_value_bool ;
}

//--------------------------------------------------------------
// Sorts a list of particles based on the "func" comparison
std::vector<ExParticle> PtCut::pT_Sorter(std::vector<ExParticle>& partlist)
{ // Particle Sorter: Sorts a vector containing particle instances by their pT.

  std::sort(partlist.begin(), partlist.end(), *this ) ;
  return partlist ;
}

//--------------------------------------------------------------
// dictionary for the pT cut values
float PtCut::pT_CutVal(size_t rank, ExParticle& p)
{
  float return_val = 0  ;
  // If electron or muon
  if(p.idAbs() == ID_ELECTRON || p.idAbs() == ID_MUON )
  {
    // If leading e or mu
    if( rank == 1 ) return_val = leading_pT_cut       ;

    // If sub-leading e or mu
    if( rank == 2 ) return_val = sub_leading_pT_cut   ;

    // If extra e or mu (next-to-subleading)
    if( rank >= 3 ) return_val = extra_pT_cut         ;
  }

  /* If hadronic tau (the hadronic condition is redundant
   sometime, depending on EV::operator() method definition) */
  else if( p.idAbs() == ID_TAU && p.IsHadDec() ) 
  {
    return_val = had_tau_pT_cut ;
  }

  // If non of the above returns 0
  return return_val ;
}
//--------------------------------------------------------------

//==============================================================