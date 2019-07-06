/*
  Pheno class

  Last Updated by Zaki on July 6, 2019

*/

#include <vector>
#include <omp.h>
#include <chrono> 
#include <thread>
#include "Pythia8/Pythia.h"
#include "../include/Pheno.h"
#include "../include/Basics.h"
#include "../include/ExParticle.h"
#include "../include/EV.h"
#include "../include/GenJet.h"
#include "../include/IdEff.h"
#include "../include/Isolation.h"
#include "../include/M2Cut.h"
#include "../include/M4Cut.h"
#include "../include/PtCut.h"
#include "../include/PrapCut.h"
#include "../include/STBinner.h"

using namespace Pythia8 ;

//==============================================================

//--------------------------------------------------------------
// Constructor
Pheno::Pheno(){ start_time = omp_get_wtime() ;}

// Destructor
Pheno::~Pheno()
{
  printf("\nNumber of threads used: %2d, Processing Time:  %.2f s.\n",
   threads, omp_get_wtime()-start_time) ;

}

//--------------------------------------------------------------

void Pheno::input(std::string command)
/*
  Input method that initializes the Pheno class:
*/
{
  // Parsing the command
  std::vector<std::string> inp = pars(command, "=") ;

  std::string prop  = inp[0] ;
  bool value                 ;

  if(inp[1] == "true" ) {value = true  ; }
  if(inp[1] == "false") {value = false ; }

  // Cases with report options:
  if(prop == "report:cuts" ) { Report_Cuts  = value ; }
  if(prop == "report:jets" ) { Report_Jets  = value ; }
  if(prop == "report:taus" ) { Report_Taus  = value ; }

  if(prop == "Threads"  ) { req_threads = std::stoi(inp[1]) ; }


  if(prop == "print_events") { stolst(print_ev_set, inp[1]) ; }

  if( prop == "required_set" )
  {
    vector<std::string> tmp_req_lst ;
    stolst( tmp_req_lst, inp[1] )  ;

    state_dict(tmp_req_lst) ;
  }

  if(prop == "cuts")
  {
    vector<std::string> tmp_cut_lst = {pars(inp[1], ":")[0]} ;
    stolst( tmp_cut_lst, pars(inp[1], ":")[1] )  ;
    cut_list.push_back(tmp_cut_lst) ;
    rec_funcs.push_back({}) ;
    rec_doub_lst.push_back({}) ;
  }

  if(prop == "Bin") 
  { 
    vector<std::string> tmp_bin_lst = {pars(inp[1], ":")[0]} ;
    stolst( tmp_bin_lst, pars(inp[1], ":")[1] ) ;
    bin_list.push_back(tmp_bin_lst) ;
  }

  if( prop == "Events") { Tot_Num_Events = std::stoi(inp[1]) ; }

  if(prop == "file:lhe"    )
  {
    // Removing the ".lhe" extenstion from the LHE file:
    std::vector<std::string> lhefilestring = pars(inp[1], ".") ;
    File_LHE     = lhefilestring[0] ;
  }

  // pythia options
    // pythia banner
    if(prop == "show_pythia_banner"    ) { show_pythia_banner  = value ; }
    // internal pythia options
    if(prop == "pythia"    )
    {
      pythia_commands.push_back(inp[1]) ;
    }

  // fastjet options
  if(prop == "fastjet"    )
    {
      fastjet_commands.push_back(inp[1]) ;
    }

}

//--------------------------------------------------------------
void Pheno::input(std::string input, double (*func)(vector<ExParticle>& parts) )
{
  // Parsing the command
  std::vector<std::string> inp = pars(input, "=") ;

  if( inp[0] == "record")
  {
    rec_fun tmp_rec  ;
    tmp_rec.f = func ;
    tmp_rec.name = inp[1] ;

    rec_funcs[cut_list.size()].push_back(tmp_rec) ;
  }
}

//--------------------------------------------------------------
int Pheno::comp_str2int(std::string input)
{
  if (input == ">=")
    return 1 ;
  else if (input == "<=")
    return 2 ;
  else if (input == ">")
    return 3 ;
  else if (input == "<")
    return 4 ;
  else if (input == "=")
    return 5 ;
  else if (input == "!=")
    return 6 ;
  else 
    return 0 ;
}

