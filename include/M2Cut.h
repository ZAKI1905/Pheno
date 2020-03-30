#ifndef M2Cut_H
#define M2Cut_H

#include "Cut.h"

//==============================================================
class M2Cut : public Cut
{
  
  // Gives access to input, etc.
  friend class Pheno ;   

  //--------------------------------------------------------------
  public:
    // Default constructor
    M2Cut() ;
    
    // Constructor takes a pointer to the event
    M2Cut(ExEvent*) ;
   
  //--------------------------------------------------------------
  private:

    // Virtual method from cut class
    void CutCond(ParticleLST&) override ;   
    void Input(std::string) override ; 
    Cut* Clone() override; 
    
    // The cut on m2
    float M2_Cut_Value = 0 ;   
};

//==============================================================
#endif /*M2Cut_H*/
