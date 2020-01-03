#ifndef PtCut_H
#define PtCut_H

// #include <vector>
// #include "Pythia8/Pythia.h"
#include "Cut.h"

// using    namespace Pythia8 ;

//==============================================================
class PtCut : public Cut
{
  // Gives access to input, etc.
  friend class Pheno ;   

  //--------------------------------------------------------------
  public:

    // Constructor takes a reference to the event
    PtCut(EV&) ;

    // Overloading ()
    bool operator() (const ExParticle&,const ExParticle&) ;  

  //--------------------------------------------------------------
  private:

    // Virtual method from cut class
    void cut_cond(std::vector<ExParticle>&) ;   
    void input(std::string) ; 

    bool comp_pT_value(const ExParticle&, const ExParticle& ) ;
    std::vector<ExParticle> pT_sorter(std::vector<ExParticle>& ) ;

    // dictionary for the pT cut values
    float pt_cut_val(size_t , ExParticle& ) ; 


    float leading_pT_cut     = 20.0 ;
    float sub_leading_pT_cut = 10.0 ;
    float extra_pT_cut       = 10.0 ;
    float had_tau_pT_cut     = 20.0 ;

};

//==============================================================

#endif /*PtCut_H*/