//--------------------------------------------------------------
void Pheno::state_dict( vector<std::string> lst_in)
{
  // The binary comparisons: {">=", "<=", ">", "<", "=", "!="}
  // Note that ">=" should precede "=" and ">" for pars to 
  // work correctly. Otherwise, ">=" could be read as ">" or "="
  vector<std::string> binary_comp = {">=", "<=", ">", "<", "=", "!="} ;
  for (size_t i=0 ; i<lst_in.size() ; ++i)
  {
    vector<std::string> name_str = pars(lst_in[i], binary_comp) ;
    vector<vector<int> > tmp_lst = { { std::stoi(name_str[2]), 
                                      comp_str2int(name_str[1]) } } ;

        if ( name_str[0] == "e+"  )
        tmp_lst.push_back({-ID_ELECTRON}) ;

  else if ( name_str[0]  == "e-"  )
        tmp_lst.push_back({+ID_ELECTRON}) ;
  
  else if ( name_str[0]  == "e"  )
        tmp_lst.push_back({+ID_ELECTRON, -ID_ELECTRON}) ;
  
  else if ( name_str[0]  == "mu+" )
        tmp_lst.push_back({-ID_MUON}) ;

  else if ( name_str[0]  == "mu-" )
        tmp_lst.push_back({ID_MUON}) ;

  else if ( name_str[0]  == "mu" )
        tmp_lst.push_back({+ID_MUON, -ID_MUON}) ;

  else if ( name_str[0]  == "emu" )
      tmp_lst.push_back({+ID_ELECTRON, -ID_ELECTRON,
                         +ID_MUON, -ID_MUON}) ;

  else if ( name_str[0]  == "emuta_h" )
      tmp_lst.push_back({+ID_ELECTRON, -ID_ELECTRON,
                         +ID_MUON, -ID_MUON,
                         +ID_TAU, -ID_TAU}) ;

  else if ( name_str[0]  == "ta_h" )
      tmp_lst.push_back({+ID_TAU, -ID_TAU}) ;
  
  else 
    printf("\n No matching definition for %s.", name_str[0].c_str()) ;

  req_states.push_back(tmp_lst) ;
  }

}

//--------------------------------------------------------------
void Pheno::run()
{
  omp_set_num_threads(req_threads) ;
  std::cout<<"\n Threads requested: "<<req_threads<<endl ;

  // Initializing the binner classes
  initBinner() ;

  #pragma omp parallel for firstprivate(show_pythia_banner, cut_list, Report_Cuts, Report_Taus, Report_Jets, print_ev_set, File_LHE, pythia_commands, fastjet_commands) shared(binner_ptr_lst)
  for (int j=0 ; j<req_threads ; ++j)
  {
    // Asking one of the threads to save the available threads
    if( j==0 ) threads = omp_get_num_threads() ;

    runPythia(j) ;

  }

  final_report() ;
}

