/* 
  GenJet class

  Last Updated by Zaki on August 27, 2019

*/ 

#include <iostream>

// #include <zaki/Vector/Vector_Basic.h>

#include "../include/Basics.h"
#include "../include/ExEvent.h"
#include "../include/GenJet.h"

//==============================================================
/*
 Note that we won't turn off the printing of FastJet banners
 due to the following request in "~/src/ClusterSequence.cc":

  " Please note that if you distribute 3rd party code
  that links with FastJet, that 3rd party code is NOT
  allowed to turn off the printing of FastJet banners
  by default. This requirement reflects the spirit of
  clause 2c of the GNU Public License (v2), under which
  FastJet and its plugins are distributed. "
*/

//--------------------------------------------------------------
// Basic  Constructor
PHENO::GenJet::GenJet(PHENO::ExEvent& ev) : ev_ref(ev){ ev.AddGenJetPtr(this); }

//--------------------------------------------------------------
// Copy  Constructor
// Careful about the ev_ref !
PHENO::GenJet& PHENO::GenJet::operator=(const PHENO::GenJet &other)
{
  return *this ;
}

//--------------------------------------------------------------
// Set Event reference
void PHENO::GenJet::SetEventRef(PHENO::ExEvent& in_event) 
{
  ev_ref = in_event ;
}

//--------------------------------------------------------------
// Takes a pointer to pseudo jets in the event
void PHENO::GenJet::GenerateJets()
{
  PROFILE_FUNCTION() ;
  //-----1-Jet Definition-----
  // Defining the jet
   fastjet::JetDefinition jet_def(jet_alg, jet_radius, recomb_scheme, strategy) ;
   
   // Jets description
   my_jet_dscr = jet_def.description() ; 
  //--------------------------

  //-----2-Ps-Jet-Update------
    ev_ref.UpdatePseudoJet() ;
  //--------------------------

  //-----3-Cluster Sequence---
  // runs the jet clustering with the jet definitions
    fastjet::ClusterSequence clust_seq(ev_ref.GetPseudoJets(), jet_def) ;
  //--------------------------

  //-----4-Selector-----------
  // get the resulting jets ordered in pt
    double ptmin = 5.0 ;
    inc_jets_before = fastjet::sorted_by_pt(clust_seq.inclusive_jets(ptmin)) ;

    inc_jets_after  = 
    fastjet::sorted_by_pt(jet_selector(clust_seq.inclusive_jets(ptmin))) ;
  //--------------------------

  //-----5-Finding the jets' charges------
  for(size_t i=0 ; i<inc_jets_before.size() ; ++i)
  {    
    int i_charge = 0 ;
    for (size_t j=0 ; j < inc_jets_before[i].constituents().size() ; ++j) 
      i_charge += inc_jets_before[i].constituents()[j].user_index();  

    inc_jets_before[i].set_user_index(i_charge) ;
  }
  //--------------------------


  //-----6-Isolation-Cut------
  // removing the non-isolated jets from inc_jets_after
    // isolate() ; // for 'cms_8TeV'
  // removing the jets failing tau_h isolation condition
    FakeTauIsolate() ; // for 'faketaus'
  //--------------------------

  //-----7-Reporting Jets-----
    if( report_jet_flag == true ) { Report() ; }
  //--------------------------

}

