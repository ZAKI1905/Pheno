/* 
  GenJet class

  Last Updated by Zaki on July 6, 2019

*/ 

#include <iostream>
#include <vector>
#include "Pythia8/Pythia.h"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/Selector.hh"
#include "../include/Basics.h"
#include "../include/EV.h"
#include "../include/GenJet.h"

using std::vector ;

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
GenJet::GenJet(EV& ev):ev_ref(ev){ ev.addGenJetPtr(this) ; }

//--------------------------------------------------------------
// Takes a pointer to pseudo jets in the event

void GenJet::gen()
{

  //-----1-Jet Definition-----
  // Defining the jet
   fastjet::JetDefinition jet_def(jet_alg, jet_radius) ;
   
   // Jets description
   my_jet_dscr = jet_def.description() ; 
  //--------------------------

  //-----2-Ps-Jet-Update------
    ev_ref.update_ps_jets() ;
  //--------------------------

  //-----3-Cluster Sequence---
  // runs the jet clustering with the jet definitions
    fastjet::ClusterSequence clust_seq(ev_ref.psjet(), jet_def) ;
  //--------------------------

  //-----4-Cuts---------------
  // Applies selection cuts to jets
    fastjet::Selector jet_selector = fastjet::SelectorPtMin(SelPtMin) *
     fastjet::SelectorAbsEtaMax(SelEtaMax) ;
  //--------------------------

  //-----5-Selector-----------
  // get the resulting jets ordered in pt
    double ptmin = 5.0 ;
    inc_jets_before = fastjet::sorted_by_pt(clust_seq.inclusive_jets(ptmin)) ;

    inc_jets_after  = 
    fastjet::sorted_by_pt(jet_selector(clust_seq.inclusive_jets(ptmin))) ;
  //--------------------------

  //-----6-Isolation-Cut------
  //removing the non-isolated jets from inc_jets_after
    isolate() ;
  //--------------------------

  //-----7-Reporting Jets-----
    if( report_jet == true ) { report() ; }
  //--------------------------

}

//--------------------------------------------------------------
// input method that takes properties with a string and double option
void GenJet::input(string property, string str_val, double num_val)
{

  //   Taking jet options:

  //-----1-Algorithm-----
  if(property=="algorithm")
  //
  // This could be one of
  //   {kt_algorithm, cambridge_algorithm, antikt_algorithm,
  //    genkt_algorithm, ee_kt_algorithm, ee_genkt_algorithm}
  {
    if(str_val=="antikt_algorithm")
    {
      // a jet algorithm with a given radius parameter (num_val)
      // Warning(May-12-2019): We may need to initialize jet_def at once like below:
      // fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, num_val);
      jet_alg = fastjet::kt_algorithm ;
      jet_radius = num_val ;
    }
  }
  //--------------------------

  //-----2-Selector-----
   if(property=="Selector")
  {
    // Sets the cut on p_T
    if(str_val=="PtMin")
     { SelPtMin =  num_val ; } 

    // Sets the cut on AbsEtaMax
    if(str_val=="EtaMax")
     { SelEtaMax =  num_val ; } 

  }
  //--------------------------

  //-----3-Report-----
   if(property=="Report_Jets") {
      report_jet = true ; File_LHE =str_val ;  pr_id= (int)num_val ; }
  //--------------------------

}

//--------------------------------------------------------------
void GenJet::isolate()
{
  vector<int> remove_jet ;
  double  d ;
  char isolate_message_char[200] ;

  for (size_t i = 0 ; i < ev_ref.pass_lepts().size() ; ++i)
  {
    ExParticle p = ev_ref.pass_lepts()[i] ;

    for (size_t j = 0 ; j < inc_jets_after.size() ; ++j)
    {
      d = distance(inc_jets_after[j], p) ;
      if(  d < 0.3 )

      {
        if (report_jet) 
        {
        sprintf(isolate_message_char,
         "\n - Isolated %s with (phi, prap)=(%4.2f, %4.2f)\
          has R= %-4.2f from the jet with (%4.2f, %4.2f) and\
           it's removed (since R < 0.3).\n", p.name().c_str(),
            p.visMom().phi(), p.visMom().eta(), d, inc_jets_after[j].phi(),
             inc_jets_after[j].pseudorapidity()) ;

        string somestring(isolate_message_char) ;
        isolate_message += somestring ;
        }
        remove_jet.push_back(j) ;
      }

    }
  }

  rm_elem(inc_jets_after, remove_jet) ;

}