//-----------------------------------
void Pheno::runPythia(int pr_id)
/*
  Runs pythia, input the events in EV's and apply cuts
  and make reports, along with binning.
*/
{

  //Generator.
    Pythia pythia("", show_pythia_banner) ;

  // Making a shared character for naming all the files
    char shared_file_char[150] ;
    sprintf(shared_file_char, "%s_%d", File_LHE.c_str(), pr_id) ;

  // Inputing the LHE file in pythia
    std::string tmp_pyth_lhe(shared_file_char) ;
    tmp_pyth_lhe = "Beams:LHEF = _lhe/"+ tmp_pyth_lhe +".lhe" ;
    pythia_commands.push_back(tmp_pyth_lhe) ;

  // Initialize pythia from pythia_commands:
    for (size_t i=0 ;  i<pythia_commands.size() ; ++i)
    {
      pythia.readString(pythia_commands[i]) ;
    }

  pythia.init() ;

  // Allow for possibility of a few faulty events.
  int nAbort      = 10 ;
  int iAbort      = 0  ;
  int iEvent_Glob = 1  ; 
  std::vector<int> num_cuts_passed ;

  // ======================= BEGIN EVENT LOOP =======================
  
  // Begin event loop; generate until none left in input file.
  for (int iEvent=0  ; ; ++iEvent)
  {
    // Generate events, and check whether generation failed.
    if (!pythia.next())
    {
        // If failure because reached end of file then exit event loop.
        if (pythia.info.atEndOfFile()) break ;

        // First few failures write off as "acceptable" errors, then quit.
        if (++iAbort < nAbort) continue ;
        break ;
    }

    // ------------------------------Event info--------------------------
    int num_proc = req_threads ;
    iEvent_Glob = pr_id*(Tot_Num_Events / num_proc) + iEvent + 1 ;

    // Creating the ev object, by taking a pythia event and it's number
    EV ev(iEvent_Glob, pythia.event) ;

    // Reporting events
    if( contains(print_ev_set, iEvent_Glob) )
    {
      std::string event_rep_str(shared_file_char) ;
      event_rep_str = "Event_Report_"+event_rep_str + "_" +
                      std::to_string(iEvent_Glob)+".txt" ;
      ev.print(event_rep_str) ;
    }

    //If Report_Taus (private) is true, it will report them
    std::string tau_rep_str(shared_file_char) ;
    tau_rep_str = "Tau_Report_" + tau_rep_str + ".txt" ;

    ev.input("Report_Taus", Report_Taus) ;

    // Find hadronic taus
    ev.find_had_tau(tau_rep_str) ;

    // Leptons are the only particles to loop over.
    vector<ExParticle> PartList = ev(lept_id_list) ;

    //---------------------Setting the cut report filename------------------
    std::string cut_file_str(shared_file_char) ;
    cut_file_str = "Pythia_Cut_Report_" + cut_file_str + ".txt" ;

    // ------------------------------Applying Cuts--------------------------
    // Number of cuts passed by an event
    int cut_counter  = run_cuts(ev, PartList, cut_list, cut_file_str) ;

    // +1 for the intial N_l check
    int tot_num_cuts = cut_list.size() + 1 ; 

    num_cuts_passed.push_back(cut_counter) ;

    if (  cut_counter != tot_num_cuts ) continue ;

    // -------------------------------FASTJET-------------------------------
    // runs fastjet and saves a list of jets, and reports if Report_Jets is true.
    GenJet genjet(ev) ; //GenJet constructor
    if(Report_Jets) { genjet.input("Report_Jets", File_LHE, (double)pr_id) ; }
    genjet.input("algorithm", "antikt_algorithm", 0.5) ;
    genjet.input("Selector", "PtMin", 30) ;
    genjet.input("Selector", "EtaMax", 2.5) ;
    // Taking the ref to the pseudo jets in ev, along with the event #
    genjet.gen() ;
    // -------------------------------Binning------------------------------

    // Update the list of hadronic taus (keep the ones that passed all the cuts)
    // Needed for the binning procedure
    ev.update_hadtaus(PartList) ;

    #pragma omp critical
    binner(ev, cut_file_str) ;

    }
  // ======================= END EVENT LOOP =======================
  // endwin();
  // ------------------------------------------------
  // Recording quantities in a text file
  std::string out_file_str(shared_file_char) ;

  saveVec(num_cuts_passed, "Num_Cuts_"+out_file_str) ;

  // ------------------------------------------------
}

//--------------------------------------------------------------
// The conditions is true when we want to continue the event
// loop to the next iteration, i.e. cut is not passed. 
// So the conditions here are the logical negation of :
//       {">=", "<=", ">", "<", "=", "!="}
bool Pheno::break_ev_loop(vector<ExParticle>& prt_lst)
{

  for ( size_t i=0 ; i < req_states.size() ; ++i)
  {
    if ( req_states[i][0][1] == 1 )
      {if ( count(prt_lst, req_states[i][1]) <  req_states[i][0][0] )
        return true ;}

    else if ( req_states[i][0][1] == 2 )
      {if ( count(prt_lst, req_states[i][1]) >  req_states[i][0][0] )
        return true ;}

    else if ( req_states[i][0][1] == 3 )
      {if ( count(prt_lst, req_states[i][1]) <=  req_states[i][0][0] )
        return true ;}

    else if ( req_states[i][0][1] == 4 )
      {if ( count(prt_lst, req_states[i][1]) >=  req_states[i][0][0] )
        return true ;}
    
    else if ( req_states[i][0][1] == 5 )
      {if ( count(prt_lst, req_states[i][1]) !=  req_states[i][0][0] )
        return true ;}
    
    else if ( req_states[i][0][1] == 6 )
      {if ( count(prt_lst, req_states[i][1]) ==  req_states[i][0][0] )
        return true ;}
  }

  return false ;
}

