#ifndef STBinner_H
#define STBinner_H

#include <vector>
#include "Pythia8/Pythia.h"
#include "EV.h"
#include "Binner.h"

using namespace Pythia8 ;

//==============================================================
class STBinner : public Binner
{

  // Gives access to input, etc.
  friend class Pheno ;   

  //--------------------------------------------------------------

  public:

    // Constructor takes a reference to the event
    STBinner() ;
    

  //--------------------------------------------------------------

  private:

    // Reporting the bin information (virtual method from Binner)
    void report(std::string) ;
    
    // Input for string commands
    void input(std::string) ;

    // Adding the event
    void input(EV&) ;

    // bins the event (virtual method from base class "binner")
    void bin_it(bool cut_report, std::string cut_file) ; 

    // The bin set
    vector<vector<vector<double> > > bin_set = 
    vector<vector<vector<double> > >(10, vector<vector<double> >(3, vector<double>(5)) ) ;

    // The set of charged leptons that passed the cuts
    vector<ExParticle> charged_lept ;

    // The set of jets
    vector<fastjet::PseudoJet> jet_set ;

    // Returns the value of S_T
    double S_T() ;

    // S_T limits for the bins:
    vector<float> st_limit ;
    
    // Sets the on-off-Z status of the event
    void on_off_Z() ;
    
    /* on-off-Z status of the event
        - 0 :            m_{l+l-} < 75 GeV
        - 1 :  75 GeV  < m_{l+l-} < 105 GeV
        - 2 :  105 GeV < m_{l+l-}
    */
    int on_z = -1 ;

    // checks the lepton number conditions
    vector<int> lep_num_check() ;

    float one_event = 1.0 ;

};


//==============================================================

#endif /*STBinner_H*/