//--------------------------------------------------------------
// input method that takes jet options
void PHENO::GenJet::Input(std::vector<std::string> option)
{

  std::string main_option = option[0] ;

  //-----1-Definition-----
  if(main_option == "Def")
  {
    SetJetDef(option);
  }

  //-----2-Selector-----
  else if(main_option == "Selector")
  {
    SetSelector(option);
  }

  //-----3-Report-------
  else if(main_option == "Report_Jets")
  {
    sprintf(jet_rep_char, "Jet_Report_%s.txt", option[1].c_str());
    report_jet_flag = true ;
  }

}
//--------------------------------------------------------------
// Sets jet definition options
void PHENO::GenJet::SetSelector(std::vector<std::string> selector_set)
{

  // Start i from "1", since the first element is "Selector".
  for (size_t i =1 ; i< selector_set.size(); ++i)
  {
    std::vector<std::string> tmp_sel = Zaki::String::Pars(selector_set[i],"=");

    // ......Cases with 4 inputs..........
    if (tmp_sel[0] == "RapPhiRange")
    {
      std::vector<std::string> tmp_sel_range = Zaki::String::Pars(tmp_sel[1],"-", -1) ;

      if ( tmp_sel_range.size() !=4 ) 
      { Z_LOG_ERROR("The range must be four numbers --> ignoring this selector.") ; continue; }
      
      double rap_min =  std::stod(tmp_sel_range[0]);
      double rap_max =  std::stod(tmp_sel_range[1]);
      double phi_min =  std::stod(tmp_sel_range[2]);
      double phi_max =  std::stod(tmp_sel_range[3]);

      jet_selector = fastjet::SelectorRapPhiRange(rap_min, rap_max, phi_min, phi_max)*jet_selector ;

    }
    // ...................................

    // ......Cases with 2 inputs..........
    else if ( Zaki::String::EndsWith(tmp_sel[0], "Range") )
    {

      std::vector<std::string> tmp_sel_range = Zaki::String::Pars(tmp_sel[1],"-", -1) ;

      if ( tmp_sel_range.size() !=2 ) 
      { Z_LOG_ERROR("The range must be two numbers --> ignoring this selector."); continue; }

      double min = std::stod(tmp_sel_range[0]);
      double max =  std::stod(tmp_sel_range[1]);

      if (tmp_sel[0] == "AbsEtaRange")
        jet_selector = fastjet::SelectorAbsEtaRange(min, max)*jet_selector ;
      else if (tmp_sel[0] == "AbsRapRange") 
        jet_selector = fastjet::SelectorAbsRapRange(min, max)*jet_selector ;
      else if (tmp_sel[0] == "ERange") 
        jet_selector = fastjet::SelectorERange(min, max)*jet_selector ;
      else if (tmp_sel[0] == "EtaRange") 
        jet_selector = fastjet::SelectorEtaRange(min, max)*jet_selector ;
      else if (tmp_sel[0] == "EtRange") 
        jet_selector = fastjet::SelectorEtRange(min, max)*jet_selector ;
      else if (tmp_sel[0] == "PtRange") 
        jet_selector = fastjet::SelectorPtRange(min, max)*jet_selector ; 
      else if (tmp_sel[0] == "RapRange") 
        jet_selector = fastjet::SelectorRapRange(min, max)*jet_selector ;  
      else if (tmp_sel[0] == "PhiRange") 
        jet_selector = fastjet::SelectorPhiRange(min, max)*jet_selector ;
      else if (tmp_sel[0] == "Doughnut") 
        jet_selector = fastjet::SelectorDoughnut(min, max)*jet_selector ;
    }
    // ...................................

    // ......Cases with 1 inputs..........
    else
    {
      double val = std::stod(tmp_sel[1]) ;

      if (tmp_sel[0] == "AbsEtaMin")
        jet_selector = fastjet::SelectorAbsEtaMin(val)*jet_selector ;
      else if (tmp_sel[0] == "AbsEtaMax")
        jet_selector = fastjet::SelectorAbsEtaMax(val)*jet_selector ;

      else if (tmp_sel[0] == "AbsRapMin")
        jet_selector = fastjet::SelectorAbsRapMin(val)*jet_selector ;
      else if (tmp_sel[0] == "AbsRapMax")
        jet_selector = fastjet::SelectorAbsRapMax(val)*jet_selector ;

      else if (tmp_sel[0] == "EMin")
        jet_selector = fastjet::SelectorEMin(val)*jet_selector ; 
      else if (tmp_sel[0] == "EMax")
        jet_selector = fastjet::SelectorEMax(val)*jet_selector ; 

      else if (tmp_sel[0] == "EtaMin")
        jet_selector = fastjet::SelectorEtaMin(val)*jet_selector ;
      else if (tmp_sel[0] == "EtaMax")
        jet_selector = fastjet::SelectorEtaMax(val)*jet_selector ;

      else if (tmp_sel[0] == "EtMin")
        jet_selector = fastjet::SelectorEtMin(val)*jet_selector ;
      else if (tmp_sel[0] == "EtMax")
        jet_selector = fastjet::SelectorEtMax(val)*jet_selector ;

      else if (tmp_sel[0] == "PtMin")
        jet_selector = fastjet::SelectorPtMin(val)*jet_selector ;
      else if (tmp_sel[0] == "PtMax")
        jet_selector = fastjet::SelectorPtMax(val)*jet_selector ;

      else if (tmp_sel[0] == "RapMin")
        jet_selector = fastjet::SelectorRapMin(val)*jet_selector ;
      else if (tmp_sel[0] == "RapMax")
        jet_selector = fastjet::SelectorRapMax(val)*jet_selector ;

      else if (tmp_sel[0] == "Circle")
        jet_selector = fastjet::SelectorCircle(val)*jet_selector ;

      else if (tmp_sel[0] == "Strip")
        jet_selector = fastjet::SelectorStrip(val)*jet_selector ; 
    }
  }
}