//-------------------------------------------------------
// Cuts
int Pheno::run_cuts(EV& ev, vector<ExParticle>& part_lst,
 vector<vector<string> > cut_list, std::string cut_file_str)
{

  std::string cut_name ;
  std::string cut_rep_title = "Report_Cut:" ;

  // Number of cuts that are passed by the event
    int cut_count = 0 ;

  // N_l cut: It checks how many of the input events pass
  // the required final state conditions
  if ( break_ev_loop(part_lst) ) return cut_count ;
    cut_count++ ;

  // Recording var's before any cuts  
  record(-1, part_lst) ;

  for( size_t i=0 ; i<cut_list.size() ; ++i)
  {
    // the cut name:
    cut_name = cut_list[i][0] ;
    cut_rep_title += cut_name ;

    std::shared_ptr<Cut> c1 = cut_dict(ev, cut_name) ;

    // Checking if c1 is NULL pointer
    if ( !c1 )    
    { 
      if ( ev.i() == 1 )
      std::cerr<<"\n\n WARNING: Invalid cut input ignored! \n"<<std::flush ;
      cut_rep_title += "(ignored) > " ;
      cut_count++ ;
      continue ;
    }
    
    // Saving the pointer to current cut to the event
    ev.addCutPtr(c1) ;

    // Saving the name of the cut in the cut instance
    c1->Cut::input("Name:"+cut_name) ;

    if(Report_Cuts) c1->Cut::input(cut_rep_title+ ":" + cut_file_str) ;

    // Read the input for each cut
    for( size_t j=1 ; j<cut_list[i].size() ; ++j)
    {
      c1->input(cut_list[i][j]) ;
    }

    // Applying cuts
    c1->apply(part_lst) ;

    cut_rep_title += " > " ;

    if ( break_ev_loop(part_lst) ) return cut_count ;
    cut_count++ ;

    record(i, part_lst) ;

  }

  // Update the list of particles that passed all the cuts
    ev.update_pass_lepts(part_lst) ;

  return cut_count ;
}

//-------------------------------------------------------
// Cut dictionary
std::shared_ptr<Cut> Pheno::cut_dict(EV& ev, string input)
{

  std::shared_ptr<Cut> pCut ;

        if (input == "ID_Eff"  ) { pCut.reset( new IdEff(ev) )   ; }
  else  if (input == "M2"      ) { pCut.reset( new M2Cut(ev) )   ; }
  else  if (input == "PT"      ) { pCut.reset( new PtCut(ev) )   ; }
  else  if (input == "PRap"    ) { pCut.reset( new PrapCut(ev))  ; }
  else  if (input == "ISO"     ) { pCut.reset( new Isolation(ev)); }
  else  if (input == "M4"      ) { pCut.reset( new M4Cut(ev) )   ; }

  return pCut ;

}

//-------------------------------------------------------
// Recording Values
void Pheno::record(size_t cut_idx, vector<ExParticle>& part_lst)
{
  
  // #pragma omp single
  rec_doub_lst[cut_idx+1].resize( rec_funcs[cut_idx+1].size() ) ;

  #pragma omp critical
  {
    for (size_t j=0 ; j<rec_funcs[cut_idx+1].size() ; ++j)
    { 
      rec_var tmp_var ;
      tmp_var.val = (*rec_funcs[cut_idx+1][j].f)(part_lst) ;
      tmp_var.name = "_rec_" + rec_funcs[cut_idx+1][j].name + "_" + File_LHE ;

      rec_doub_lst[cut_idx+1][j].push_back( tmp_var ) ;

    }

  }

}

//-------------------------------------------------------
// Binner dictionary
std::shared_ptr<Binner> Pheno::bin_dict(string input)
{
  std::shared_ptr<Binner> pBinner ;

    if (input == "STBinner"  ) { pBinner.reset( new STBinner() ) ; }

  return pBinner ;

}

//-------------------------------------------------------
// Initializing the binner classes
void Pheno::initBinner()
{    

  for( size_t i=0 ; i<bin_list.size() ; ++i)
  {
    // the binner name:
    std::string binner_name = bin_list[i][0] ;

    std::shared_ptr<Binner> bi = bin_dict(binner_name) ;

    // Read the input for each binner
    for( size_t j=1 ; j<bin_list[i].size() ; ++j)
    {
      bi->input(bin_list[i][j]) ;
    }

    binner_ptr_lst.push_back(bi) ;
  }
}

//-------------------------------------------------------
// Binning the event 
// Has to be within critical statement in parallel region!
void Pheno::binner(EV& ev, std::string cut_file_str)
{    
  
 // Read the input for each binner
  for( size_t i=0 ; i<binner_ptr_lst.size() ; ++i)
  {
    binner_ptr_lst[i]->input(ev) ;
    binner_ptr_lst[i]->bin_it(Report_Cuts, cut_file_str) ;
  }

  num_ev_passed += ev.weight() ;
}

//-------------------------------------------------------
// Reporting 
void Pheno::final_report() 
{
  cout<<"\n Total number of events passing the cuts: "<<num_ev_passed<<".\n" ;

  // Output the recorded values
  for (size_t i=0 ; i<rec_doub_lst.size() ; ++i)
    {
      for (size_t j=0 ; j<rec_doub_lst[i].size() ; ++j)
      {
        saveVec(rec_doub_lst[i][j]) ;
      }  
    }
      
  // Output for each binner
  for( size_t i=0 ; i<binner_ptr_lst.size() ; ++i)
   { binner_ptr_lst[i]->report(File_LHE) ; }
}
//==============================================================
