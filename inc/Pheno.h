#ifndef Pheno_H
#define Pheno_H

#include "Pythia8/Pythia.h"
#include "EV.h"
#include "Cut.h"
#include "GenJet.h"
#include "Binner.h"

using    namespace Pythia8 ;

//==============================================================
class Pheno
{
//--------------------------------------------------------------

 public:

  // Constructor
  Pheno() ;

  // Destructor
  ~Pheno() ;

  // Inputing the parameters (bool is for stripping the spaces)
  void input(std::string, bool=true) ;

  template<class T>
  void input(std::string, T (*func)(vector<ExParticle>&) ) ;
  
  // List of input parameters
  vector<std::string> input_list;

  // records variables
  void record(size_t cut_idx, vector<ExParticle>& ) ;


//................UNDER CONSTRUCTION................
  template<typename T>
  struct rec_fun 
  {
   std::string name = "";
   T (*f)(vector<ExParticle>& parts) ;
  };
  
  // Record functions
  // ( initilized to have 0-th element to record
  //   before any cuts have been applied )
  // vector< vector<double (*)(vector<ExParticle>& parts)> > rec_funcs = { {} };
  // vector<vector<rec_fun> > rec_funcs = { {} } ;/

  template<typename T>
  struct rec_fun_set 
  {
  vector<vector<rec_fun<T> > > funcs = { {} } ;
  };

  rec_fun_set<int> func_int;
  rec_fun_set<double> func_double;
  rec_fun_set<vector<double> > func_vec_double;

  //  Record list
  template<typename T>
  struct rec_var_set
  {
    vector< vector< vector<rec_var<T> > > > vars= { {} } ;
  };

  rec_var_set<int> rec_int;
  rec_var_set<double> rec_double;
  rec_var_set<vector<double> > rec_vec_double;

//................UNDER CONSTRUCTION................


  // handles the parallel options and runPythia
  void run() ;

  vector<int> num_cuts_passed ;

  double num_ev_passed = 0 ;
  int req_threads = 1 ;
  int threads = 1 ;

  // Final report
  void final_report() ;

  // runs Pythia!
  void runPythia(int pr_id) ;

//--------------------------------------------------------------

 private:

    // Start time
    double start_time ;

    bool show_pythia_banner = false ;

    // Required list of states in the final state,
    // the format is
    // { { {2}, {ID_MUON}} , { {2}, {ID_TAU, -ID_TAU}}, ...  }.
    // This is used in break_ev_loop.
    vector<vector<vector<int> > > req_states ;
    vector<vector<vector<int> > > min_req_states ;
    vector<vector<vector<int> > > max_req_states ;

    // Dictionary for binary comparison operators
    int comp_str2int(std::string) ;

    // Dictionary for req_states input
    void state_dict(vector<std::string>) ;

    // Run cuts (returns the number of cuts passed)
    int run_cuts(EV&, vector<ExParticle>&,
     vector<vector<std::string> >, char*)  ;

    // Cut dictionary
    std::shared_ptr<Cut>  cut_dict(EV&, string) ;

    // List of cuts to be applied
    vector<vector<std::string> > cut_list ;

    // List of cut pointers
    vector<std::shared_ptr<Cut> > CutPtrList ;

    //---------------------------------------------
    // Report options:
    bool Report_Cuts  = false ;
    // Special set of events for reporting cuts
    vector<int> report_cuts_set = {} ;  

    bool Report_Taus  = false ;
    vector<int> report_taus_set = {} ;

    bool Report_Jets  = false ;
    vector<int> report_jets_set = {} ;
  //---------------------------------------------

    // Event print set
    vector<int> print_ev_set = {} ;

   // File options:
    std::string File_LHE  = "" ;
    std::string LHE_Path  = "" ;

    /* Breaks the event loop if some conditions are met,
     and continues to the next event */
    bool break_ev_loop(vector<ExParticle>& ) ;

    /*
    Pythia options are saved in a list
     For a full list, check:
     http://home.thep.lu.se/~torbjorn/pythia81html/MainProgramSettings.html.
    */
   std::vector<string>  pythia_commands ;

    // fastjet options:
    vector< vector<std::string> >   fastjet_commands ;
    
    void runGenJet(EV& ev, char*) ;
    // some random high value so by default fastjet is not run
    size_t GenJetIdx = 50; 

    // Initializes the binner instances with the given options
    void initBinner() ;

    // bins the event
    void binner(EV& ev, char*);

    // Lis of binning classes and their options
    vector<vector<std::string> > bin_list ;

    // Binner dictionary
    std::shared_ptr<Binner> bin_dict(string) ;

    // A list of binner pointers
    vector<std::shared_ptr<Binner> > binner_ptr_lst ;

    // The bin set
    // vector<vector<vector<double> > > bin_set = vector<vector<vector<double> > >(10, vector<vector<double> >(3, vector<double>(5)) ) ;

    // Total number of events
    int Tot_Num_Events = 0 ;

};

//=============================================================
// void run(Pheno phen) ;

//=============================================================

#endif /*Pheno_H*/
