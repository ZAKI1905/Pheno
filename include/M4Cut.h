#ifndef M4Cut_H
#define M4Cut_H

#include <vector>
#include "Pythia8/Pythia.h"
#include "Cut.h"

using namespace Pythia8 ;

//==============================================================
class M4Cut : public Cut
{
  
  // Gives access to input, etc.
  friend class Pheno ;   

  //--------------------------------------------------------------

  public:

    // Constructor takes a reference to the event
    M4Cut(EV&) ;
   

  //--------------------------------------------------------------

  private:

    // Virtual method from cut class
    void cut_cond(vector<ExParticle>&) ;   
    void input(std::string) ; 

    // The minimum of m4l
    float M4_cut_min = 0 ;   

    // The maximum of m4l
    float M4_cut_max = 10000 ;   

    // Sorts particles based on pT
    void pT_sort(vector<ExParticle>&) ;
};

//==============================================================
#endif /*M4Cut_H*/
