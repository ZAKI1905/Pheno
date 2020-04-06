#ifndef IdEffCut_H
#define IdEffCut_H

#include "Cut.h"

//--------------------------------------------------------------
namespace PHENO
{
  class Pheno;
//--------------------------------------------------------------
namespace CUTS
{
//==============================================================
class IdEffCut : public Cut
{

// Gives access to input, etc.
friend class PHENO::Pheno ;   

//--------------------------------------------------------------
public:

  // Default Constructor
  IdEffCut() ;

  // Constructor
  IdEffCut(ExEvent*) ;

  // Copy Constructor
  IdEffCut(const IdEffCut&) ;

  // Destructor
  ~IdEffCut() ;
  
  // // Finds the ID_eff of p in ev
  // void find_id_eff(ExParticle& p) ;   

  // // equal to 0.7 for hadronic taus
  // const double tau_h_ID_eff ;   

  // // prints the eff for an event
  // void print_id_eff() ;   


//--------------------------------------------------------------
private:

  // Virtual method from cut class
  void CutCond(ParticleLST&) override ;    
  std::shared_ptr<Cut> Clone() override ;
  // // Value of the ID_Eff
  // double ID_Eff_Val ;    

  // // Value of the whole event ID_Eff
  // double ID_Eff_Event ;    

  void Input(std::string) override ;

  /* Whether drop a particle which fails efficiency test,
   or keep it but weigh the event number so 
   that it's counted as less than 1.  */
  // bool drop_low_eff = true ;   

};
//=============================================================
} // CUTS namespace
//==============================================================
} // PHENO namespace
//=============================================================
#endif /*IdEffCut_H*/
