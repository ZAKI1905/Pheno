#ifndef M2Cut_H
#define M2Cut_H

// #include <vector>
// #include "Pythia8/Pythia.h"
#include "Cut.h"

// using namespace Pythia8 ;

//==============================================================
class M2Cut : public Cut
{
  
  // Gives access to input, etc.
  friend class Pheno ;   

  //--------------------------------------------------------------
  public:

    // Constructor takes a reference to the event
    M2Cut(EV&) ;
   
  //--------------------------------------------------------------
  private:

    // Virtual method from cut class
    void cut_cond(std::vector<ExParticle>&) ;   
    void input(std::string) ; 
    
    // The cut on m2
    float M2_Cut_Value = 0 ;   

};

//==============================================================
#endif /*M2Cut_H*/
