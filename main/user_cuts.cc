/*

  This file is for adding user defined cuts.

  - Last Updated by Zaki on March 24, 2020
*/

#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>
#include <chrono>
#include <ctime>
#include <omp.h>

#include "../include/Pheno.h"

// Cut classes header files
#include "../include/IdEffCut.h"
#include "../include/IsolationCut.h"
#include "../include/M2Cut.h"
#include "../include/M4Cut.h"
#include "../include/PtCut.h"
#include "../include/PrapCut.h"
#include "../include/OffZCut.h"

// Binner classes header files
#include "../include/STBinner.h"

double ftest(std::vector<PHENO::ExParticle>&);
//==============================================================
// .............................................................
//                          CUTS
// .............................................................
//  Defining Cuts:
// User defined cut sub-classes should contain two overriden
// methods from the Cut class, namely:
// 'CutCond' & 'Clone'
// ..............................
//       * IMPORTANT *
//  The objects MUST be heap allocated (use 'new'), 
//   & they SHOULD'NT be deleted by user (don't write 'delete')
// ..............................
// Options: 
// It can optionally have 'Input(std::string)' method
// with this method, the options can be input
// directly into Pheno object, as a string and
// separated by commas.
// .............................................................
//                          BINNERS
// .............................................................
//  Defining Binners:
// Binner class similarly needs:

//    For inputting event:
// (1) 'void Input(ExEvent&)'
//
//    For inputting properties:
// (2) 'void Input(std::string)'
//
//    For deep-copying the object
// (3) 'std::shared_ptr<Binner> Clone()'
//
//    For binning the event, & adding to cut report if "true"
// (4) 'void BinIt(bool cut_report, std::string)'
//
//    For reporting the binning results
// (5) 'void Report(std::string) const'
// .............................................................
class MyCut : public PHENO::CUTS::Cut
{

  public:

    // Default constructor
    MyCut() {SetName("my_cut");}

    // Constructor
    // MyCut(ExEvent* ev) : Cut(ev) {}

  private:
    // Virtual method from cut class
    void CutCond(std::vector<PHENO::ExParticle>& in_set) override
    {

    } 

    std::shared_ptr<Cut> Clone() override
    {
      return std::shared_ptr<MyCut>(new MyCut(*this));
    }

};

//==============================================================

///////////////////////////////////////////////////
///////////////      main()     ///////////////////
///////////////////////////////////////////////////
int main(int argc,char *argv[])
{
#if PROFILING
  // Begin session 
  Zaki::Util::Instrumentor::BeginSession("user_def_cuts", "user_def_cuts.json");       
#endif

  // PROFILE_FUNCTION() ;
{  
  PROFILE_SCOPE("main") ;

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

  using namespace PHENO ;

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
  // Use ',' for separating event ranges, or single events.
  // Use '-' for ranges.
  // phen.Input("report:cuts=1-500") ;

  phen.Input("report:taus=" + report_input_str) ;
  // phen.Input("report:taus=5-17,155,122") ;
  phen.Input("report:jets=" + report_input_str) ;
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

  // ----------------------Cut options----------------------

  // Required final states ( use e for both e+ and e-)
  
  phen.Input( "required_set=emuta_h>=3" ) ; // N_l >= 3
  // phen.Input( "required_set=e+>=1,e->=1,mu+>=1,mu->=1" ) ;
  // phen.Input( "required_set=ta_h>=2,mu+>=1,mu->=1" ) ;
  // phen.Input( "required_set=ta_h=1,emu=3" ) ;
  // phen.Input( "required_set=ta_h=1,emu>=2" ) ;

  // ---------------------- Cuts ----------------------
  // You can move the recording function(s) between the cuts
  // it will record the values for particles passing
  // the cut prior to it. The functions defined here or
  // in included header files can be used.
  // phen.Input("record=invMass_0", ftest);

  // ID_Eff cut
  // IdEffCut* id_eff = new IdEffCut  ;
  phen.Input({new CUTS::IdEffCut, "drop_low_eff=true"}) ;

  // Cut on M_l+l- 
  // M2Cut* m2_cut = new M2Cut ;
  phen.Input({new CUTS::M2Cut ,"M2_Cut_Value=12"}) ;

  // /*  p_T Cut Conditions:
  //       e & mu: pt>= 10 GeV  (at least 1 > 20 GeV)
  //       t_h: pt>= 20 GeV
  // */
  // PtCut* pt_cut = new PtCut ;
  phen.Input({new CUTS::PtCut, "lead=20, sub_lead=10, extra=10, had_tau=20"}) ;

  // /*  prap Cut Conditions:
  //       e & mu: |eta| < 2.4
  //       t_h: |eta| < 2.3
  // */
  // PrapCut* prap_cut = new PrapCut;
  phen.Input({new CUTS::PrapCut, "e=2.4, mu=2.4, had_tau=2.3"}) ;

  // phen.Input("record=test_after_Prap", ftest);
  
  // MyCut* my_cut = new  MyCut;
  phen.Input(new  MyCut) ;

  // Isolation cut
  // IsolationCut* iso_cut = new  IsolationCut; 
  phen.Input(new  CUTS::IsolationCut) ;

  //-------------------------
  // fastjet options
  // phen.Input("fastjet=Def:Algorithm=antikt_algorithm, R=0.5") ;
  // phen.Input("fastjet=Selector:EtaMax=2.5") ;
  // phen.Input("fastjet=Selector:PtMin=30") ;

  // running fastjet
  // phen.Input("fastjet=run") ;
  //------------------------------------------------------

  // Binning  
  // phen.Input( "Bin=STBinner:ST_Bins=0-300-600-1000-1500" ) ;
  phen.Input({new STBinner, "ST_Bins=0-300-600-1000-1500"}) ;


  phen.Run() ;
}

#if PROFILING
  Zaki::Util::Instrumentor::EndSession();  // End Session
#endif

  return 0 ;
}

//==============================================================
// You can change the body of this function
// and use it for recording variables, and vectors
double ftest(std::vector<PHENO::ExParticle>& parts)
{
  double test_out = 0 ;

  for (size_t i=0 ; i<parts.size() ; ++i)
  {
    test_out += parts[i].px();
  }

  return test_out;
}

//==============================================================