//--------------------------------------------------------------
// Sets jet definition options
void PHENO::GenJet::SetJetDef(std::vector<std::string> jet_def_set)
{
  // Start i from "1", since the first element is "Def".
  for (size_t i =1 ; i< jet_def_set.size(); ++i)
    {
      std::vector<std::string> tmp_def = Zaki::String::Pars(jet_def_set[i],"=");

      if (tmp_def[0] == "Algorithm")
        SetJetAlg(tmp_def[1]) ;

      if (tmp_def[0] == "R")
        jet_radius = std::stod(tmp_def[1]) ;
      
      if (tmp_def[0] == "Recomb_Scheme")
        SetRecScheme( tmp_def[1] ) ;

      if (tmp_def[0] == "Strategy")
        SetStrategy( tmp_def[1] ) ;

    }
}

//--------------------------------------------------------------
// Sets jet algorithm
void PHENO::GenJet::SetJetAlg(std::string jet_alg_str)
{

  if (jet_alg_str == "antikt_algorithm")
    jet_alg = fastjet::antikt_algorithm ;

  else if (jet_alg_str == "cambridge_algorithm")
    jet_alg = fastjet::cambridge_algorithm ;
  
  else if (jet_alg_str == "kt_algorithm")
    jet_alg = fastjet::kt_algorithm ;

  else if (jet_alg_str == "genkt_algorithm")
    jet_alg = fastjet::genkt_algorithm ;

  else if (jet_alg_str == "ee_kt_algorithm")
    jet_alg = fastjet::ee_kt_algorithm ;
  
  else if (jet_alg_str == "ee_genkt_algorithm")
    jet_alg = fastjet::ee_genkt_algorithm ;

  else
  {
    Z_LOG_WARNING("The algorithm is not found, please add it to PHENO::GenJet::setJetAlg options."); 
  }
  
}

//--------------------------------------------------------------
// Sets recombination scheme
void PHENO::GenJet::SetRecScheme(std::string rec_scheme)
{
  if (rec_scheme == "E_scheme")
    recomb_scheme = fastjet::E_scheme ;
  else if ( rec_scheme == "pt_scheme") 
    recomb_scheme = fastjet::pt_scheme ;
  else if ( rec_scheme == "pt2_scheme") 
    recomb_scheme = fastjet::pt2_scheme ;
  else if ( rec_scheme == "Et_scheme") 
    recomb_scheme = fastjet::Et_scheme ;
  else if ( rec_scheme == "Et2_scheme") 
    recomb_scheme = fastjet::Et2_scheme ;
  else if ( rec_scheme == "BIpt_scheme") 
    recomb_scheme = fastjet::BIpt_scheme ;
  else if ( rec_scheme == "BIpt2_scheme") 
    recomb_scheme = fastjet::BIpt2_scheme ;
  else
  {
    Z_LOG_WARNING("The recombination scheme is not found, add it to PHENO::GenJet::SetRecScheme options."); 
  }

}

