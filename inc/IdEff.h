#ifndef IdEff_H
#define IdEeff_H

#include <vector>
#include "Pythia8/Pythia.h"
#include "Basics.h"
#include "Cut.h"

using    std::vector ;
using    namespace Pythia8 ;

//==============================================================
class IdEff : public Cut
{

// Gives access to input, etc.
friend class Pheno ;   

//--------------------------------------------------------------

public:

  // Constructor
  IdEff(EV &) ;

  // // Finds the ID_eff of p in ev
  // void find_id_eff(ExParticle& p) ;   

  // // equal to 0.7 for hadronic taus
  // const double tau_h_ID_eff ;   

  // // prints the eff for an event
  // void print_id_eff() ;   


//--------------------------------------------------------------

private:

  // Virtual method from cut class
  void cut_cond(vector<ExParticle>&) ;    

  // // Value of the ID_Eff
  // double ID_Eff_Val ;    

  // // Value of the whole event ID_Eff
  // double ID_Eff_Event ;    


  void input(std::string) ;

  /* Whether drop a particle which fails efficiency test,
   or keep it but weigh the event number so 
   that it's counted as less than 1.  */
  // bool drop_low_eff = true ;   

};

//==============================================================
#endif /*IdEff_H*/
