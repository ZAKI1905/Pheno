#ifndef Isolation_H
#define Isolation_H

// #include <vector>
// #include "Pythia8/Pythia.h"
// #include "Basics.h"
#include "Cut.h"

// using std::vector ;
// using namespace Pythia8 ;

//==============================================================

class Isolation : public Cut
{
  //--------------------------------------------------------------

  public:

    // Constructor takes a reference to the event
    Isolation(EV&) ;

  //--------------------------------------------------------------

  private:

    // Virtual method from cut class
    void cut_cond(std::vector<ExParticle>&) ;   

    // These values have to be reset for each particle!
    
      // Sum of the E_T value for e & muon
      double sum_ET_value ;   
      // Sum of the E_T value for tau
      double sum_ET_tau_value ;   

};

//==============================================================
#endif /*Isolation_H*/
