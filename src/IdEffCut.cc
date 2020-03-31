/*
  IdEffCut class

  Last Updated by Zaki on July 6, 2019

*/

#include "../include/IdEffCut.h"

//==============================================================
//--------------------------------------------------------------
// Default Constructor
IdEffCut::IdEffCut()
{
  logger.SetUnit("IdEffCut");
  SetName("IdEffCut") ;

  // char tmp[100];
  // sprintf(tmp, " --> %p  constructor!", (void*)this) ;
  // LOG_INFO(tmp);
}

// Constructor
IdEffCut::IdEffCut(ExEvent* ev) : Cut(ev)
{
  logger.SetUnit("IdEffCut");
  SetName("IdEffCut") ;
}

//--------------------------------------------------------------
// Copy Constructor
IdEffCut::IdEffCut(const IdEffCut& old_obj)
{
  name = old_obj.name ;
}

//--------------------------------------------------------------
// Copy Constructor
IdEffCut::~IdEffCut()
{
  // char tmp[100];
  // sprintf(tmp, "--> %p  destructor!", (void*)this) ;
  // LOG_INFO(tmp);
}

//--------------------------------------------------------------
// Virtual method from cut class:
void IdEffCut::CutCond(ParticleLST& in_parlst)
/*
ID_eff_check: Takes an event and checks if the detector missed an
 electron or muon.
*/
{

  PROFILE_SCOPE("IdEffCut::CutCond");

  for(size_t i = 0 ; i < in_parlst.size() ; ++i)
  {
    // find_id_eff( in_parlst[i] ) ;
    // if(ID_Eff_Val!= 0)   ID_Eff_Event *= ID_Eff_Val ;

    if( in_parlst[i].GetIdEff()==0 || 
        (evPtr->drop_low_eff && in_parlst[i].GetIdEff() <rand01()) )

      add_elem(rm_list, (int)i ) ;
  }

  // // Setting the event weight
  // if( !drop_low_eff ) evPtr->weight = ID_Eff_Event ;

}

//--------------------------------------------------------------
// void IdEffCut::find_id_eff(ExParticle& p)
// /*
// IdEffCut(p): Takes a particle(p) and evaluates the efficiency of 
//  detectors.
// */
// {
//   if ( p.pT() < 10 )
//     {ID_Eff_Val = 0 ; return ;}

//   if (p.idAbs() == ID_ELECTRON)
//   {
//     ID_Eff_Val = 0.91 - 6.83 /(p.pT()) + 78.1/(pow(p.pT(),2)) - 731/(pow(p.pT(),3));

//   } else if(p.idAbs() == ID_MUON)
//     {
//       ID_Eff_Val = 0.94 - 2.85/(p.pT()) + 41.9/(pow(p.pT(),2)) - 227/(pow(p.pT(),3));

//     } else if(p.idAbs() == ID_TAU &&  p.isHadDec() )  // instead of ev.had_dec(p)
//       {
//         ID_Eff_Val = tau_h_ID_eff ;
//       } else   ID_Eff_Val = 1.0 ;

// }

// //--------------------------------------------------------------
// void IdEffCut::print_id_eff() {printf("\nID_EFF is: %-4.3f\n",ID_Eff_Event) ;}

//--------------------------------------------------------------
// Overriding the input method from base class "cut".
void IdEffCut::Input(std::string property)
{
  // Parsing the command
  std::vector<std::string> inp = pars(property, "=") ;

  if(inp[0] == "drop_low_eff")
  {
    if( inp[1] == "false" )
      evPtr->drop_low_eff = false ;
  }

}

//--------------------------------------------------------------
// Overriding the clone method
std::shared_ptr<Cut> IdEffCut::Clone() 
{
  return std::shared_ptr<IdEffCut>(new IdEffCut(*this)) ;
}

//==============================================================
