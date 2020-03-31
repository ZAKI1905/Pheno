#ifndef OffZCut_H
#define OffZCut_H

#include "Cut.h"

//==============================================================
class OffZCut : public Cut
{
  
  // Gives access to input, etc.
  friend class Pheno ;   

  //--------------------------------------------------------------
  public:
    // Default constructor
    OffZCut() ;

    // Constructor takes a pointer to the event
    OffZCut(ExEvent*) ;

    // Copy Constructor
    OffZCut(const OffZCut&) ;

  //--------------------------------------------------------------
  private:

    // Virtual method from cut class
    void CutCond(ParticleLST&) override;   
    void Input(std::string) override; 
    std::shared_ptr<Cut> Clone() override; 

    // The minimum of OffZ cut
    float OffZ_cut_min = 75 ;   

    // The maximum of OffZ cut
    float OffZ_cut_max = 105 ;   
};

//==============================================================
#endif /*OffZCut_H*/
