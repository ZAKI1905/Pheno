/*

  This file is for proposing new searches for
  the LFV_1 model.

  - Last Updated by Zaki on September 3, 2019
*/

#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>
// #include <vector>
#include <chrono>
#include <ctime>
#include <omp.h>
// #include "Pythia8/Pythia.h"
// #include "fastjet/ClusterSequence.hh"
// #include "fastjet/Selector.hh"
// #include "../inc/EV.h"
// #include "../inc/Basics.h"
#include "../include/Pheno.h"

// using namespace Pythia8 ;


std::vector<double> InvMeMu(std::vector<ExParticle>& parts);

///////////////////////////////////////////////////
///////////////      main()     ///////////////////
///////////////////////////////////////////////////
int main(int argc,char *argv[])
{
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

  // phen.input( "required_set=emuta_h>=3" ) ; // N_l >= 3
  phen.input( "required_set=e+>=1,e->=1,mu+>=1,mu->=1" ) ; // LFV_1
  // phen.input( "required_set=ta_h=1,emu>=2" ) ;
  // phen.input( "required_set=ta_h>=1, e>=1, mu+>=1, mu->=1" ) ; // LFV_3

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

  // Demanding 120 GeV <= m(e+, mu-, e-, mu+) <= 130 GeV
  phen.input( "cuts=M4:M4_Cut_Min=120,M4_Cut_Max=130" ) ;  // LFV_1
  
  // -------------------------------------------------------------------------

  //------------------------------------------------------
  // Recording invariant masses
  // LFV_1
  phen.input("record=E_Mu_InvM", InvMeMu) ;

  //------------------------------------------------------
  

  phen.run() ;

  return 0 ;
}

//==============================================================
// Invariant mass for (mu, e) pair (for LFV_1 )
std::vector<double> InvMeMu(std::vector<ExParticle>& parts)
{
  if (parts.size() != 4 )  return {-100, -100, -100};

  std::vector<ExParticle> sel_EpMum ;
  std::vector<ExParticle> sel_EmMup ;

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