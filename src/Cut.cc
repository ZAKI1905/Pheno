/*
  Cut class

  Last Updated by Zaki on July 6, 2019

*/

// "algorithm" for "remove" and "remove_if"
#include <algorithm>
#include <iostream>

// #include <zaki/Vector/Vector_Basic.h>

#include "../include/Cut.h"

//==============================================================

//--------------------------------------------------------------
// Constructor
// Default Constructor
PHENO::CUTS::Cut::Cut() { }

// Constructor takes a reference to an event.
PHENO::CUTS::Cut::Cut(PHENO::ExEvent* ev) : evPtr(ev) {
  // // Adding the pointer to this cut to the event
  //   ev.addCutPtr(this);
}

//--------------------------------------------------------------
std::unique_ptr<PHENO::CUTS::Cut> PHENO::CUTS::Cut::Clone() const 
{
  return std::unique_ptr<Cut>(IClone());
}

//--------------------------------------------------------------
// Setting pointer to an event
void PHENO::CUTS::Cut::SetEventPtr(PHENO::ExEvent* in_ev) 
{
  evPtr = in_ev ;
}

//--------------------------------------------------------------
// Setting the cut label
void PHENO::CUTS::Cut::SetName(const char* in_name) 
{
  name = in_name ;
}

//--------------------------------------------------------------
// Setting the cut label
std::string PHENO::CUTS::Cut::GetName() const 
{
  return name;
}

//--------------------------------------------------------------
// Getting pointer to an event
PHENO::ExEvent* PHENO::CUTS::Cut::GetEventPtr() const
{
  return evPtr;
}

//--------------------------------------------------------------
// Virtual destructor
PHENO::CUTS::Cut::~Cut() { }

//--------------------------------------------------------------
/*
  Reports the details of the cut into a text file.
*/
void PHENO::CUTS::Cut::Report() const
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
    if( same_iEv != evPtr->i() )
    {
      same_iEv = evPtr->i() ;
      fprintf(pCUT_REPORT_FILE,"\n %s Event %5d %s\n", Zaki::String::Multiply('*', 50).c_str(),
       same_iEv, Zaki::String::Multiply('*', 50).c_str()) ;

    }

  // --------------------------------------
  // List of particles prior to the Cut
  // --------------------------------------
    fprintf(pCUT_REPORT_FILE,
    "\n %s\n| %19s  %-74s %-20s |\n| %19s  %-74s %-20s |\n|%s|\n",
    Zaki::String::Multiply('_', 118).c_str()," "," "," ","*",report_title.c_str(),
    "*",Zaki::String::Multiply('_', 118).c_str()) ;

    if(in_parts_cpy.size() > 0){fprintf(pCUT_REPORT_FILE,
    "  %s\n |  Particles selected for this cut   |",
    Zaki::String::Multiply('_', 36).c_str());}

    else{no_part_selected = true; fprintf(pCUT_REPORT_FILE,
    "\n No particle was selected for this cut.");}

    evPtr->PrintTable<ExParticle>(pCUT_REPORT_FILE, in_parts_cpy) ;

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
      "  %s\n |  Particles passing this cut   |", Zaki::String::Multiply('_', 31).c_str()) ;
      evPtr->PrintTable<ExParticle>(pCUT_REPORT_FILE, out_parts_cpy) ;
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
void PHENO::CUTS::Cut::Input(std::string property)
{
  // Parsing the command
  std::vector<std::string> inp = Zaki::String::Pars(property, ":") ;

  if(inp[0] == "Report_Cut")
  {
    report_title = Zaki::String::Pars(inp[1], ":" )[0]  ;
    sprintf(cut_report_char, "%s", (Zaki::String::Pars(inp[1], ":" )[1]).c_str() ) ;
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
void PHENO::CUTS::Cut::Apply(ParticleLST& in_parlst)
{

  // Copying in_parlst for report()
    in_parts_cpy = in_parlst ;

  // Adds those not passing cuts to rm_list
    CutCond(in_parlst) ;

  // Removes the elements in in_parlst based on rm_list
    Zaki::Vector::Remove(in_parlst, rm_list) ;

  // Adding the pointer to this cut to the event
    // evPtr->addCutPtr(this) ;

  // Copying in_parlst for report()
    out_parts_cpy = in_parlst ;

  // Reports cut
    if(report_cut) Report() ;

}
//--------------------------------------------------------------
//==============================================================
