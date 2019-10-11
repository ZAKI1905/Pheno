/*
  Pheno class

  Last Updated by Zaki on July 6, 2019

*/

#include <vector>
// #include <ncurses.h>
#include <omp.h>
#include <chrono>
#include <thread>
#include "Pythia8/Pythia.h"
#include "../inc/Pheno.h"
#include "../inc/Basics.h"
#include "../inc/ExParticle.h"
#include "../inc/EV.h"
#include "../inc/GenJet.h"
#include "../inc/IdEff.h"
#include "../inc/Isolation.h"
#include "../inc/M2Cut.h"
#include "../inc/M4Cut.h"
#include "../inc/PtCut.h"
#include "../inc/PrapCut.h"
#include "../inc/OffZCut.h"
#include "../inc/STBinner.h"


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

void Pheno::input(std::string command, bool strip_space)
/*
  Input method that initializes the Pheno class:
*/
{
  // "strip_space" option is true by default

  // Saving the command in a list
  input_list.push_back(command) ;

  // Stripping the command from any spaces:
  if ( strip_space )
    command = strip(command, ' ');
  

  // Parsing the command
  std::vector<std::string> inp = pars(command, "=") ;

  std::string prop  = inp[0] ;
  bool value                 ;

  if(inp[1] == "true" ) {value = true  ; }
  if(inp[1] == "false") {value = false ; }

  // Cases with report options:
  std::vector<std::string> parsed_prop = pars(prop, ":") ;

  if( parsed_prop[0] == "report" ) 
  {
    if ( inp[1] == "true" || inp[1] == "false" )
    { 
      if ( parsed_prop[1] == "cuts" )
        Report_Cuts  = value ;
      if ( parsed_prop[1] == "jets" )
        Report_Jets  = value ;
      if ( parsed_prop[1] == "taus" )
        Report_Taus  = value ;
    }
    else 
    {
      if ( parsed_prop[1] == "cuts" )
        report_cuts_set  = ev_range_pars(inp[1])  ;
      if ( parsed_prop[1] == "jets" )
        report_jets_set  = ev_range_pars(inp[1])  ;
      if ( parsed_prop[1] == "taus" )
        report_taus_set  = ev_range_pars(inp[1])  ;
    }
  
  }


  if(prop == "Threads"  ) { req_threads = std::stoi(inp[1]) ; }


  if(prop == "print_events") 
  {
    // Parsing the input event numbers
    print_ev_set = ev_range_pars(inp[1]) ;
  }

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

    //................Recording funcs & vars................
    func_int.funcs.push_back({}) ;
    func_double.funcs.push_back({}) ;
    func_vec_double.funcs.push_back({}) ;

    rec_int.vars.push_back({}) ;
    rec_double.vars.push_back({}) ;
    rec_vec_double.vars.push_back({}) ;
    
    //......................................................
  }

  if ( prop == "fastjet" )
  {
    if ( inp[1] == "run" )
    {
      GenJetIdx = cut_list.size() - 1 ;
      cout<<"\n GenJetIdx is = "<<GenJetIdx<<" .\n"<<std::flush;
    }
    else
    {
      // fastjet options
      vector<std::string> tmp_fj_commands = {pars(inp[1], ":")[0]} ;
      stolst( tmp_fj_commands, pars(inp[1], ":")[1] )  ;

      fastjet_commands.push_back(tmp_fj_commands) ;

    }
    
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
    // Saving the LHE file path:
    LHE_Path = inp[1] ;

    // Parsing the lhe file path
    std::vector<std::string> parsed_lhe_path = pars(LHE_Path, "/", -1) ;

    // Extracting the lhe file name
    inp[1] = parsed_lhe_path [ parsed_lhe_path.size() - 1] ;

    // Removing the ".lhe" extenstion from the LHE file:
    std::vector<std::string> lhe_file_string = pars(inp[1], ".") ;

    File_LHE     = lhe_file_string[0] ;

    // LHE file path minus the file name ( + 4 for '.lhe')
    size_t lhe_str_len = File_LHE.length() + 4 ;
    LHE_Path = LHE_Path.substr(0, LHE_Path.length() - lhe_str_len);

  }

  // pythia options
    // pythia banner
    if(prop == "show_pythia_banner"    ) { show_pythia_banner  = value ; }
    // internal pythia options
    if(prop == "pythia"    )
    {
      pythia_commands.push_back(inp[1]) ;
    }

}

//--------------------------------------------------------------
template<class T>
void Pheno::input(std::string input, T (*func)(vector<ExParticle>& parts) )
{
  // Parsing the command
  std::vector<std::string> inp = pars(input, "=") ;

  if( inp[0] == "record")
  {
    rec_fun<T> tmp_rec  ;
    tmp_rec.f = func ;
    tmp_rec.name = inp[1] ;

    func_double.funcs[cut_list.size()].push_back(tmp_rec) ;
  }
}

