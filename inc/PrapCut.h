#ifndef PrapCut_H
#define PrapCut_H

#include <vector>
#include "Pythia8/Pythia.h"
#include "Cut.h"

using namespace Pythia8 ;

//==============================================================
class PrapCut : public Cut
{

  // Gives access to input, etc.
  friend class Pheno ;   

  //--------------------------------------------------------------

  public:

    // Constructor takes a reference to the event
      PrapCut(EV&) ;
    

  //--------------------------------------------------------------

  private:
  
    // Virtual method from cut class
    void cut_cond(vector<ExParticle>&) ;   
    void input(std::string) ;  

    // Pseudorapidity cut values
    double e_prap_cut   = 1000 ;
    double mu_prap_cut  = 1000 ;
    double tau_prap_cut = 1000 ;

};

//==============================================================

#endif /*PrapCut_H*/
