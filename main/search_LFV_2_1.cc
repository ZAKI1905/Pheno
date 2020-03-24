/*

  This file is for proposing new searches for
  the LFV_2_1 model.

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

// LFV_2_1
std::vector<double> InvEpMum1(std::vector<ExParticle>& parts);

///////////////////////////////////////////////////
///////////////      main()     ///////////////////
///////////////////////////////////////////////////
int main(int argc,char *argv[])
{
  Instrumentor::Get().BeginSession("search_LFV_2_1", "Profile_search_LFV_2_1.json") ;        // Begin session  

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

  // phen.Input("report:taus="+report_input_str) ;
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
  phen.Input( "required_set= e+>=2, mu->=2" ) ; // LFV_2

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
  phen.Input(&iso);

  //-------------------------
  // Don't have to run fastjet in this case
  // fastjet options
  // phen.Input("fastjet=Def:Algorithm=antikt_algorithm,R=0.5") ;
  // phen.Input("fastjet=Selector:EtaMax=2.5") ;
  // phen.Input("fastjet=Selector:PtMin=30") ;
  
  // // running fastjet
  // phen.Input("fastjet=run") ;
  //-------------------------

  // Demanding  m(l+l-) <= 75 GeV or 105 GeV <= m(l+l-)
  OffZCut offz_cut;
  phen.Input({&offz_cut, "OffZ_Cut_Min=75,OffZ_Cut_Max=105"}) ;

  // Demanding 120 GeV <= m(e+, mu-, e+, mu-) <= 130 GeV
  M4Cut m4_cut;
  phen.Input({&m4_cut, "M4_Cut_Min=120,M4_Cut_Max=130"}) ;  // LFV_2
  
  // -------------------------------------------------------------------------

  //------------------------------------------------------
  // Recording invariant masses
  // LFV_2
  phen.Input("record=Ep_Mum_InvM", InvEpMum1) ;

  //------------------------------------------------------
  

  phen.Run() ;

  Instrumentor::Get().EndSession();  // End Session
  return 0 ;
}

//==============================================================
// Invariant mass for (mu-, e+) pairs (for LFV_2_1 )
std::vector<double> InvEpMum1(std::vector<ExParticle>& parts)
{
  if (parts.size() != 4 )  return {-1, -1, -1};

  std::vector<ExParticle> sel_Ep ;
  std::vector<ExParticle> sel_Mum ;

  for (size_t i=0 ; i<parts.size() ; ++i)
  {
    if ( parts[i].id() == -ID_ELECTRON)
      sel_Ep.push_back(parts[i]);

    if ( parts[i].id() == ID_MUON)
      sel_Mum.push_back(parts[i]);
  }

  if (sel_Ep.size() != 2 || sel_Mum.size() != 2 )  return {-2, -2, -2};

  // pair up the e+ and mu- in 2 possible ways, i.e. a & b
  std::vector<ExParticle> comb_a_1 = {sel_Ep[0], sel_Mum[0]} ;
  std::vector<ExParticle> comb_a_2 = {sel_Ep[1], sel_Mum[1]} ;

  std::vector<ExParticle> comb_b_1 = {sel_Ep[0], sel_Mum[1]} ;
  std::vector<ExParticle> comb_b_2 = {sel_Ep[1], sel_Mum[0]} ;

  // Find the invariant masses and their differences in each case
  double invM_a_1 = invM( comb_a_1 ) ; double invM_a_2 = invM( comb_a_2 ) ;
  double invM_b_1 = invM( comb_b_1 ) ; double invM_b_2 = invM( comb_b_2 ) ;

  double Diff_a = abs(invM_a_1 - invM_a_2);
  double Diff_b = abs(invM_b_1 - invM_b_2);

  // Assuming both pairs came from the ame particle (fi), 
  // the true combination is the one where the difference is smaller
  if ( Diff_a <= Diff_b )
    return {invM_a_1, invM_a_2, invM(parts)} ;
  else
    return {invM_b_1, invM_b_2, invM(parts)} ;
}

//==============================================================
