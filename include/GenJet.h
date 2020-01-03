#ifndef  GenJet_H
#define  GenJet_H

// #include <vector>
// #include <Pythia8/Pythia.h>
// #include <fastjet/ClusterSequence.hh>
// #include <fastjet/Selector.hh>

#include "EV.h"

// using    std::vector ;

//=============================================================
class STBinner ;
/*
  Generates the jets and makes reports.
*/
class GenJet
{

  // Gives access to input, etc.
  friend class Pheno ;   

  // Gives access to inc_jets_after, etc.
  friend class STBinner ;   

  //--------------------------------------------------------------
  protected:
    Logger logger ;

  //--------------------------------------------------------------
  public:

    // Basic constructor
    GenJet(EV&) ;  

    // Generates jets
    void gen() ;  

    // Saves jet details
    void report() ;  

    // Returns the inclusive jets after cuts
    std::vector<fastjet::PseudoJet> inclusive_after() ;

  //--------------------------------------------------------------

  private:

    // Ref. to the event
    EV& ev_ref ;  

    // // Takes fastjet commands
    // void input(string , bool) ;  

    // Takes fastjet commands
    void input(std::vector<std::string> )  ;


  // ............. Definition options ..............
    // Sets jet definition options
    void setJetDef(std::vector<std::string>) ;

      // Sets jet algorithm
      void setJetAlg(std::string) ;

      // Sets recombination scheme
      void setRecScheme(std::string) ;

      // Sets the Strategy
      void setStrategy(std::string) ;

    /*
      This could be one of
        {kt_algorithm, cambridge_algorithm, antikt_algorithm,
         genkt_algorithm, ee_kt_algorithm, ee_genkt_algorithm}
     */
    fastjet::JetAlgorithm jet_alg ;

    // Default definiton options
    // The radius would not be mandatory for e^+ e^- algorithms
    double jet_radius = 0.0 ;
    fastjet::RecombinationScheme recomb_scheme = fastjet::E_scheme;
    fastjet::Strategy strategy = fastjet::Best;

    std::string my_jet_dscr ;

  // .............................................


  // ............. Selector options ..............
  fastjet::Selector jet_selector = fastjet::SelectorIdentity() ;

    // Sets the Selector options
    void setSelector(std::vector<std::string>) ;
    
      // PtMin cut value
      double SelPtMin  = 30 ;   

      // AbsEtaMAx cut value
      double SelEtaMax = 2.5 ;   

  // .............................................
    
    
    // inclusive jets before cuts
    std::vector<fastjet::PseudoJet> inc_jets_before = {} ;   

    // inclusive jets after cuts
    std::vector<fastjet::PseudoJet> inc_jets_after = {} ;   


    // Finds the distance between the pseudo jet and a particle
    double distance(fastjet::PseudoJet&, ExParticle&) ; 

    // Removes jets close to isoalted leptons
    void  isolate() ;    

    // Isolation condition assuming jets are faking taus
    void fake_isolate();

  // ............. Report options ..............
    // report jets or not
    bool report_jet = false ;    

    char jet_rep_char[100] ;

    // The report lines from isolate method
    std::string isolate_message  = "" ;   

    // prints table of jets
    template <class T> void print_jettable(std::FILE *,
     std::vector<T> list, char* description, const char* title, int iEv) ;
  // .............................................
};


//=============================================================

#endif
