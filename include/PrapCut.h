#ifndef PrapCut_H
#define PrapCut_H

#include "Cut.h"

//==============================================================
class PrapCut : public Cut
{

  // Gives access to input, etc.
  friend class Pheno ;   

  //--------------------------------------------------------------
  public:
    // Default constructor
    PrapCut() ;

    // Constructor takes a pointer to the event
    PrapCut(ExEvent*) ;
    
  //--------------------------------------------------------------
  private:
  
    // Virtual method from cut class
    void CutCond(std::vector<ExParticle>&) override ;   
    void Input(std::string) override ;  
    Cut* Clone() override; 

    // Pseudorapidity cut values
    double e_prap_cut   = 1000 ;
    double mu_prap_cut  = 1000 ;
    double tau_prap_cut = 1000 ;
};

//==============================================================

#endif /*PrapCut_H*/
