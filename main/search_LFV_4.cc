/*

  This file is for proposing new searches for
  the LFV_4_1 model.

  - Last Updated by Zaki on September 5, 2019
*/

#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>
#include <chrono>
#include <ctime>
#include <omp.h>

#include "../include/Pheno.h"
#include "../include/IdEff.h"
#include "../include/Isolation.h"
#include "../include/M2Cut.h"
#include "../include/M4Cut.h"
#include "../include/PtCut.h"
#include "../include/PrapCut.h"
#include "../include/OffZCut.h"

// LFV_4_1  (mu+ mu+ ta- ta-)
std::vector<double> scaleTauE_LFV4(std::vector<ExParticle>&);
std::vector<double> alpha(std::vector<ExParticle>&, std::vector<ExParticle>&) ;
bool check(std::vector<double>) ;

///////////////////////////////////////////////////
///////////////      main()     ///////////////////
///////////////////////////////////////////////////
int main(int argc,char *argv[])
{
  Instrumentor::Get().BeginSession("search_LFV_4", "Profile_search_LFV_4.json") ;        // Begin session  

  std::string filename           = "" ;
  std::string Tot_Num_Events_str = "" ;
  std::string NUM_THREADS_str    = "" ;
  std::string report_input_str   = "" ;

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
  phen.Input("Threads=" + NUM_THREADS_str) ;

  // Total number of events
  phen.Input("Events=" + Tot_Num_Events_str) ;
  phen.Input("file:lhe=" + filename) ;

  // reporting options
  phen.Input("report:cuts=" + report_input_str) ;

  // This is an exception list that will always
  //  print reports for the range of events listed.
  // Use ',' for separatng event ranges, or single events.
  // Use '-' for ranges.
  // phen.Input("report:cuts=1-500") ;

  phen.Input("report:taus=" + report_input_str) ;
  // phen.Input("report:taus=5-17,155,122") ;
  // phen.Input("report:jets="+report_input_str) ;
  // phen.Input("report:jets=1-1000") ;

  // printing event records
  // phen.Input("print_events=1-10,50,300") ;


  // ----------------------pythia options-------------------------------------

  phen.Input( "pythia=Beams:frameType = 4" ) ;

  // "8" for CTEQ6L1 PDF set.
  // "False" option is for not stripping the space 
  // between 'pSet' & '8'.
  phen.Input( "pythia=PDF:pSet 8", false ) ;

  phen.Input( "pythia=Random:setSeed = on" ) ;

  // Use -1 for a time dep. random #
  phen.Input( "pythia=Random:seed = -1" ) ;

  // Show pythia banner or not?
  phen.Input( "show_pythia_banner=false" ) ;

  // Init settings:
  phen.Input( "pythia=Init:showProcesses  = off" ) ;
  phen.Input( "pythia=Init:showChangedParticleData  = off" ) ;
  phen.Input( "pythia=Init:showMultipartonInteractions  = off" ) ;
  phen.Input( "pythia=Init:showChangedSettings   = off" ) ;

  // Next settings:
  phen.Input( "pythia=Next:numberShowProcess  = 0" ) ;
  phen.Input( "pythia=Next:numberShowInfo  = 0" ) ;
  phen.Input( "pythia=Next:numberShowLHA = 0" ) ;
  phen.Input( "pythia=Next:numberShowEvent = 0" ) ;


  // Stat settings:
  phen.Input( "pythia=Stat:showProcessLevel = off" ) ;

  // ----------------------Cut options----------------------------------------

  // Required final states ( use e for both e+ and e-)
  phen.Input( "required_set=ta-_h>=1, e->=1, mu+>=2" ) ; // LFV_4

  // ---------------------- Cuts ----------------------
  // You can move the recording function(s) between the cuts
  // it will record the values for particles passing
  // the cut prior to it. The functions defined here or
  // in included header files can be used.

  // ID_Eff cut
  IdEff id_eff;
  phen.Input({&id_eff, "drop_low_eff=true"}) ;

  // Cut on M_l+l-
  M2Cut m2_cut;
  phen.Input({&m2_cut, "M2_Cut_Value=12"}) ;

  /*  p_T Cut Conditions:
        e & mu: pt>= 7 GeV  (at least 1 > 20 GeV)
        t_h: pt>= 20 GeV
  */
  PtCut pt_cut;
  phen.Input({&pt_cut, "lead=20,sub_lead=7,extra=7,had_tau=20"}) ;

  /*  prap Cut Conditions:
        e & mu: |eta| < 2.4
        t_h: |eta| < 2.3
  */
  PrapCut prap_cut;
  phen.Input({&prap_cut, "e=2.4,mu=2.4,had_tau=2.3"}) ;

  // Isolation cut
  Isolation iso;
  phen.Input(&iso) ;

  //-------------------------
  // // Don't have to run fastjet in this case
  // // fastjet options
  // phen.Input("fastjet=Def:Algorithm=antikt_algorithm,R=0.5") ;
  // phen.Input("fastjet=Selector:EtaMax=2.5") ;
  // phen.Input("fastjet=Selector:PtMin=30") ;
  
  // // running fastjet
  // phen.Input("fastjet=run") ;
  //-------------------------

  // Demanding  m(l+l-) <= 75 GeV or 105 GeV <= m(l+l-)
  OffZCut offz_cut;
  phen.Input({&offz_cut, "OffZ_Cut_Min=75,OffZ_Cut_Max=105"}) ;

  // Demanding 80 GeV <= m(ta-, e-, mu+, mu+) <= 120 GeV
  M4Cut m4_cut;
  phen.Input({&m4_cut, "M4_Cut_Min=80,M4_Cut_Max=120"}) ;  // LFV_4
  // -------------------------------------------------------------------------

  //------------------------------------------------------
  // Recording invariant masses
  
  // phen.Input("record=Tau_E_Scaled_LFV4", scaleTauE_LFV4) ;  // LFV_4_1
  //------------------------------------------------------
  

  phen.Run() ;

  Instrumentor::Get().EndSession();  // End Session
  return 0 ;
}

