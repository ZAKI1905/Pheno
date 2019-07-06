/*
  IdEff class

  Last Updated by Zaki on July 6, 2019

*/

#include <iostream>
#include <vector>
#include "Pythia8/Pythia.h"
#include "../include/EV.h"
#include "../include/IdEff.h"
#include "../include/Basics.h"
#include "../include/Cut.h"

using std::vector ;
using namespace Pythia8 ;

//==============================================================
//--------------------------------------------------------------
// Constructor
IdEff::IdEff(EV& ev) : Cut(ev) {}

//--------------------------------------------------------------
// Virtual method from cut class:
void IdEff::cut_cond(vector<ExParticle>& in_parlst)
/*
ID_eff_check: Takes an event and checks if the detector missed an
 electron or muon.
*/
{
  for(size_t i = 0 ; i < in_parlst.size() ; ++i)
  {
    if( in_parlst[i].idEff()==0 || 
        (ev_ref.drop_low_eff && in_parlst[i].idEff() <rand01()) )

      add_elem(rm_list, (int)i ) ;
  }
}

//--------------------------------------------------------------
// Overriding the input method from base class "cut".
void IdEff::input(std::string property)
{
  // Parsing the command
  std::vector<std::string> inp = pars(property, "=") ;

  if(inp[0] == "drop_low_eff")
  {
    if( inp[1] == "false" )
      ev_ref.drop_low_eff = false ;
  }

}

//--------------------------------------------------------------

//==============================================================