//--------------------------------------------------------------
// Specialization for int instances
template<>
void Pheno::input<int>(std::string input, int (*func)(vector<ExParticle>& parts) )
{
  // Parsing the command
  std::vector<std::string> inp = pars(input, "=") ;

  if( inp[0] == "record")
  {
    rec_fun<int> tmp_rec  ;
    tmp_rec.f = func ;
    tmp_rec.name = inp[1] ;

    func_int.funcs[cut_list.size()].push_back(tmp_rec) ;
  }
}

//--------------------------------------------------------------
// Specialization for double vectors instances
template<>
void Pheno::input<vector<double> >(std::string input, vector<double> (*func)(vector<ExParticle>& parts) )
{
  // Parsing the command
  std::vector<std::string> inp = pars(input, "=") ;

  if( inp[0] == "record")
  {
    rec_fun<vector<double> > tmp_rec  ;
    tmp_rec.f = func ;
    tmp_rec.name = inp[1] ;

    func_vec_double.funcs[cut_list.size()].push_back(tmp_rec) ;
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
  else if ( name_str[0]  == "ta+_h" )
      tmp_lst.push_back({-ID_TAU}) ;
  else if ( name_str[0]  == "ta-_h" )
      tmp_lst.push_back({ID_TAU}) ;

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

  num_cuts_passed.resize(Tot_Num_Events) ;
  
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
    std::string tmp_pyth_lhe(shared_file_char);
    tmp_pyth_lhe = "Beams:LHEF = "+ LHE_Path + tmp_pyth_lhe + ".lhe";

    pythia_commands.push_back(tmp_pyth_lhe) ;

  // Initialize pythia from pythia_commands:
    for (size_t i=0 ;  i<pythia_commands.size() ; ++i)
    {
      pythia.readString(pythia_commands[i]) ;
    }

   // Initialize fastjet from fastjet_commands:
   // For now I don't know how to implement this (May-12-2019)
   // for (size_t i=0 ;  i<fastjet_commands.size() ; ++i)
   //       {
   //         GenJet.input(fastjet_commands[i]) ;
   //       }

  pythia.init() ;

  // Allow for possibility of a few faulty events.
  int nAbort      = 10 ;
  int iAbort      = 0  ;
  int iEvent_Glob = 1  ;
  
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

    // if ( pr_id == 0 && iEvent % 100 == 0)
    //   {
    //     double progress = (double) iEvent / ( (double) Tot_Num_Events / num_proc) ;
    //     showProgress(progress) ;
    //   }

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

    bool tau_report_bool = ( Report_Taus || contains( report_taus_set, ev.i() ) ) ;
    ev.input("Report_Taus", tau_report_bool) ;

    // Find hadronic taus
    ev.find_had_tau(tau_rep_str) ;

    // Leptons are the only particles to loop over.
    vector<ExParticle> PartList = ev(lept_id_list) ;

    // //---------------------Setting the cut report filename------------------
    // std::string cut_file_str(shared_file_char) ;
    // cut_file_str = "Pythia_Cut_Report_" + cut_file_str + ".txt" ;

    // ------------------------------Applying Cuts--------------------------
    // There is a life-time issue with genjet, so we define it here. 
    // This way the jet information is accessible all the way through "run_cut"
    // i.e. can be accessed via recording functions.
    // As for the cut information, I'm not sure now. There might be an
    // issue there too. Since they access EV via a reference to EV 
    // inside the base class "Cut".   
    GenJet genjet(ev) ; //GenJet constructor

    // Number of cuts passed by an event
    int cut_counter  = run_cuts(ev, PartList, cut_list, shared_file_char) ;

    // +1 for the intial N_l check
    int tot_num_cuts = cut_list.size() + 1 ;

    // #pragma omp critical
    num_cuts_passed[iEvent_Glob-1] = cut_counter ;

    if (  cut_counter != tot_num_cuts ) continue ;

    // -------------------------------Binning------------------------------

    // Update the list of hadronic taus (keep the ones that passed all the cuts)
    // Needed for the binning procedure
    ev.update_hadtaus(PartList) ;

    #pragma omp critical
    binner(ev, shared_file_char) ;

    }
  // ======================= END EVENT LOOP =======================
  // endwin();

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
 vector<vector<string> > cut_list, char* shared_file_char)
{

  //------Setting the cut report filename--------
  std::string cut_file_str(shared_file_char) ;
  cut_file_str = "Pythia_Cut_Report_" + cut_file_str + ".txt" ;

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

    bool cut_report_bool = ( Report_Cuts || contains( report_cuts_set, ev.i() ) ) ;
    if(cut_report_bool) c1->Cut::input(cut_rep_title+ ":" + cut_file_str) ;

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

    // Update the list of particles that passed the cuts so far
    ev.update_pass_lepts(part_lst) ;

    // run fastjet
    if ( i == GenJetIdx ) runGenJet(ev, shared_file_char) ;

    record(i, part_lst) ;

  }


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
  else  if (input == "OffZ"    ) { pCut.reset( new OffZCut(ev) ) ; }

  return pCut ;

}