//==============================================================
// Electron & tau momentum scaling ( for LFV_4_1 )
std::vector<double> scaleTauE_LFV4(std::vector<ExParticle>& parts)
{
  if (parts.size() != 4)  return {-1} ;

  std::vector<ExParticle> comb_a_emu ;
  std::vector<ExParticle> comb_b_emu ;
  std::vector<ExParticle> comb_a_taumu ;
  std::vector<ExParticle> comb_b_taumu ;
  std::vector<ExParticle> mu_set ;

  // Adding tau- and electron
  for (size_t i=0 ; i<parts.size() ; ++i)
  {
    if ( parts[i].id() == ID_TAU)
    {
      comb_a_taumu.push_back(parts[i]) ;
      comb_b_taumu.push_back(parts[i]) ;
    }
    if ( parts[i].id() == ID_ELECTRON)
      comb_a_emu.push_back(parts[i]) ;
      comb_b_emu.push_back(parts[i]) ;
  }

  // Adding the muons
 for (size_t i=0 ; i<parts.size() ; ++i)
  {
    if ( parts[i].id() == -ID_MUON )
      mu_set.push_back(parts[i]);
  }

  if (comb_a_taumu.size() != 1 || comb_a_emu.size() != 1 || mu_set.size() != 2 )
    return {-2} ;

  comb_a_emu.push_back(mu_set[0]) ;  comb_a_taumu.push_back(mu_set[1]) ;
  comb_b_emu.push_back(mu_set[1]) ;  comb_b_taumu.push_back(mu_set[0]) ;

  std::vector<double> alpha_a = alpha(comb_a_emu, comb_a_taumu) ;
  std::vector<double> alpha_b = alpha(comb_b_emu, comb_b_taumu) ;

  // for debugging alpha's
  // std::vector<double> out =  { alpha_a[0], alpha_a[1], alpha_a[2], alpha_b[0], alpha_b[1], alpha_b[2] } ;
  
  bool alpha_a_status = check(alpha_a) ;
  bool alpha_b_status = check(alpha_b) ;

  // if both scaling seem okay, return error code -3
  if ( alpha_a_status && alpha_b_status ) 
    {std::cout<<"(-3) a: ["<<alpha_a[0]<<", "<<alpha_a[1]<<"] - b: ["<<alpha_b[0]<<", "<<alpha_b[1]<<"].\n";
    return {-3} ;}

  // if both scaling aren't okay, return error code -4
  if ( !alpha_a_status && !alpha_b_status ) 
    {std::cout<<"(-4) a: ["<<alpha_a[0]<<", "<<alpha_a[1]<<"] - b: ["<<alpha_b[0]<<", "<<alpha_b[1]<<"].\n";
    return {-4} ;}
  
 Pythia8::Vec4 p_ta = comb_a_taumu[0].GetVisMom() ; // same between a & b
 Pythia8::Vec4 p_e = comb_a_emu[0].GetVisMom()   ; // same between a & b

  // assume combination 'a' is correct:
  std::vector<ExParticle> sel_taumu  = comb_a_taumu ;
  std::vector<ExParticle> sel_emu    = comb_a_emu ;
  double alpha_e  = alpha_a[0] ;
  double alpha_ta = alpha_a[1] ;
  double tauMuInv = alpha_a[2] ;
  double eMuInv   = alpha_a[3] ;
  double M4       = alpha_a[4] ;
  double mfi_sq_p = alpha_a[5] ;

  // but if 'b' is correct, changes the values
  if ( alpha_b_status ) 
    sel_taumu  = comb_b_taumu ;
    sel_emu    = comb_b_emu ;
    alpha_e  = alpha_b[0] ;
    alpha_ta = alpha_b[1] ; 
    tauMuInv = alpha_b[2] ;
    eMuInv   = alpha_b[3] ;
    M4       = alpha_b[4] ;
    mfi_sq_p = alpha_b[5] ;


 Pythia8::Vec4 scaled_p_ta = alpha_ta*p_ta ;
 Pythia8::Vec4 scaled_p_e = alpha_e*p_e ;
  // ..........................................


  double tauMuInv_scaled  = (scaled_p_ta + sel_taumu[1].GetVisMom()).mCalc() ;
  double eMuInv_scaled    = (scaled_p_e + sel_emu[1].GetVisMom()).mCalc() ;

  double M4_scaled = (scaled_p_ta + sel_taumu[1].GetVisMom() + scaled_p_e + sel_emu[1].GetVisMom() ).mCalc() ;

  std::vector<double> out = {tauMuInv, eMuInv, M4, mfi_sq_p, 
                        tauMuInv_scaled, eMuInv_scaled, M4_scaled} ;


  return out;
}

