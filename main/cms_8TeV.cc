/*

  This file is for finding the limits from:
    '  CMS Collaboration,
    p V. Khachatryan et al., “Searches for R-parity-violating 
    supersymmetry in ppcollisions at (s) = 8 TeV in final 
    states with 0-4 leptons,” Phys. Rev. D94 (2016)
    no. 11, 112009, arXiv:1606.08076 [hep-ex].'

  - Last Updated by Zaki on August 29, 2019
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


double ftest(std::vector<ExParticle>&);

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
  // Use ',' for separating event ranges, or single events.
  // Use '-' for ranges.
  // phen.input("report:cuts=1-500") ;

  phen.input("report:taus=" + report_input_str) ;
  // phen.input("report:taus=5-17,155,122") ;
  phen.input("report:jets=" + report_input_str) ;
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

  // ----------------------Cut options----------------------

  // Required final states ( use e for both e+ and e-)
  
  phen.input( "required_set=emuta_h>=3" ) ; // N_l >= 3
  // phen.input( "required_set=e+>=1,e->=1,mu+>=1,mu->=1" ) ;
  // phen.input( "required_set=ta_h>=2,mu+>=1,mu->=1" ) ;
  // phen.input( "required_set=ta_h=1,emu=3" ) ;
  // phen.input( "required_set=ta_h=1,emu>=2" ) ;

  // ---------------------- Cuts ----------------------
  // You can move the recording function(s) between the cuts
  // it will record the values for particles passing
  // the cut prior to it. The functions defined here or
  // in included header files can be used.
  // phen.input("record=invMass_0", ftest);
  
  // ID_Eff cut
  phen.input( "cuts=ID_Eff:drop_low_eff=true" ) ;

  // Cut on M_l+l- 
  phen.input( "cuts=M2:M2_Cut_Value=12" ) ;

  /*  p_T Cut Conditions:
        e & mu: pt>= 10 GeV  (at least 1 > 20 GeV)
        t_h: pt>= 20 GeV
  */
  phen.input( "cuts=PT:lead=20, sub_lead=10, extra=10, had_tau=20" ) ;

  /*  prap Cut Conditions:
        e & mu: |eta| < 2.4
        t_h: |eta| < 2.3
  */
  phen.input( "cuts=PRap:e=2.4, mu=2.4, had_tau=2.3" ) ;

  phen.input("record=test_after_Prap", ftest);
  
  // Isolation cut
  phen.input( "cuts=ISO" ) ;

  //-------------------------
  // fastjet options
  phen.input("fastjet=Def:Algorithm=antikt_algorithm, R=0.5") ;
  phen.input("fastjet=Selector:EtaMax=2.5") ;
  phen.input("fastjet=Selector:PtMin=30") ;

  // running fastjet
  phen.input("fastjet=run") ;
  //------------------------------------------------------

  // Binning  
  phen.input( "Bin=STBinner:ST_Bins=0-300-600-1000-1500" ) ;

  phen.run() ;

  return 0 ;
}

//==============================================================
// You can change the body of this function
// and use it for recording variables, and vectors
double ftest(std::vector<ExParticle>& parts)
{
  double test_out = 0 ;

  for (size_t i=0 ; i<parts.size() ; ++i)
  {
    test_out += parts[i].px();
  }

  return test_out;
}

//==============================================================
