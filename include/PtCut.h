#ifndef PtCut_H
#define PtCut_H

#include "Cut.h"

//==============================================================
class PtCut : public Cut
{
  // Gives access to input, etc.
  friend class Pheno ;   

  //--------------------------------------------------------------
  public:
    // Default constructor
    PtCut() ;

    // Constructor takes a pointer to the event
    PtCut(ExEvent*) ;

    // Overloading ()
    bool operator() (const ExParticle&,const ExParticle&) ;  

  //--------------------------------------------------------------
  private:

    // Virtual method from cut class
    void CutCond(std::vector<ExParticle>&) override ;   
    void Input(std::string) override ; 
    Cut* Clone() override; 

    bool Comp_pT_Value(const ExParticle&, const ExParticle& ) ;
    std::vector<ExParticle> pT_Sorter(std::vector<ExParticle>& ) ;

    // dictionary for the pT cut values
    float pT_CutVal(size_t , ExParticle& ) ; 


    float leading_pT_cut     = 20.0 ;
    float sub_leading_pT_cut = 10.0 ;
    float extra_pT_cut       = 10.0 ;
    float had_tau_pT_cut     = 20.0 ;
};

//==============================================================

#endif /*PtCut_H*/