//--------------------------------------------------------------
// Sets the Strategy
void PHENO::GenJet::SetStrategy(std::string strat)
{
  if (strat == "Best")
    strategy = fastjet::Best ;
  else if (strat == "N2Plain")
    strategy = fastjet::N2Plain ;
  else if (strat == "N2Tiled")
    strategy = fastjet::N2Tiled ;
  else if (strat == "N2MinHeapTiled")
    strategy = fastjet::N2MinHeapTiled ;
  else if (strat == "NlnN")
    strategy = fastjet::NlnN ;
  else if (strat == "NlnNCam")
    strategy = fastjet::NlnNCam ;
  else
  {
    std::cout<<"==> WARNING: The strategy is not found, please add it to PHENO::GenJet::setStrategy options.\n"<<std::flush; 
  }
}

//--------------------------------------------------------------
// Isolation condition assuming jets are faking taus
void PHENO::GenJet::FakeTauIsolate()
{
  PROFILE_FUNCTION();
  std::vector<int> remove_jet ;
  char isolate_message_char[200] ;

  for(size_t i=0 ; i < inc_jets_after.size() ; ++i )
  {
    double sum_ET = 0.0 ;

    fastjet::PseudoJet jet_i =  inc_jets_after[i] ;

    // ......................................................
    int i_charge = 0 ;
    for (size_t k = 0 ; k < jet_i.constituents().size() ; ++k) 
      i_charge += jet_i.constituents()[k].user_index();  

    inc_jets_after[i].set_user_index(i_charge) ;
    // ......................................................

    for(size_t j=0 ; j < ev_ref.size() ; ++j )
    {
      ExParticle prt_j =  ev_ref.FullEvent()[j] ;
      /* 
        Checking if part_j is final state
      */
      if( prt_j.isVisible() && prt_j.isFinal() && prt_j.GetMom().eT() > 0.001 )
      {   
        // Checking if they are within 0.1 < R < 0.3 cone
        if(  0.1 < Distance(jet_i, prt_j) && Distance(jet_i, prt_j) < 0.3 )
        { sum_ET += prt_j.GetMom().eT() ;  }
      }
    }

    if(sum_ET  > 2)
    {
      Zaki::Vector::Add(remove_jet, (int) i) ;
      
      if (report_jet_flag) 
      {
        sprintf(isolate_message_char,
          "\n - Jet with (phi, prap)= (%4.2f, %4.2f) failed the tau_h isolation condition.\n",
          jet_i.phi(), jet_i.pseudorapidity()) ;

        std::string somestring(isolate_message_char) ;
        isolate_message += somestring ;
      }
    }
  }
  
  Zaki::Vector::Remove(inc_jets_after, remove_jet) ;

}

//--------------------------------------------------------------
void PHENO::GenJet::Isolate()
{
  PROFILE_FUNCTION();
  std::vector<int> remove_jet ;
  double  d ;
  char isolate_message_char[200] ;

  for (size_t i = 0 ; i < ev_ref.PassedLeptons().size() ; ++i)
  {
    ExParticle p = ev_ref.PassedLeptons()[i] ;

    for (size_t j = 0 ; j < inc_jets_after.size() ; ++j)
    {
      d = Distance(inc_jets_after[j], p) ;
      if(  d < 0.3 )

      {
        if (report_jet_flag) 
        {
        sprintf(isolate_message_char,
         "\n - Isolated %s with (phi, prap)=(%4.2f, %4.2f)\
          has R= %-4.2f from the jet with (%4.2f, %4.2f) and\
           it's removed (since R < 0.3).\n", p.name().c_str(),
            p.GetVisMom().phi(), p.GetVisMom().eta(), d, inc_jets_after[j].phi(),
             inc_jets_after[j].pseudorapidity()) ;

        std::string somestring(isolate_message_char) ;
        isolate_message += somestring ;
        }
        remove_jet.push_back(j) ;
      }

    }
  }

  Zaki::Vector::Remove(inc_jets_after, remove_jet) ;

}

