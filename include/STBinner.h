#ifndef STBinner_H
#define STBinner_H

#include "ExEvent.h"
#include "Binner.h"

//==============================================================
class STBinner : public Binner
{
  // Gives access to input, etc.
  friend class Pheno ;   

  //--------------------------------------------------------------
  public:
    typedef std::vector<ExParticle> ParticleLST;

    // Constructor takes a reference to the event
    STBinner() ;

    // Copy Constructor
    STBinner(const STBinner&) ;
  //--------------------------------------------------------------
  private:

    std::shared_ptr<Binner> Clone() override; 

    // Reporting the bin information (virtual method from Binner)
    void Report(std::string) const override ;
    
    // Input for string commands
    void Input(std::string) override ;

    // Adding the event
    void Input(ExEvent&) override ;

    // bins the event (virtual method from base class "binner")
    void BinIt(bool cut_report, std::string cut_file) override ; 

    // The bin set
    std::vector<std::vector<std::vector<double> > > bin_set = 
    std::vector<std::vector<std::vector<double> > >(10, std::vector<std::vector<double> >(3, std::vector<double>(5)) ) ;

    // The set of charged leptons that passed the cuts
    ParticleLST charged_lept ;

    // The set of jets
    std::vector<fastjet::PseudoJet> jet_set ;

    // Returns the value of S_T
    double S_T() ;

    // S_T limits for the bins:
    std::vector<float> st_limit ;
    
    // Sets the on-off-Z status of the event
    void OnOffZ() ;
    
    /* on-off-Z status of the event
        - 0 :            m_{l+l-} < 75 GeV
        - 1 :  75 GeV  < m_{l+l-} < 105 GeV
        - 2 :  105 GeV < m_{l+l-}
    */
    int on_z = -1 ;

    // checks the lepton number conditions
    std::vector<int> LepNumCheck() ;

    float one_event = 1.0 ;
};
//==============================================================

#endif /*STBinner_H*/