//==============================================================
// Solves the scaling factors alpha
std::vector<double> alpha(std::vector<ExParticle>& emu_set, std::vector<ExParticle>& taumu_set)
{
  
  Pythia8::Vec4 p_ta = taumu_set[0].GetVisMom();
 Pythia8::Vec4 p_e = emu_set[0].GetVisMom();
 Pythia8::Vec4 p_mu_sum = taumu_set[1].GetVisMom() + emu_set[1].GetVisMom();

  double m_ta = 1.777 ; // GeV
  double m_H  = 125   ; // GeV

  double M4 = (p_ta + taumu_set[1].GetVisMom() + p_e + emu_set[1].GetVisMom() ).mCalc() ;
  double tauMuInv  = (p_ta + taumu_set[1].GetVisMom()).mCalc() ;
  double eMuInv    = (p_e + emu_set[1].GetVisMom()).mCalc() ;


  // ..........................................
  // Scaling
  double a = ( 2*p_ta*p_e / (pow(eMuInv, 2)*pow(tauMuInv, 2)) + pow(m_ta,2) / pow(tauMuInv, 4) ); 
  double b = 2*(p_mu_sum) * ( p_ta/pow(tauMuInv, 2) + p_e/pow(eMuInv, 2) );  
  double c = p_mu_sum.m2Calc() - pow(m_H,2);  
  // ..........................................

  double mfi_sq_p = ( -b + sqrt( pow(b,2) - 4*a*c ) ) / ( 2*a ) ; 
  // double mfi_sq_m = ( -b - sqrt( pow(b,2) - 4*a*c ) ) / ( 2*a ) ; 

  // ..........................................

  double alpha_ta = mfi_sq_p / pow(tauMuInv, 2);
  double alpha_e = mfi_sq_p / pow(eMuInv, 2);

  return {alpha_e, alpha_ta, tauMuInv, eMuInv, M4, mfi_sq_p} ;
}

//==============================================================
// Sanity check for alphas
bool check(std::vector<double> alpha)
{
  bool ok = true ;

  if ( alpha[0] < 1 ||  alpha[1] < 1 )
    ok = false ;
  
  return ok;
}
//==============================================================
