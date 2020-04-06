#ifndef PrapCut_H
#define PrapCut_H

#include "Cut.h"

//--------------------------------------------------------------
namespace PHENO
{
  class Pheno;

//--------------------------------------------------------------
namespace CUTS
{
//==============================================================
class PrapCut : public Cut
{

  // Gives access to input, etc.
  friend class PHENO::Pheno ;   

  //--------------------------------------------------------------
  public:
    // Default constructor
    PrapCut() ;

    // Constructor takes a pointer to the event
    PrapCut(ExEvent*) ;
    
    // Copy Constructor
    PrapCut(const PrapCut&) ;

  //--------------------------------------------------------------
  private:
  
    // Virtual method from cut class
    void CutCond(ParticleLST&) override ;   
    void Input(std::string) override ;  
    std::shared_ptr<Cut> Clone() override; 

    // Pseudorapidity cut values
    double e_prap_cut   = 1000 ;
    double mu_prap_cut  = 1000 ;
    double tau_prap_cut = 1000 ;
};

//=============================================================
} // CUTS namespace
//==============================================================
} // PHENO namespace
//=============================================================
#endif /*PrapCut_H*/
