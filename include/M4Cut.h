#ifndef M4Cut_H
#define M4Cut_H

#include "Cut.h"

//==============================================================
class M4Cut : public Cut
{
  
  // Gives access to input, etc.
  friend class Pheno ;   

  //--------------------------------------------------------------
  public:
    // Constructor takes a reference to the event
    M4Cut(ExEvent&) ;
   
  //--------------------------------------------------------------
  private:

    // Virtual method from cut class
    void CutCond(std::vector<ExParticle>&) override;   
    void Input(std::string) override; 

    // The minimum of m4l
    float M4_cut_min = 0 ;   

    // The maximum of m4l
    float M4_cut_max = 10000 ;   

    // Sorts particles based on pT
    void pT_Sort(std::vector<ExParticle>&) ;
};

//==============================================================
#endif /*M4Cut_H*/
