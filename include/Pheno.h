#ifndef Pheno_H
#define Pheno_H

#include <zaki/String/String_Basic.h>

#include "Cut.h"
#include "GenJet.h"
#include "Binner.h"
//--------------------------------------------------------------
namespace PHENO
{
  
//==============================================================
class Pheno : public Prog
{

  //--------------------------------------------------------------
  public:

  typedef std::vector<ExParticle> ParticleLST ;
  // Constructor
  Pheno() ;

  // Destructor
  ~Pheno() ;

  // Adding user defined cuts
  void Input(CUTS::CutOptions) ;

  // Adding binner classes & their options
  void Input(BinnerOptions) ;

  // Inputing the parameters (bool is for stripping the spaces)
  void Input(std::string, bool=true) ;

  template<class T>
  void Input(std::string, T (*func)(ParticleLST&) ) ;
  
  // List of input parameters
  std::vector<std::string> input_list;

  // records variables
  void Record(size_t cut_idx, ParticleLST& ) ;


//................UNDER CONSTRUCTION................
  template<typename T>
  struct rec_fun 
  {
   std::string name = "";
   T (*f)(ParticleLST& parts) ;
  };
  
  // Record functions
  // ( initilized to have 0-th element to record
  //   before any cuts have been applied )
  // std::vector< std::vector<double (*)(ParticleLST& parts)> > rec_funcs = { {} };
  // std::vector<std::vector<rec_fun> > rec_funcs = { {} } ;/

  template<typename T>
  struct rec_fun_set 
  {
  std::vector<std::vector<rec_fun<T> > > funcs = { {} } ;
  };

  rec_fun_set<int> func_int;
  rec_fun_set<double> func_double;
  rec_fun_set<std::vector<double> > func_vec_double;

  //  Record list
  template<typename T>
  struct rec_var_set
  {
    std::vector< std::vector< std::vector<rec_var<T> > > > vars= { {} } ;
  };

  rec_var_set<int> rec_int;
  rec_var_set<double> rec_double;
  rec_var_set<std::vector<double> > rec_vec_double;

//................UNDER CONSTRUCTION................


  // handles the parallel options and runPythia
  void Run() ;

  std::vector<int> num_cuts_passed ;

  double num_ev_passed = 0 ;
  int req_threads = 1 ;
  int threads = 1 ;

  // Final report
  void FinalReport() ;

  // runs Pythia!
  void RunPythia(int pr_id) ;

  //--------------------------------------------------------------
  private:

    // Start time
    double start_time ;

    bool show_pythia_banner = false ;

    // Required list of states in the final state,
    // the format is
    // { { {2}, {ID_MUON}} , { {2}, {ID_TAU, -ID_TAU}}, ...  }.
    // This is used in break_ev_loop.
    std::vector<std::vector<std::vector<int> > > req_states ;
    std::vector<std::vector<std::vector<int> > > min_req_states ;
    std::vector<std::vector<std::vector<int> > > max_req_states ;

    // Dictionary for binary comparison operators
    int CompStr2Int(std::string) ;

    // Dictionary for req_states input
    void StateDict(std::vector<std::string>) ;

    // Run cuts (returns the number of cuts passed)
    int RunCuts(ExEvent&, ParticleLST&,
     std::vector<CUTS::CutOptions>, char*)  ;

    // Cut dictionary
    // std::shared_ptr<CUTS::Cut>  CutDict(ExEvent*, std::string) ;

    // List of cuts to be applied
    std::vector<CUTS::CutOptions> cut_list ;

    // List of cut pointers
    std::vector<std::shared_ptr<CUTS::Cut> > cut_ptr_list ;

    //---------------------------------------------
    // Report options:
    bool rep_num_cut_flag = false  ;
    bool rep_input_commands_flag = false ;
    bool report_cuts_flag  = false ;
    // Special set of events for reporting cuts
    std::vector<int> report_cuts_set = {} ;  

    bool report_taus_flag  = false ;
    std::vector<int> report_taus_set = {} ;

    bool report_jets_flag  = false ;
    std::vector<int> report_jets_set = {} ;
    //---------------------------------------------

    // Event print set
    std::vector<int> print_ev_set = {} ;

   // File options:
    std::string file_LHE  = "" ;
    std::string LHE_path  = "" ;

    /* Breaks the event loop if some conditions are met,
     and continues to the next event */
    bool break_ev_loop(ParticleLST& ) ;

    /*
    Pythia options are saved in a list
     For a full list, check:
     http://home.thep.lu.se/~torbjorn/pythia81html/MainProgramSettings.html.
    */
    std::vector<std::string>  pythia_commands ;

    // fastjet options:
    std::vector< std::vector<std::string> >   fastjet_commands ;
    
    void RunGenJet(ExEvent& ev, char*) ;

    // some random high value so by default fastjet is not run
    size_t gen_jet_idx = 50; 

    // Initializes the binner instances with the given options
    // void InitBinner() ;

    // bins the event
    void BinEvents(ExEvent& ev, char*);

    // Lis of binning classes and their options
    // std::vector<std::vector<std::string> > bin_list ;
    std::vector<BinnerOptions> bin_list ;

    // Binner dictionary
    // std::shared_ptr<Binner> bin_dict(std::string) ;

    // A list of binner pointers
    // std::vector<std::shared_ptr<Binner> > binner_ptr_lst ;

    // The bin set
    // std::vector<std::vector<std::vector<double> > > bin_set = std::vector<std::vector<std::vector<double> > >(10, std::vector<std::vector<double> >(3, std::vector<double>(5)) ) ;

    // Total number of events
    int tot_num_events = 0 ;
};

//=============================================================
} // PHENO namespace
//=============================================================

#endif /*Pheno_H*/
