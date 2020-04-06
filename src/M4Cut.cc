/*
  M4Cut class

  Last Updated by Zaki on July 6, 2019

*/

#include "../include/M4Cut.h"

//==============================================================

//--------------------------------------------------------------
// Default Constructor
PHENO::CUTS::M4Cut::M4Cut()
{
  // logger.SetUnit("M4Cut");
  SetName("M4Cut") ;
}

// Constructor
PHENO::CUTS::M4Cut::M4Cut(PHENO::ExEvent* ev) : Cut(ev) 
{
  // logger.SetUnit("M4Cut");
  SetName("M4Cut") ;
}

//--------------------------------------------------------------
// Copy Constructor
PHENO::CUTS::M4Cut::M4Cut(const M4Cut& old_obj)
  : M4_cut_min(old_obj.M4_cut_min), M4_cut_max(old_obj.M4_cut_max)
{
  name = old_obj.name ;
}

//--------------------------------------------------------------
// Overriding the input method from base class "cut".
void PHENO::CUTS::M4Cut::Input(std::string property)
{
  // Parsing the command
  std::vector<std::string> inp = Zaki::String::Pars(property, "=") ;
  
  if(inp[0] == "M4_Cut_Min")
  {
    M4_cut_min = std::stof (inp[1]) ;
  }
  else if(inp[0] == "M4_Cut_Max")
  {
    M4_cut_max= std::stof (inp[1]) ;
  }
  else
  { std::cerr<<"\n"<<inp[0]<<" is invalid option for M4 cut!"<<std::flush ;
  } 
}

//--------------------------------------------------------------
// Virtual method from cut class:
void PHENO::CUTS::M4Cut::CutCond(ParticleLST& in_parlst)
{

  PROFILE_SCOPE("PHENO::CUTS::M4Cut::CutCond") ;

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
      Zaki::Vector::Add(rm_list, (int) i) ;
    }
  } else return ;

  char special_message_char[100] ;
  if(report_cut)
  {
    sprintf(special_message_char,
    "\n | - This event with invariant mass %2.2f GeV fails this cut.  |",
     tmp_m) ;

    std::string somestring(special_message_char) ;
    special_message += somestring ;

    // Adding the top frame
    sprintf(special_message_char," +%s+", Zaki::String::Multiply('-', 62).c_str() ) ;
    somestring = special_message_char ;
    special_message = somestring + special_message  ;

    // Adding the bottom frame
    sprintf(special_message_char,"\n +%s+\n", Zaki::String::Multiply('-', 62).c_str() ) ;
    somestring = special_message_char ;
    special_message += somestring ;
  }

}

//--------------------------------------------------------------
// Sorts particles according to pT
void PHENO::CUTS::M4Cut::pT_Sort(ParticleLST& in_list)
{
  std::sort(in_list.begin(), in_list.end(),
            [](ExParticle& a, ExParticle& b) 
            {return a.GetMom().pT() > b.GetMom().pT() ; } ) ;
}

//--------------------------------------------------------------
// Overriding the clone method
std::shared_ptr<PHENO::CUTS::Cut> PHENO::CUTS::M4Cut::Clone() 
{
  return std::shared_ptr<M4Cut>(new M4Cut(*this)) ;
}

//==============================================================