//--------------------------------------------------------------
// Returns the distance between the pseudo jet and a particle
double GenJet::distance(fastjet::PseudoJet& psjet, ExParticle& p) 
{
  return pow((pow((psjet.pseudorapidity() - p.visMom().eta() ), 2)
  + pow( ( psjet.phi() - p.visMom().phi() ), 2) ), 0.5) ;
}

//--------------------------------------------------------------
/*
  Reports the details of the jet production into a text file.
*/
void GenJet::report()
{
  std::FILE * JET_REPORT_FILE  ;
  char JET_REPORT_CHAR[50] ;
  sprintf(JET_REPORT_CHAR, "Jet_Report_%s_%d.txt", File_LHE.c_str(), pr_id);
  JET_REPORT_FILE = fopen(JET_REPORT_CHAR, "a") ;

  /*
    "description"():
     -the description of the algorithm used

    "print_jettable":
     -shows the output as {index, prap, rap, phi, pt, # of Constituents}
  */

  char Jet_Def_Description[100] ;
  sprintf(Jet_Def_Description, "%s", my_jet_dscr.c_str()) ;

  // Before Selection:
  print_jettable(JET_REPORT_FILE, inc_jets_before, Jet_Def_Description,
   "Before Selection", ev_ref.i() ) ;

  // After Selection:
  print_jettable(JET_REPORT_FILE, inc_jets_after, Jet_Def_Description,
   "After Selection", ev_ref.i() ) ;

  // Writing the isolate method reports
  fprintf(JET_REPORT_FILE,"%s", isolate_message.c_str()) ; 

  fclose(JET_REPORT_FILE) ;

}


//--------------------------------------------------------------
/*
  Function responisble for printing the jet report
*/
template <class T> void GenJet::print_jettable(std::FILE * file,
 vector<T> list, char* File_Description,  const char* Table_Title, int iEv)
{

  thread_local static int Table_Num   = 0  ;
  thread_local static int Same_iEvent = -1 ;

  if(list.size() >0)
  {
    // Title of the file
    if(Table_Num==0){fprintf(file," %s FastJet Report %s\n",
     pr(28,'=').c_str(), pr(28,'=').c_str() ) ;

    fprintf(file,"\n FastJet Ran %s. \n", File_Description) ;}

    if(Same_iEvent != iEv){  fprintf(file,"\n %s Event %5d begins %s\n",
     pr(25,'-').c_str() , iEv, pr(25,'-').c_str() ) ;
                                Same_iEvent = iEv ;  }

    string Title_Dash((int) strlen(Table_Title)+8 ,'_') ;
    fprintf(file,"  %s\n |  %*s  |",Title_Dash.c_str(),
     (int) strlen(Table_Title)+4  , Table_Title) ;

    // label the columns
    fprintf(file,"\n |%-40s \n | %5s %10s %7s %8s %11s %17s |\n",
    pr(65,'=').c_str() ,"Jet #", "P.Rap.", "Rap.", "Phi", "p_T", "Constituents") ;
    fprintf(file, " | %s |\n", pr(63,'-').c_str() ) ;

    // print out the details for each jet
    for (unsigned int i = 0 ; i < list.size() ; ++i) {

      // get the constituents of the jet
      vector<fastjet::PseudoJet> constituents = list[i].constituents() ;
      fprintf(file," | %5u %8.2f %9.2f %9.2f %12.2f %10u %6s\n",i+1,
       list[i].pseudorapidity(), list[i].rap(), list[i].phi(),
       list[i].perp(),(unsigned int) constituents.size(),"|") ;
    }

      fprintf(file,"  %s\n", pr(65,'=').c_str() ) ;
      Table_Num++ ;
      
  } else   if(iEv == Same_iEvent) {fprintf(file,
              "\n  => None of the jets pass the Selection.\n") ;}
}

//==============================================================
