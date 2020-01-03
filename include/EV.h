#ifndef EV_H
#define EV_H

// #include <Pythia8/Pythia.h>
// #include <fastjet/ClusterSequence.hh>
// #include <fastjet/Selector.hh>

#include "Basics.h"
#include "ExParticle.h"

// using std::vector ;
// using namespace Pythia8 ;

//==============================================================
// Forward declaration
class Cut ;
class GenJet ;

class EV
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
    EV(int, Pythia8::Event&) ; 
    // Destructor
    ~EV() ;

    // Sets the EV pointer for Exparticle
    void  setEVPtr(ExParticle &) ;

    // Returns the event weight
    double weight() ;

    /* Whether drop a particle which fails efficiency test,
    or keep it but weigh the event number so 
    that it's counted as less than 1.  */
    bool drop_low_eff = true ;

    // Finds the hadronic taus saves into filename
    void  find_had_tau(std::string filename) ;

    // removing the taus not passing cuts
    void  update_hadtaus(std::vector<ExParticle>) ; 

    // returns a reference to full event
    std::vector<ExParticle>& Full_Ev() ; 

    // returns the size of an event
    size_t  size() const ; 

    // Overloading ()
    std::vector<ExParticle>  operator() (std::vector<int>) ; 

    // returns the hadronic taus
    std::vector<ExParticle>& htau() ; 

    // returns the event_number
    int i() const ; 

    // returns the visible 4-momentum
    Pythia8::Vec4  vis_p(ExParticle& p) ; 

    //------------------FASTJET----------------------
    // returns a reference to the ps_jts
    std::vector<fastjet::PseudoJet>& psjet() ; 

    // returns a reference to the lep_jts
    std::vector<ExParticle>& lep_for_jet() ; 

    // Updates the ps_jet set based on the cuts
    void  update_ps_jets() ; 

    // GenJet pointer for STBinner
    GenJet* GenJetPtr = NULL ;
    //-----------------------------------------------

    //-----------------------CUTS--------------------
    // returns a reference to leptons that passed all the cuts
    std::vector<ExParticle>& pass_lepts() ; 
    void  update_pass_lepts(std::vector<ExParticle> ) ;
    //-----------------------------------------------

    // Prints the full event given the file name
    void  print(std::string) ; 

    template <class T>
    void  print_table(std::FILE *, std::vector<T>) ;

//--------------------------------------------------------------

  private:

    // inputs options
    void  input(std::string, bool) ; 

    // The event number
    int event_number ; 

    // Sets event weight
    void setEVWeight() ;

    // event weight is how much an event should count
    double weight_val = 1 ;

    // If the event weight is set or not
    bool eVWeightisSet  = false ;
    
    // The hadronically decaying taus
    std::vector<ExParticle>  h_taus ; 

    // True if p hadronically decays
    bool  had_dec(ExParticle &) ; 

    // The full event
    std::vector<ExParticle>  Full_Event ; 

    // Saves the Cut pointer
    void addCutPtr(std::shared_ptr<Cut>) ;

    // A list of Cut pointers that have been applied to the event
    std::vector<std::shared_ptr<Cut> > CutPtr ;

    // Adds GenJet pointer for STBinner
    void addGenJetPtr(GenJet*) ;

    // Reports the taus
    bool  tau_report = false ; 

    // Saves psjets for fastjet
    void  gen_psjts(ExParticle ) ; 



    //------------------FASTJET----------------------
    // Pseudojets as an input to fastjet
    std::vector<fastjet::PseudoJet>  ps_jts ; 

    // Leptons as an input to fastjet
    std::vector<ExParticle>  lep_jts ; 
    //-----------------------------------------------
    
    // Leptons that passed all the cuts 
    std::vector<ExParticle>  iso_leptons ;   
  
};

//==============================================================
#endif /*EV_H*/
