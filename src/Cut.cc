/*
  Cut class

  Last Updated by Zaki on July 6, 2019

*/

// "algorithm" for "remove" and "remove_if"
#include <algorithm>
#include <iostream>

#include "../include/Cut.h"

//==============================================================

//--------------------------------------------------------------
// Constructor
// Constructor takes a reference to an event.
Cut::Cut(ExEvent& ev) : ev_ref(ev) {
  // // Adding the pointer to this cut to the event
  //   ev.addCutPtr(this);
   }

//--------------------------------------------------------------
// Virtual destructor
Cut::~Cut() { }

//--------------------------------------------------------------
/*
  Reports the details of the cut into a text file.
*/
void Cut::Report() const
{

  bool no_part_selected = false ;
  thread_local static int same_iEv = -1 ;

  // --------------------------------------
  // Opening the report file
  // --------------------------------------
    std::FILE * pCUT_REPORT_FILE;
    pCUT_REPORT_FILE = fopen(cut_report_char, "a") ;
  // --------------------------------------

  // --------------------------------------
  // Adding the event number header
  // --------------------------------------
    if( same_iEv != ev_ref.i() )
    {
      same_iEv = ev_ref.i() ;
      fprintf(pCUT_REPORT_FILE,"\n %s Event %5d %s\n", pr(50,'*').c_str(),
       same_iEv, pr(50,'*').c_str()) ;

    }


  // --------------------------------------
  // List of particles prior to the Cut
  // --------------------------------------
    fprintf(pCUT_REPORT_FILE,
    "\n %s\n| %19s  %-74s %-20s |\n| %19s  %-74s %-20s |\n|%s|\n",
    pr(118,'_').c_str()," "," "," ","*",report_title.c_str(),
    "*",pr(118,'_').c_str()) ;

    if(in_parts_cpy.size() > 0){fprintf(pCUT_REPORT_FILE,
    "  %s\n |  Particles selected for this cut   |",
    pr(36,'_').c_str());}

    else{no_part_selected = true; fprintf(pCUT_REPORT_FILE,
    "\n No particle was selected for this cut.");}

    ev_ref.PrintTable<ExParticle>(pCUT_REPORT_FILE, in_parts_cpy) ;

  // --------------------------------------
  // Special message from the cut
  // --------------------------------------

    fprintf(pCUT_REPORT_FILE, "\n%s\n",special_message.c_str()) ;

  // --------------------------------------
  // List of particles after the Cut
  // --------------------------------------
  if ( ! no_part_selected )
  {
  // If some (not all) passed the cut
    if( out_parts_cpy.size() > 0 && in_parts_cpy.size() != out_parts_cpy.size() )
     {
      fprintf(pCUT_REPORT_FILE,
      "  %s\n |  Particles passing this cut   |",pr(31,'_').c_str()) ;
      ev_ref.PrintTable<ExParticle>(pCUT_REPORT_FILE, out_parts_cpy) ;
     }
  // If none passed
    else if( out_parts_cpy.size() == 0 )
      fprintf(pCUT_REPORT_FILE,"\n No particle passed this cut.") ;
  // If all passed
    else
      fprintf(pCUT_REPORT_FILE,"\n All of the particles passed this cut.") ;
  }

  // --------------------------------------
  // Closing the report file
  // --------------------------------------
    fclose(pCUT_REPORT_FILE) ;

}

//--------------------------------------------------------------
void Cut::Input(std::string property)
{
  // Parsing the command
  std::vector<std::string> inp = pars(property, ":") ;

  if(inp[0] == "Report_Cut")
  {
    report_title = pars(inp[1], ":" )[0]  ;
    sprintf(cut_report_char, "%s", (pars(inp[1], ":" )[1]).c_str() ) ;
    report_cut   = true ;
    return ;
  }

  if(inp[0] == "Special_Message")
  { special_message = inp[1] ; return ; }

  if(inp[0] == "Name")
  { name = inp[1] ; return ; }

}

//--------------------------------------------------------------
// Applies the cut on in_parlst
void Cut::Apply(std::vector<ExParticle>& in_parlst)
{

  // Copying in_parlst for report()
    in_parts_cpy = in_parlst ;

  // Adds those not passing cuts to rm_list
    CutCond(in_parlst) ;

  // Removes the elements in in_parlst based on rm_list
    rm_elem(in_parlst, rm_list) ;

  // Adding the pointer to this cut to the event
    // ev_ref.addCutPtr(this) ;

  // Copying in_parlst for report()
    out_parts_cpy = in_parlst ;

  // Reports cut
    if(report_cut) Report() ;

}
//--------------------------------------------------------------
//==============================================================
