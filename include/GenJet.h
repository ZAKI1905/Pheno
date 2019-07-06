#ifndef  GenJet_H
#define  GenJet_H

#include <vector>
#include "Pythia8/Pythia.h"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/Selector.hh"
#include "EV.h"

using    std::vector ;

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

  public:

    // Basic constructor
    GenJet(EV&) ;  

    // Generates jets
    void gen() ;  

    // Saves jet details
    void report() ;  


  //--------------------------------------------------------------

  private:

    // Ref. to the event
    EV&               ev_ref ;  

    // Takes fastjet commands
    void input(string , bool) ;  

    // Takes fastjet commands
    void input(string , string, double) ;  

    /*
      This could be one of
        {kt_algorithm, cambridge_algorithm, antikt_algorithm,
         genkt_algorithm, ee_kt_algorithm, ee_genkt_algorithm}
     */
    fastjet::JetAlgorithm jet_alg ;

    //The radius would not be mandatory for e^+ e^- algorithms
    double jet_radius ;

    string my_jet_dscr ;

    // report jets or not
    bool report_jet = false ;   

    // process id
    int   pr_id           = 0 ;   

    // LHE File name
    std::string File_LHE  = "" ;   

    // The report lines from isolate method
    std::string isolate_message  = "" ;   
    
    // PtMin cut value
    double SelPtMin  = 30 ;   

    // AbsEtaMAx cut value
    double SelEtaMax = 2.5 ;   

    // inclusive jets before cuts
    vector<fastjet::PseudoJet> inc_jets_before ;   

    // inclusive jets after cuts
    vector<fastjet::PseudoJet> inc_jets_after ;   


    // Finds the distance between the pseudo jet and a particle
    double distance(fastjet::PseudoJet&, ExParticle&) ; 

    // Removes jets close to isoalted leptons
    void  isolate() ;    

    // prints table of jets
    template <class T> void print_jettable(std::FILE *,
     vector<T> list, char* description, const char* title, int iEv) ;

};


//=============================================================

#endif
