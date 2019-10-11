/*

  This file is for analysing the jets faking taus.

  - Last Updated by Zaki on August 29, 2019
*/

#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>
#include <vector>
#include <chrono>
#include <ctime>
#include <omp.h>
#include "Pythia8/Pythia.h"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/Selector.hh"
#include "../inc/EV.h"
#include "../inc/Basics.h"
#include "../inc/Pheno.h"

using namespace Pythia8 ;


vector<double> InvMeMu(vector<ExParticle>& parts);
vector<double> scaleFTauE(vector<ExParticle>&);
double jettest(vector<ExParticle>&) ; 

///////////////////////////////////////////////////
///////////////      main()     ///////////////////
///////////////////////////////////////////////////
int main(int argc,char *argv[])
{
  string filename           = "" ;
  string Tot_Num_Events_str = "" ;
  string NUM_THREADS_str    = "" ;
  string report_input_str   = "" ;

  filename.assign(argv[1]) ;
  Tot_Num_Events_str.assign(argv[2]) ;
  NUM_THREADS_str.assign(argv[3]) ;
  report_input_str.assign(argv[4]) ;

  if( report_input_str == "1")
    report_input_str = "true" ;
  else
    report_input_str = "false" ;

  // Seeding the random function for ID_eff "rand01()"
  srand(time(NULL)) ;

  Pheno phen;

  //..................INPPUT.....................
  // 'input' method strips inputs from any spaces,
  // unless a "false" option is passed as a second argument.

  // Number of requested threads
  phen.input("Threads= " + NUM_THREADS_str) ;

  // Total number of events
  phen.input("Events= " + Tot_Num_Events_str) ;
  phen.input("file: lhe=" + filename) ;

  // reporting options
  phen.input("report:cuts="+report_input_str) ;

  // This is an exception list that will always
  //  print reports for the range of events listed.
  // Use ',' for separatng event ranges, or single events.
  // Use '-' for ranges.
  // phen.input("report:cuts=1-500") ;

  phen.input("report:taus="+report_input_str) ;
  // phen.input("report:taus=5-17,155,122") ;
  phen.input("report:jets="+report_input_str) ;
  // phen.input("report:jets=1-1000") ;

  // printing event records
  // phen.input("print_events=1-10,50,300") ;


  // ----------------------pythia options-------------------------------------

  phen.input( "pythia=Beams:frameType = 4" ) ;

  // "8" for CTEQ6L1 PDF set.
  // "False" option is for not stripping the space 
  // between 'pSet' & '8'.
  phen.input( "pythia=PDF:pSet 8", false ) ;

  phen.input( "pythia=Random:setSeed = on" ) ;

  // Use -1 for a time dep. random #
  phen.input( "pythia=Random:seed = -1" ) ;

  // Show pythia banner or not?
  phen.input( "show_pythia_banner= false" ) ;

  // Init settings:
  phen.input( "pythia= Init:showProcesses  = off" ) ;
  phen.input( "pythia= Init:showChangedParticleData  = off" ) ;
  phen.input( "pythia= Init:showMultipartonInteractions  = off" ) ;
  phen.input( "pythia= Init:showChangedSettings   = off" ) ;

  // Next settings:
  phen.input( "pythia= Next:numberShowProcess  = 0" ) ;
  phen.input( "pythia= Next:numberShowInfo  = 0" ) ;
  phen.input( "pythia= Next:numberShowLHA = 0" ) ;
  phen.input( "pythia= Next:numberShowEvent = 0" ) ;


  // Stat settings:
  phen.input( "pythia= Stat:showProcessLevel = off" ) ;

  // ----------------------Cut options----------------------

  // Required final states ( use e for both e+ and e-)
  phen.input( "required_set= ta_h=0, e>=1, mu+>=1, mu->=1" ) ;

  // ---------------------- Cuts ----------------------
  // You can move the recording function(s) between the cuts
  // it will record the values for particles passing
  // the cut prior to it. The functions defined here or
  // in included header files can be used.

  // ID_Eff cut
  phen.input( "cuts= ID_Eff: drop_low_eff=true" ) ;

  // Cut on M_l+l-
  phen.input( "cuts= M2: M2_Cut_Value=12" ) ;

  /*  p_T Cut Conditions:
        e & mu: pt>= 7 GeV  (at least 1 > 20 GeV)
        t_h: pt>= 20 GeV
  */
  phen.input( "cuts= PT: lead=20, sub_lead=7, extra=7, had_tau=20" ) ;

  /*  prap Cut Conditions:
        e & mu: |eta| < 2.4
        t_h: |eta| < 2.3
  */
  phen.input( "cuts= PRap: e=2.4, mu=2.4, had_tau=2.3" ) ;

  // Isolation cut
  phen.input( "cuts=ISO" ) ;

  //-------------------------
  // fastjet options
  phen.input("fastjet= Def: Algorithm=antikt_algorithm, R=0.4") ;
  phen.input("fastjet= Selector: AbsEtaMax=2.3") ;
  phen.input("fastjet= Selector: PtMin=20") ;
  
  // running fastjet
  phen.input("fastjet=run") ;
  //-------------------------

  // Demanding  m(l+l-) <= 75 GeV or 105 GeV <= m(l+l-)
  phen.input( "cuts=OffZ: OffZ_Cut_Min=75, OffZ_Cut_Max=105" ) ;

  // -------------------------------------------------------------------------

  // -----------------------------------------------------
  // Recording invariant masses
  phen.input("record=Tau_E_Scaled_5", scaleFTauE) ;
  // phen.input("record=Testing_Jets", jettest) ;

  // -----------------------------------------------------


  phen.run() ;

  return 0 ;
}


