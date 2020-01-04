#ifndef ExParticle_H
#define ExParticle_H

#include <Pythia8/Pythia.h>
#include <fastjet/ClusterSequence.hh>
#include <fastjet/Selector.hh>

//==============================================================
// Forward declaration
class ExEvent ;

class ExParticle : public Pythia8::Particle
{

  friend class ExEvent ;
  // friend class pheno ;  // For debugging, remove it later!
  //--------------------------------------------------------------

  public:
    // Constructor
    ExParticle() ; 
    ExParticle(const Particle&) ;

    // Copy constructor 
    // ExParticle(const ExParticle&) ; 

    ExParticle& operator=(const ExParticle& pt) {
      if (this != &pt) 
      {
        idSave = pt.idSave; statusSave = pt.statusSave;
        mother1Save = pt.mother1Save; mother2Save = pt.mother2Save;
        daughter1Save = pt.daughter1Save; daughter2Save = pt.daughter2Save;
        colSave = pt.colSave; acolSave = pt.acolSave; pSave = pt.pSave;
        mSave = pt.mSave; scaleSave = pt.scaleSave; polSave = pt.polSave;
        hasVertexSave = pt.hasVertexSave; vProdSave = pt.vProdSave;
        tauSave = pt.tauSave; pdePtr = pt.pdePtr; evtPtr = pt.evtPtr; 
        
        decays_to_had = pt.decays_to_had; set_had_dec_flag = pt.set_had_dec_flag;
        visible_mom =pt.visible_mom; set_vis_mom_flag = pt.set_vis_mom_flag;
        detected_mom = pt.detected_mom; event_ptr = pt.event_ptr;
        id_eff_val = pt.id_eff_val; set_id_eff_flag = pt.set_id_eff_flag ;
      }

      return *this; }

    bool operator==(const ExParticle& right) ;

    // Returns the IdEff of particle
    double GetIdEff() ;

    // returns Mom
    Pythia8::Vec4 GetMom() ;                  

    // returns DecaysToHad
    bool IsHadDec() ;

    // returns visible momentum ("visibleMom")
    Pythia8::Vec4 GetVisMom() ;   

    // Pointer to the ExEvent to which the particle belongs 
    ExEvent* event_ptr=NULL ;
  //--------------------------------------------------------------

  private:

    // Setting the IdEff of particle
    void SetIdEff() ;

    // Member function to set the Event pointer.
    void SetEventPtr(ExEvent*) ;

    // Sets DecaysToHad
    void SetHadDec() ;
    
    // Sets the visible momentum for a decaying particle
    void SetVisMom() ;
        
    // Sets ptMom
    void SetMom() ; 

    // Muon mom resolution
    float MuonMomRes() ; 

    // Electron mom resolution
    float ElectronMomRes() ; 
    
    // ....................................
    // Flags
    // If the idEff is set
    bool set_id_eff_flag = false ;

    // it's true if decays hadronically
    bool decays_to_had = false ;

    // Checks if DecaysToHad is set by ExEvent
    bool set_had_dec_flag = false ;

    // Checks if visibleMom is set
    bool set_vis_mom_flag = false ; 
    // ....................................

    // The idEff value
    double id_eff_val = 1 ;

    // Visible momentum
    Pythia8::Vec4 visible_mom ;

    // The measured 4-mom by detectors
    Pythia8::Vec4 detected_mom ;
};

//==============================================================
#endif /*ExParticle_H*/
