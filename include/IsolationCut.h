#ifndef IsolationCut_H
#define IsolationCut_H

#include "Cut.h"

//--------------------------------------------------------------
namespace PHENO
{
//--------------------------------------------------------------
namespace CUTS
{
//==============================================================
class IsolationCut : public Cut
{
  //--------------------------------------------------------------
  public:
    // Default Constructor
    IsolationCut() ;

    // Constructor takes a pointer to the event
    IsolationCut(ExEvent*) ;

    // Copy Constructor
    IsolationCut(const IsolationCut&) ;

  //--------------------------------------------------------------
  private:

    // Virtual method from cut class
    void CutCond(ParticleLST&) override;  
    std::shared_ptr<Cut> Clone() override; 

    // These values have to be reset for each particle!
    
      // Sum of the E_T value for e & muon
      double sum_ET_value ;   
      // Sum of the E_T value for tau
      double sum_ET_tau_value ;   
};

//=============================================================
} // CUTS namespace
//==============================================================
} // PHENO namespace
//=============================================================
#endif /*IsolationCut_H*/
