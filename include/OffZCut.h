#ifndef OffZCut_H
#define OffZCut_H

// #include <vector>
// #include "Pythia8/Pythia.h"
#include "Cut.h"

// using namespace Pythia8 ;

//==============================================================
class OffZCut : public Cut
{
  
  // Gives access to input, etc.
  friend class Pheno ;   

  //--------------------------------------------------------------

  public:

    // Constructor takes a reference to the event
    OffZCut(EV&) ;
   

  //--------------------------------------------------------------

  private:

    // Virtual method from cut class
    void cut_cond(std::vector<ExParticle>&) ;   
    void input(std::string) ; 

    // The minimum of OffZ cut
    float OffZ_cut_min = 75 ;   

    // The maximum of OffZ cut
    float OffZ_cut_max = 105 ;   

};

//==============================================================
#endif /*OffZCut_H*/
