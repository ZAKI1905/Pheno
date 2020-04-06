#ifndef PtCut_H
#define PtCut_H

#include "Cut.h"

//--------------------------------------------------------------
namespace PHENO
{
  class Pheno;

//--------------------------------------------------------------
namespace CUTS
{
//==============================================================
class PtCut : public Cut
{
  // Gives access to input, etc.
  friend class PHENO::Pheno ;   

  //--------------------------------------------------------------
  public:
    // Default constructor
    PtCut() ;

    // Constructor takes a pointer to the event
    PtCut(ExEvent*) ;

    // Copy Constructor
    PtCut(const PtCut&) ;

    // Overloading ()
    bool operator() (const ExParticle&,const ExParticle&) ;  

  //--------------------------------------------------------------
  private:

    // Virtual method from cut class
    void CutCond(ParticleLST&) override ;   
    void Input(std::string) override ; 
    std::shared_ptr<Cut> Clone() override; 

    bool Comp_pT_Value(const ExParticle&, const ExParticle& ) ;
    ParticleLST pT_Sorter(ParticleLST& ) ;

    // dictionary for the pT cut values
    float pT_CutVal(size_t , ExParticle& ) ; 


    float leading_pT_cut     = 20.0 ;
    float sub_leading_pT_cut = 10.0 ;
    float extra_pT_cut       = 10.0 ;
    float had_tau_pT_cut     = 20.0 ;
};

//=============================================================
} // CUTS namespace
//==============================================================
} // PHENO namespace
//=============================================================
#endif /*PtCut_H*/
