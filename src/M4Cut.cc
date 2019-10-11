/*
  M4Cut class

  Last Updated by Zaki on July 6, 2019

*/

#include <iostream>
#include <vector>
#include "Pythia8/Pythia.h"
#include "../inc/EV.h"
#include "../inc/M4Cut.h"
#include "../inc/Basics.h"
#include "../inc/Cut.h"

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

  //************************************************************
  //           Special Colinear Approximation 
  //************************************************************

//   if (in_parlst.size() != 4)  return;

//   vector<ExParticle> sel_emu ;
//   vector<ExParticle> sel_taumu ;
//   int tau_sign;

//   for (size_t i=0 ; i<in_parlst.size() ; ++i)
//   {
//     if ( in_parlst[i].idAbs() == ID_TAU)
//     {
//       tau_sign = (in_parlst[i].id() / in_parlst[i].idAbs()) ;
//       sel_taumu.push_back(in_parlst[i]) ;
//     }
//     if ( in_parlst[i].idAbs() == ID_ELECTRON)
//       sel_emu.push_back(in_parlst[i]) ;
    
//   }

//  for (size_t i=0 ; i<in_parlst.size() ; ++i)
//   {
//     if ( in_parlst[i].idAbs() == ID_MUON ) 
//       {
//         if( (in_parlst[i].id() / in_parlst[i].idAbs()) == tau_sign )
//           sel_emu.push_back(in_parlst[i]) ;
//         else
//           sel_taumu.push_back(in_parlst[i]) ;
//       }
//   }

//   if (sel_emu.size() != 2 || sel_taumu.size() != 2)  return ;

//   double tauMuInv = invM(sel_taumu) ;
//   double eMuInv = invM(sel_emu) ;
//   double momProd = (sel_emu[0].mom()*sel_emu[1].mom());

//   double eps = ( pow(tauMuInv, 2) - pow(eMuInv, 2) ) / (2*momProd) ;

//   Vec4 tmp_vec4 =  (1+eps)*sel_emu[0].mom() + sel_emu[1].mom() ;
//   Vec4 tmp_total_vec4 = tmp_vec4 + sel_taumu[0].mom() +  sel_taumu[1].mom() ;

//   double tmp_m = tmp_total_vec4.mCalc() ;

  //************************************************************
  //************************************************************

  // Calculate the invariant mass of the 
  // 4 leptons
  double tmp_m = invM(in_parlst) ;

  // For now we get rid of the events with size != 4.
  if ( in_parlst.size() != 4 ||  tmp_m < M4_cut_min || tmp_m > M4_cut_max )
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
