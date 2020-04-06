#ifndef M4Cut_H
#define M4Cut_H

#include "Cut.h"

//--------------------------------------------------------------
namespace PHENO
{
  class Pheno;

//--------------------------------------------------------------
namespace CUTS
{
//==============================================================
class M4Cut : public Cut
{
  
  // Gives access to input, etc.
  friend class PHENO::Pheno ;   

  //--------------------------------------------------------------
  public:
    // Default constructor
    M4Cut() ;

    // Constructor takes a pointer to the event
    M4Cut(ExEvent*) ;
    
    // Copy Constructor
    M4Cut(const M4Cut&) ;
    
  //--------------------------------------------------------------
  private:

    // Virtual method from cut class
    void CutCond(ParticleLST&) override;   
    void Input(std::string) override; 
    std::shared_ptr<Cut> Clone() override; 

    // The minimum of m4l
    float M4_cut_min = 0 ;   

    // The maximum of m4l
    float M4_cut_max = 10000 ;   

    // Sorts particles based on pT
    void pT_Sort(ParticleLST&) ;
};

//=============================================================
} // CUTS namespace
//==============================================================
} // PHENO namespace
//=============================================================
#endif /*M4Cut_H*/