//-------------------------------------------------------
// Runs fastjet and saves a list of jets, and reports if Report_Jets is true.
void Pheno::runGenJet(EV& ev, char* shared_file_char)
{   
  std::string jet_file_str(shared_file_char) ;

  bool jet_report_bool = ( Report_Jets || contains( report_jets_set, ev.i() ) ) ;
  if(jet_report_bool) { ev.GenJetPtr->input( {"Report_Jets", jet_file_str} ) ; }

  for (size_t i=0 ; i<fastjet_commands.size() ; ++i)
    ev.GenJetPtr->input(fastjet_commands[i]) ;

  ev.GenJetPtr->gen() ;
}
  
//-------------------------------------------------------
// Recording Values
void Pheno::record(size_t cut_idx, vector<ExParticle>& part_lst)
{

  // #pragma omp single
  rec_double.vars[cut_idx+1].resize( func_double.funcs[cut_idx+1].size() ) ;
  rec_vec_double.vars[cut_idx+1].resize( func_vec_double.funcs[cut_idx+1].size() ) ;
  rec_int.vars[cut_idx+1].resize( func_int.funcs[cut_idx+1].size() ) ;

  #pragma omp critical
  {
    // For double variables
    for (size_t j=0 ; j<func_double.funcs[cut_idx+1].size() ; ++j)
    {
      rec_var<double> tmp_var ;
      tmp_var.val = (*func_double.funcs[cut_idx+1][j].f)(part_lst) ;
      tmp_var.name = "_rec_" + func_double.funcs[cut_idx+1][j].name + "_" + File_LHE ;

      rec_double.vars[cut_idx+1][j].push_back( tmp_var ) ;
    }

    // For double vector variables
    for (size_t j=0 ; j<func_vec_double.funcs[cut_idx+1].size() ; ++j)
    {
      rec_var<vector<double> > tmp_var ;
      tmp_var.val = (*func_vec_double.funcs[cut_idx+1][j].f)(part_lst) ;
      tmp_var.name = "_rec_" + func_vec_double.funcs[cut_idx+1][j].name + "_" + File_LHE ;

      rec_vec_double.vars[cut_idx+1][j].push_back( tmp_var ) ;
    }

    // For int variables
    for (size_t j=0 ; j<func_int.funcs[cut_idx+1].size() ; ++j)
    {
      rec_var<int> tmp_var ;
      tmp_var.val = (*func_int.funcs[cut_idx+1][j].f)(part_lst) ;
      tmp_var.name = "_rec_" + func_int.funcs[cut_idx+1][j].name + "_" + File_LHE ;

      rec_int.vars[cut_idx+1][j].push_back( tmp_var ) ;
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
void Pheno::binner(EV& ev, char* shared_file_char)
{

  std::string cut_file_str(shared_file_char) ;
  cut_file_str = "Pythia_Cut_Report_" + cut_file_str + ".txt" ;

  bool cut_report_bool = ( Report_Cuts || contains( report_cuts_set, ev.i() ) ) ;

 // Read the input for each binner
  for( size_t i=0 ; i<binner_ptr_lst.size() ; ++i)
  {
    binner_ptr_lst[i]->input(ev) ;
    binner_ptr_lst[i]->bin_it(cut_report_bool, cut_file_str) ;
  }

  num_ev_passed += ev.weight() ;
}

//-------------------------------------------------------
// Reporting
void Pheno::final_report()
{
  cout<<"\n Total number of events passing the cuts: "<<num_ev_passed<<".\n" ;


  // .....................................
  // Output the recorded values
  // .....................................
  // Double values
  for (size_t i=0 ; i<rec_double.vars.size() ; ++i)
    {
      for (size_t j=0 ; j<rec_double.vars[i].size() ; ++j)
      {
        saveVec(rec_double.vars[i][j]) ;
      }
    }
  // Double vector values
  for (size_t i=0 ; i<rec_vec_double.vars.size() ; ++i)
    {
      for (size_t j=0 ; j<rec_vec_double.vars[i].size() ; ++j)
      {
        saveVec(rec_vec_double.vars[i][j]) ;
      }
    }
  // int values
  for (size_t i=0 ; i<rec_int.vars.size() ; ++i)
  {
    for (size_t j=0 ; j<rec_int.vars[i].size() ; ++j)
    {
      saveVec(rec_int.vars[i][j]) ;
    }
  }
  // .....................................


  saveVec(num_cuts_passed, "_rec_Num_Cuts_" + File_LHE);
  saveVec(input_list,  "_main_inputs_" + File_LHE);

  // Output for each binner
  for( size_t i=0 ; i<binner_ptr_lst.size() ; ++i)
   { binner_ptr_lst[i]->report(File_LHE) ; }
}
//==============================================================

//==============================================================
//                    explicit instantiations
//==============================================================

template void Pheno::input<double>(std::string, double (*func)(vector<ExParticle>&) ) ;

template void Pheno::input<vector<double> >(std::string, vector<double> (*func)(vector<ExParticle>&) ) ;

template void Pheno::input<int>(std::string, int (*func)(vector<ExParticle>&) ) ;

//==============================================================