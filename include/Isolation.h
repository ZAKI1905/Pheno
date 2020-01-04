#ifndef Isolation_H
#define Isolation_H

#include "Cut.h"

//==============================================================

class Isolation : public Cut
{
  //--------------------------------------------------------------
  public:

    // Constructor takes a reference to the event
    Isolation(ExEvent&) ;

  //--------------------------------------------------------------
  private:

    // Virtual method from cut class
    void CutCond(std::vector<ExParticle>&) override;   

    // These values have to be reset for each particle!
    
      // Sum of the E_T value for e & muon
      double sum_ET_value ;   
      // Sum of the E_T value for tau
      double sum_ET_tau_value ;   
};

//==============================================================
#endif /*Isolation_H*/
