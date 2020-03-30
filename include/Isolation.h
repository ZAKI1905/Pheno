#ifndef Isolation_H
#define Isolation_H

#include "Cut.h"

//==============================================================

class Isolation : public Cut
{
  //--------------------------------------------------------------
  public:
    // Default Constructor
    Isolation() ;

    // Constructor takes a pointer to the event
    Isolation(ExEvent*) ;

  //--------------------------------------------------------------
  private:

    // Virtual method from cut class
    void CutCond(ParticleLST&) override;  
    Cut* Clone() override; 

    // These values have to be reset for each particle!
    
      // Sum of the E_T value for e & muon
      double sum_ET_value ;   
      // Sum of the E_T value for tau
      double sum_ET_tau_value ;   
};

//==============================================================
#endif /*Isolation_H*/
