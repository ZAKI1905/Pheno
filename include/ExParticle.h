#ifndef ExParticle_H
#define ExParticle_H

#include <vector>
#include "Pythia8/Pythia.h"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/Selector.hh"

using std::vector ;
using namespace Pythia8 ;

//==============================================================
// Forward declaration
class EV ;

class ExParticle : public Particle
{

  friend class EV ;
  //--------------------------------------------------------------

  public:
    // Constructor
    ExParticle() ; 
    ExParticle(const Particle&) ; 

    ExParticle& operator=(const ExParticle& pt) {if (this != &pt) {
    idSave = pt.idSave; statusSave = pt.statusSave;
    mother1Save = pt.mother1Save; mother2Save = pt.mother2Save;
    daughter1Save = pt.daughter1Save; daughter2Save = pt.daughter2Save;
    colSave = pt.colSave; acolSave = pt.acolSave; pSave = pt.pSave;
    mSave = pt.mSave; scaleSave = pt.scaleSave; polSave = pt.polSave;
    hasVertexSave = pt.hasVertexSave; vProdSave = pt.vProdSave;
    tauSave = pt.tauSave; pdePtr = pt.pdePtr; evtPtr = pt.evtPtr; 
    
    DecaysToHad = pt.DecaysToHad; HadDecisSet = pt.HadDecisSet;
    visibleMom =pt.visibleMom; visMomisSet = pt.visMomisSet;
    ptMom = pt.ptMom; EVPtr = pt.EVPtr;}
    return *this; }

    bool operator==(const ExParticle& right) ;

    // Returns the IdEff of particle
    double idEff() ;

    // returns Mom
    Vec4 mom() ;                  

    // returns DecaysToHad
    bool isHadDec() ;

    // returns visible momentum ("visibleMom")
    Vec4 visMom() ;   

  //--------------------------------------------------------------

  private:

    // Setting the IdEff of particle
    void setIdEff() ;

    // If the idEff is set
    bool idEffisSet = false ;

    // The idEff value
    double id_eff_val = 1 ;

    // Pointer to the EV to which the particle belongs 
    EV* EVPtr=NULL ;

    // Member function to set the Event pointer.
    void setEVPtr(EV*) ;

    // Sets DecaysToHad
    void setHadDec() ;

    // it's true if decays hadronically
    bool DecaysToHad = false ;

    // Checks if DecaysToHad is set by EV
    bool HadDecisSet = false ;


    // Sets the visible momentum for a decaying particle
    void setVisMom() ;

    // Visible momentum
    Vec4 visibleMom ;

    // Checks if visibleMom is set
    bool visMomisSet = false ; 

    // The measured 4-mom by detectors
    Vec4 ptMom ; 

    // Muon mom resolution
    float muonMomRes() ; 

    // Electron mom resolution
    float elecMomRes() ; 
    
    // Sets ptMom
    void setMom() ; 
  
};

//==============================================================
#endif /*ExParticle_H*/
