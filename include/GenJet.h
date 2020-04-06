#ifndef  GenJet_H
#define  GenJet_H

#include "ExEvent.h"
#include "Prog.h"

//--------------------------------------------------------------
namespace PHENO
{
//=============================================================
class STBinner ;
/*
  Generates the jets and makes reports.
*/
class GenJet : public Prog
{

  // Gives access to input, etc.
  friend class Pheno ;   

  // Gives access to inc_jets_after, etc.
  friend class STBinner ;   

  //--------------------------------------------------------------
  protected:

  //--------------------------------------------------------------
  public:

    // Basic constructor
    GenJet(ExEvent&) ;  

    // Assignment operator
    GenJet& operator=(const GenJet&) ;

    // Set Event reference
    void SetEventRef(ExEvent&) ; 

    // Generates jets
    void GenerateJets() ;  

    // Saves jet details
    void Report() ;  

    // Returns the inclusive jets after cuts
    std::vector<fastjet::PseudoJet> InclusiveJetsAfter() ;

  //--------------------------------------------------------------

  private:

    // Ref. to the event
    ExEvent& ev_ref ;  

    // // Takes fastjet commands
    // void input(string , bool) ;  

    // Takes fastjet commands
    void Input(std::vector<std::string> )  ;


  // ............. Definition options ..............
    // Sets jet definition options
    void SetJetDef(std::vector<std::string>) ;

      // Sets jet algorithm
      void SetJetAlg(std::string) ;

      // Sets recombination scheme
      void SetRecScheme(std::string) ;

      // Sets the Strategy
      void SetStrategy(std::string) ;

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
    void SetSelector(std::vector<std::string>) ;
    
      // PtMin cut value
      double sel_pT_min  = 30 ;   

      // AbsEtaMAx cut value
      double sel_eta_max = 2.5 ;   

  // .............................................
    
    
    // inclusive jets before cuts
    std::vector<fastjet::PseudoJet> inc_jets_before = {} ;   

    // inclusive jets after cuts
    std::vector<fastjet::PseudoJet> inc_jets_after = {} ;   


    // Finds the distance between the pseudo jet and a particle
    double Distance(fastjet::PseudoJet&, ExParticle&) ; 

    // Removes jets close to isolated leptons
    void  Isolate() ;    

    // Isolation condition assuming jets are faking taus
    void FakeTauIsolate();

  // ............. Report options ..............
    // report jets or not
    bool report_jet_flag = false ;    

    char jet_rep_char[100] ;

    // The report lines from isolate method
    std::string isolate_message  = "" ;   

    // prints table of jets
    template <class T> void PrintJetTable(std::FILE *,
     std::vector<T> list, char* description, const char* title, int iEv) ;
  // .............................................
};

//=============================================================
} // PHENO namespace
//=============================================================
#endif
