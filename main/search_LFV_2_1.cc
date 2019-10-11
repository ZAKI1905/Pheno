/*

  This file is for proposing new searches for
  the LFV_2_1 model.

  - Last Updated by Zaki on September 5, 2019
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


// LFV_2_1
vector<double> InvEpMum1(vector<ExParticle>& parts);

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
  phen.input("Threads=" + NUM_THREADS_str) ;

  // Total number of events
  phen.input("Events=" + Tot_Num_Events_str) ;
  phen.input("file:lhe=" + filename) ;

  // reporting options
  phen.input("report:cuts=" + report_input_str) ;

  // This is an exception list that will always
  //  print reports for the range of events listed.
  // Use ',' for separatng event ranges, or single events.
  // Use '-' for ranges.
  // phen.input("report:cuts=1-500") ;

  // phen.input("report:taus="+report_input_str) ;
  // phen.input("report:taus=5-17,155,122") ;
  // phen.input("report:jets="+report_input_str) ;
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
  phen.input( "show_pythia_banner=false" ) ;

  // Init settings:
  phen.input( "pythia=Init:showProcesses  = off" ) ;
  phen.input( "pythia=Init:showChangedParticleData  = off" ) ;
  phen.input( "pythia=Init:showMultipartonInteractions  = off" ) ;
  phen.input( "pythia=Init:showChangedSettings   = off" ) ;

  // Next settings:
  phen.input( "pythia=Next:numberShowProcess  = 0" ) ;
  phen.input( "pythia=Next:numberShowInfo  = 0" ) ;
  phen.input( "pythia=Next:numberShowLHA = 0" ) ;
  phen.input( "pythia=Next:numberShowEvent = 0" ) ;


  // Stat settings:
  phen.input( "pythia=Stat:showProcessLevel = off" ) ;

  // ----------------------Cut options----------------------------------------

  // Required final states ( use e for both e+ and e-)
  phen.input( "required_set= e+>=2, mu->=2" ) ; // LFV_2

  // ---------------------- Cuts ----------------------
  // You can move the recording function(s) between the cuts
  // it will record the values for particles passing
  // the cut prior to it. The functions defined here or
  // in included header files can be used.

  // ID_Eff cut
  phen.input( "cuts=ID_Eff:drop_low_eff=true" ) ;

  // Cut on M_l+l-
  phen.input( "cuts=M2:M2_Cut_Value=12" ) ;

  /*  p_T Cut Conditions:
        e & mu: pt>= 7 GeV  (at least 1 > 20 GeV)
        t_h: pt>= 20 GeV
  */
  phen.input( "cuts=PT:lead=20,sub_lead=7,extra=7,had_tau=20" ) ;

  /*  prap Cut Conditions:
        e & mu: |eta| < 2.4
        t_h: |eta| < 2.3
  */
  phen.input( "cuts=PRap:e=2.4,mu=2.4,had_tau=2.3" ) ;

  // Isolation cut
  phen.input( "cuts=ISO" ) ;

  //-------------------------
  // Don't have to run fastjet in this case
  // fastjet options
  // phen.input("fastjet=Def:Algorithm=antikt_algorithm,R=0.5") ;
  // phen.input("fastjet=Selector:EtaMax=2.5") ;
  // phen.input("fastjet=Selector:PtMin=30") ;
  
  // // running fastjet
  // phen.input("fastjet=run") ;
  //-------------------------

  // Demanding  m(l+l-) <= 75 GeV or 105 GeV <= m(l+l-)
  phen.input( "cuts=OffZ:OffZ_Cut_Min=75,OffZ_Cut_Max=105" ) ;

  // Demanding 120 GeV <= m(e+, mu-, e+, mu-) <= 130 GeV
  phen.input( "cuts=M4:M4_Cut_Min=120,M4_Cut_Max=130" ) ;  // LFV_2
  
  // -------------------------------------------------------------------------

  //------------------------------------------------------
  // Recording invariant masses
  // LFV_2
  phen.input("record=Ep_Mum_InvM", InvEpMum1) ;

  //------------------------------------------------------
  

  phen.run() ;

  return 0 ;
}

//==============================================================
// Invariant mass for (mu-, e+) pairs (for LFV_2_1 )
vector<double> InvEpMum1(vector<ExParticle>& parts)
{
  if (parts.size() != 4 )  return {-1, -1, -1};

  vector<ExParticle> sel_Ep ;
  vector<ExParticle> sel_Mum ;

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
