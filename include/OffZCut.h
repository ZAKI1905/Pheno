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

    // Constructor takes a reference to the event
    OffZCut(ExEvent&) ;

  //--------------------------------------------------------------
  private:

    // Virtual method from cut class
    void CutCond(std::vector<ExParticle>&) override;   
    void Input(std::string) override; 

    // The minimum of OffZ cut
    float OffZ_cut_min = 75 ;   

    // The maximum of OffZ cut
    float OffZ_cut_max = 105 ;   
};

//==============================================================
#endif /*OffZCut_H*/
