#ifndef Pheno_H
#define Pheno_H

#include "Pythia8/Pythia.h"
#include "../include/EV.h"
#include "../include/Cut.h"
#include "../include/GenJet.h"
#include "../include/Binner.h"

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

  // Inputing the parameters
  void input(std::string) ;                              

  void input(std::string, double (*func)(vector<ExParticle>&) ) ;

  // records variables
  void record(size_t cut_idx, vector<ExParticle>& ) ;

  struct rec_fun {
   std::string name = "";
   double (*f)(vector<ExParticle>& parts) ;
  };
  // Record functions 
  // ( initilized to have 0-th element to record 
  //   before any cuts have been applied )
  // vector< vector<double (*)(vector<ExParticle>& parts)> > rec_funcs = { {} };
  vector<vector<rec_fun> > rec_funcs = { {} } ;

  // Double record list 
  vector< vector< vector<rec_var> > > rec_doub_lst= { {} } ;

  // handles the parallel options and runPythia
  void run() ; 

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
     vector<vector<std::string> >, std::string)  ;

    // Cut dictionary 
    std::shared_ptr<Cut>  cut_dict(EV&, string) ;

    // List of cuts to be applied
    vector<vector<std::string> > cut_list ;

    // List of cut pointers
    vector<std::shared_ptr<Cut> > CutPtrList ;

    // Report options: 
    bool Report_Cuts  = false ;
    bool Report_Taus  = false ;
    bool Report_Jets  = false ;

    // Event print set
    vector<int> print_ev_set = {} ;
    
   // File options:
    std::string File_LHE  = "" ;

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
   std::vector<string>   fastjet_commands ;

    // Initializes the binner instances with the given options
    void initBinner() ;
    
    // bins the event
    void binner(EV& ev, std::string);

    // Lis of binning classes and their options
    vector<vector<std::string> > bin_list ;
  
    // Binner dictionary
    std::shared_ptr<Binner> bin_dict(string) ;

    // A list of binner pointers
    vector<std::shared_ptr<Binner> > binner_ptr_lst ;

    // Total number of events
    int Tot_Num_Events = 0 ;
  
};

//=============================================================

#endif /*Pheno_H*/