//==============================================================
// Accessing the jets from particles
double jettest(vector<ExParticle>& parts) 
{
  size_t jet_set_size = parts[0].EVPtr->GenJetPtr->inclusive_after().size() ;
  double output = jet_set_size*1.0 ;
  
  return output ;
}

//==============================================================
// Invariant mass for (mu, e) pair (for LFV_1 )
vector<double> InvMeMu(vector<ExParticle>& parts)
{
  if (parts.size() != 4 )  return {-100, -100, -100};

  vector<ExParticle> sel_EpMum ;
  vector<ExParticle> sel_EmMup ;

  for (size_t i=0 ; i<parts.size() ; ++i)
  {
    if ( parts[i].id() == ID_ELECTRON || parts[i].id() == -ID_MUON)
      sel_EmMup.push_back(parts[i]);

    if ( parts[i].id() == -ID_ELECTRON || parts[i].id() == ID_MUON)
      sel_EpMum.push_back(parts[i]);
  }

  if (sel_EpMum.size() != 2 || sel_EmMup.size() != 2 )  return {-200, -200, -200};


  return {invM(sel_EmMup), invM(sel_EpMum), invM(parts)};
}

//==============================================================
// Electron & tau momentum scaling ( for LFV_3_1 )
vector<double> scaleFTauE(vector<ExParticle>& parts)
{
  if ( parts.size() != 3 )  return { -1 } ;

  std::vector<fastjet::PseudoJet> jet_set = 
   parts[0].EVPtr->GenJetPtr->inclusive_after() ;

  if ( jet_set.size() ==0 )  return { -2 } ;

  vector<double> out ;

  // out.push_back(parts[0].EVPtr->i()) ;

  for (size_t i=0 ; i<jet_set.size() ; ++i)
  {

    vector<ExParticle> sel_emu ;
    vector<ExParticle> Ftau_mu ;

    // Finding the total charge of each jet
    int j_charge = jet_set[i].user_index() ;

    // dicard if the charge is not +/- 1
    if ( abs(j_charge) != 3) continue ;

    int ftau_sign = ( j_charge / abs(j_charge) );

    // Adding electron
    for (size_t i=0 ; i<parts.size() ; ++i)
    {
      if ( parts[i].idAbs() == ID_ELECTRON)
        sel_emu.push_back(parts[i]) ;
    }

    // Finding muon
    for (size_t i=0 ; i<parts.size() ; ++i)
    {
      if ( parts[i].idAbs() == ID_MUON )
      {
        if( (parts[i].id() / parts[i].idAbs()) == ftau_sign )
          sel_emu.push_back(parts[i]) ;
        else
          Ftau_mu.push_back(parts[i]) ;
      }
    }

    if (sel_emu.size() != 2 || Ftau_mu.size() != 1)  return { -3 } ;

    Vec4 p_Fta( jet_set[i].px(), jet_set[i].py(), jet_set[i].pz(), jet_set[i].e() ) ;


  // ..........................................................
  // The weight is taken from jet -> tau_h misidentification rate
  //  in Fig.~4 of Ref.~https://arxiv.org/abs/1809.02816
  double EV_weight = 0 ;
  double f_ta_pT = p_Fta.pT() ;

  if ( 19.99 < f_ta_pT && f_ta_pT <= 30 )
    EV_weight = 0.0106;
  else if ( 30 < f_ta_pT && f_ta_pT <= 40 )
    EV_weight = 0.00771084;
  else if ( 40 < f_ta_pT && f_ta_pT <= 50 )
    EV_weight = 0.0038;
  else if ( 50 < f_ta_pT && f_ta_pT <= 70 )
    EV_weight = 0.0017874;
  else if ( 70 < f_ta_pT && f_ta_pT <= 90 )
    EV_weight = 0.000955;
  else if ( 90 < f_ta_pT && f_ta_pT <= 120 )
    EV_weight = 0.000578149;
  else if ( 120 < f_ta_pT && f_ta_pT <= 150 )
    EV_weight = 0.000350;
  else if ( 150 < f_ta_pT && f_ta_pT <= 200 )
    EV_weight = 0.000251;
  else if ( 200 < f_ta_pT && f_ta_pT <= 250 )
    EV_weight = 0.0001753;
  else if ( 250 < f_ta_pT && f_ta_pT <= 300 )
    EV_weight = 0.0001453;
  // ..........................................................

    Vec4 p_e = sel_emu[0].visMom();
    Vec4 p_mu_sum = Ftau_mu[0].visMom() + sel_emu[1].visMom();

    double m_ta = 1.777 ; // GeV
    double m_H  = 125   ; // GeV

    double M4 = (p_Fta + Ftau_mu[0].visMom() + p_e + sel_emu[1].visMom() ).mCalc() ;
    double tauMuInv  = (p_Fta + Ftau_mu[0].visMom()).mCalc() ;
    double eMuInv    = (p_e + sel_emu[1].visMom()).mCalc() ;


    // ..........................................
    // Fifth Scaling
    double a = ( 2*p_Fta*p_e / (pow(eMuInv, 2)*pow(tauMuInv, 2)) + pow(m_ta,2) / pow(tauMuInv, 4) ); 
    double b = 2*(p_mu_sum) * ( p_Fta/pow(tauMuInv, 2) + p_e/pow(eMuInv, 2) );  
    double c = p_mu_sum.m2Calc() - pow(m_H,2);  
    // ..........................................

    double mfi_sq_p = ( -b + sqrt( pow(b,2) - 4*a*c ) ) / ( 2*a ) ; 
    double mfi_sq_m = ( -b - sqrt( pow(b,2) - 4*a*c ) ) / ( 2*a ) ; 

    // ..........................................
    //........Choosing the poitive answer........
    double positive_mfi_sq = 0 ; 
    if ( mfi_sq_p > 0 )
      positive_mfi_sq = mfi_sq_p ;
    else if ( mfi_sq_m > 0 )
      positive_mfi_sq = mfi_sq_m ;
    //.....................  
    double alpha_ta = positive_mfi_sq / pow(tauMuInv, 2);
    double alpha_e = positive_mfi_sq / pow(eMuInv, 2);

    if(alpha_ta < 1 || alpha_e < 1) { alpha_e = 1; alpha_ta = 1; }

    Vec4 scaled_p_ta = alpha_ta*p_Fta ;
    Vec4 scaled_p_e = alpha_e*p_e ;
    // ..........................................


    double tauMuInv_scaled  = (scaled_p_ta + Ftau_mu[0].visMom()).mCalc() ;
    double eMuInv_scaled    = (scaled_p_e + sel_emu[1].visMom()).mCalc() ;

    double M4_scaled = (scaled_p_ta + Ftau_mu[0].visMom() + scaled_p_e + sel_emu[1].visMom() ).mCalc() ;

    out.push_back(EV_weight) ;
    out.push_back(tauMuInv)  ; out.push_back(eMuInv) ; out.push_back(M4) ;
    out.push_back(mfi_sq_p)  ; out.push_back(mfi_sq_m)  ;
    out.push_back(tauMuInv_scaled) ; out.push_back(eMuInv_scaled) ; out.push_back(M4_scaled) ;

  }

  return out;
}

//==============================================================