//--------------------------------------------------------------
// Returns the distance between the pseudo jet and a particle
double PHENO::GenJet::Distance(fastjet::PseudoJet& psjet, ExParticle& p) 
{
  return pow((pow((psjet.pseudorapidity() - p.GetVisMom().eta() ), 2)
  + pow( ( psjet.phi() - p.GetVisMom().phi() ), 2) ), 0.5) ;
}

//--------------------------------------------------------------
/*
  Reports the details of the jet production into a text file.
*/
void PHENO::GenJet::Report()
{
  std::FILE * jet_rep_file  ;

  jet_rep_file = fopen(jet_rep_char, "a") ;

  /*
    "description"():
     -the description of the algorithm used

    "print_jettable":
     -shows the output as {index, prap, rap, phi, pt, # of Constituents}
  */

  char Jet_Def_Description[100] ;
  sprintf(Jet_Def_Description, "%s", my_jet_dscr.c_str()) ;

  // Before Selection:
  PrintJetTable(jet_rep_file, inc_jets_before, Jet_Def_Description,
   "Before Selection", ev_ref.i() ) ;

  // After Selection:
  PrintJetTable(jet_rep_file, inc_jets_after, Jet_Def_Description,
   "After Selection", ev_ref.i() ) ;

  // Writing the isolate method reports
  fprintf(jet_rep_file,"%s", isolate_message.c_str()) ; 

  fclose(jet_rep_file) ;

}


//--------------------------------------------------------------
/*
  Printing the jet report
*/
template <class T> void PHENO::GenJet::PrintJetTable(std::FILE * file,
 std::vector<T> list, char* File_Description,  const char* Table_Title, int iEv)
{

  using namespace Zaki::String ;

  thread_local static int Table_Num   = 0  ;
  thread_local static int Same_iEvent = -1 ;

  if(list.size() >0)
  {
    // Title of the file
    if(Table_Num==0){fprintf(file," %s FastJet Report %s\n",
     Multiply('=', 28).c_str(), Multiply('=', 36).c_str() ) ;

    fprintf(file,"\n FastJet Ran %s. \n", File_Description) ;}

    if(Same_iEvent != iEv){  fprintf(file,"\n %s Event %5d begins %s\n",
     Multiply('-', 29).c_str() , iEv, Multiply('-', 29).c_str() ) ;
                                Same_iEvent = iEv ;  }

    std::string Title_Dash((int) strlen(Table_Title)+8 ,'_') ;
    fprintf(file,"  %s\n |  %*s  |",Title_Dash.c_str(),
     (int) strlen(Table_Title)+4  , Table_Title) ;

    // label the columns
    fprintf(file,"\n |%-40s \n | %5s %10s %7s %8s %11s %17s %7s |\n",
    Multiply('=', 73).c_str() ,"Jet #", "P.Rap.", "Rap.", "Phi", "p_T", "Constituents", "Charge") ;
    fprintf(file, " | %s |\n", Multiply('-', 71).c_str() ) ;

    // print out the details for each jet
    for (unsigned int i = 0 ; i < list.size() ; ++i) {
      
      // get the constituents of the jet
      std::vector<fastjet::PseudoJet> constituents = list[i].constituents() ;

      // The total charge of each jet
      float charge = ( list[i].user_index() / 3. )  ;
      
      fprintf(file," | %5u %8.2f %9.2f %8.2f %13.2f %13u %7.2f %3s\n",i+1,
       list[i].pseudorapidity(), list[i].rap(), list[i].phi(),
       list[i].pt(),(unsigned int) constituents.size(), charge,"|") ;
    }

      fprintf(file,"  %s\n", Multiply('=', 73).c_str() ) ;
      Table_Num++ ;
      
  } else   if(iEv == Same_iEvent) {fprintf(file,
              "\n  => None of the jets pass the Selection.\n") ;}
}

//--------------------------------------------------------------
// Returns the inclusive jets after cuts
std::vector<fastjet::PseudoJet> PHENO::GenJet::InclusiveJetsAfter()
{
  return inc_jets_after;
}

//==============================================================
