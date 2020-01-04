#ifndef ExEvent_H
#define ExEvent_H

#include "Basics.h"
#include "ExParticle.h"

//==============================================================
// Forward declaration
class Cut ;
class GenJet ;

class ExEvent
{
  // Grants access to input, etc.
  friend class Pheno ;

  // Grants access to had_dec   
  friend class ExParticle ;

  // Grants access to setCutPtr:
  friend class Cut ;

  // Grants access to setGenJetPtr:
  friend class GenJet ;

  // Grants access to GenJetPtr:
  friend class STBinner ;

  //--------------------------------------------------------------
  public:

    // Constructor
    ExEvent(int, Pythia8::Event&) ; 
    // Destructor
    ~ExEvent() ;

    // Copy constructor
    ExEvent(const ExEvent& ) ;

    // Sets the ExEvent pointer for Exparticle
    void  SetEventPtr(ExParticle &) ;

    // Returns the event weight
    double Weight() ;

    // Finds the hadronic taus saves into filename
    void  FindHadTaus(std::string filename) ;

    // removing the taus not passing cuts
    void  UpdateHadTaus(std::vector<ExParticle>) ; 

    // returns a reference to full event
    std::vector<ExParticle>& FullEvent() ; 

    // returns the size of an event
    size_t  size() const ; 

    // Overloading ()
    std::vector<ExParticle>  operator() (std::vector<int>) ; 

    // returns the hadronic taus
    std::vector<ExParticle>& HadronicTaus() ; 

    // returns the event_number
    int i() const ; 

    // returns the visible 4-momentum
    Pythia8::Vec4  VisMom(ExParticle& p) ; 

    //------------------FASTJET----------------------
    // returns a reference to the ps_jts
    std::vector<fastjet::PseudoJet>& GetPseudoJets() ; 

    // returns a reference to the lep_jts
    std::vector<ExParticle>& GetLeptonJets() ; 

    // Updates the ps_jet set based on the cuts
    void  UpdatePseudoJet() ; 
    //-----------------------------------------------

    //-----------------------CUTS--------------------
    // returns a reference to leptons that passed all the cuts
    std::vector<ExParticle>& PassedLeptons() ; 
    void  UpdatePassedLepts(std::vector<ExParticle> ) ;
    //-----------------------------------------------

    // Prints the full event given the file name
    void  Print(std::string) ; 

    template <class T>
    void  PrintTable(std::FILE *, std::vector<T>) ;


    /* Whether drop a particle which fails efficiency test,
    or keep it but weigh the event number so 
    that it's counted as less than 1.  */
    bool drop_low_eff = true ;

    // GenJet pointer for STBinner
    GenJet* GenJetPtr = NULL ;

//--------------------------------------------------------------
  private:

    // inputs options
    void  Input(std::string, bool) ; 

    // Sets event weight
    void SetEventWeight() ;

    // True if p hadronically decays
    bool  HadronicDecay(ExParticle &) ; 

    // Saves the Cut pointer
    void AddCutPtr(std::shared_ptr<Cut>) ;

    // Adds GenJet pointer for STBinner
    void AddGenJetPtr(GenJet*) ;

    // Saves psjets for fastjet
    void  GenPseudoJets(ExParticle ) ; 

    // .....................................................
    // Member Variables
    // .....................................................
    // The event number
    int event_number ; 

    // event weight is how much an event should count
    double weight_val = 1 ;

    // If the event weight is set or not
    bool set_event_weight_flag  = false ;
    
    // Reports the taus
    bool  tau_report_flag = false ; 

    // The hadronically decaying taus
    std::vector<ExParticle>  had_taus ; 

    // The full event
    std::vector<ExParticle>  full_event ; 

    // A list of Cut pointers that have been applied to the event
    std::vector<std::shared_ptr<Cut> > cut_ptr ;
    
    // Leptons that passed all the cuts 
    std::vector<ExParticle>  isolated_leptons ;   

    //------------------FASTJET----------------------
    // Pseudojets as an input to fastjet
    std::vector<fastjet::PseudoJet>  pseudo_jet_set ; 

    // Leptons as an input to fastjet
    std::vector<ExParticle>  lepton_jets ; 
    //-----------------------------------------------

    // .....................................................

  
};

//==============================================================
#endif /*ExEvent_H*/
