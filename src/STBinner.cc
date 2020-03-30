/*
  STBinner class

  Last Updated by Zaki on Sep 3, 2019

*/

#include "../include/STBinner.h"
#include "../include/GenJet.h"

//==============================================================

// Constructor
STBinner::STBinner() {}
//--------------------------------------------------------------
// Adding the event
void STBinner::Input(ExEvent& evIn)
{
  // Saving the event reference
  evp = &evIn ;
 
  // The leptons that passed the cuts (by definition charged)
   charged_lept = evp->PassedLeptons() ;
  
  // Setting unit event counter from the event weight
   one_event  = evp->Weight() ;

  // Saving the inclusive jets after the cuts
   jet_set = evp->GenJetPtr->InclusiveJetsAfter() ;

}

//--------------------------------------------------------------
// // Should be used after the genjet method
// void STBinner::input(vector<fastjet::PseudoJet> in_jet_set)
// {
//   jet_set = in_jet_set ;
// }

//--------------------------------------------------------------
// Overloading the input method.
void STBinner::Input(std::string input)
{
  // Parsing the command
  std::vector<std::string> inp = pars(input, "=") ;

  if(inp[0] == "ST_Bins") { stolst( st_limit , inp[1], "-" )  ;}
}

//--------------------------------------------------------------
// Virtual method from binner class:
void STBinner::BinIt(bool cut_report, std::string cut_f_name)
{

  // Finds the S_T of the event
  double st_val = S_T()  ;

  // Sets the "on_z" variable (default = 0)
  OnOffZ() ;

  // Finds which lepton channel the event belongs to
  std::vector<int> lep_n = LepNumCheck() ;

  /*
    Sorts events according to their S_T value,
    also whether they are on-Z or off-Z, 
    and their lepton channel.

  */

// ------------------------------------------
    // Assume it's in m_ll < 75 bin
    int             x = 0 ;
    std::string on_z_char  = "off-Z" ;
    // If it's on_z:
    if( on_z == 1 ) { x = 1 ; on_z_char = "On-Z" ; }
    // In the special case of channel 8 & 9, where we separate m_{ll} > 105 bin:
    else if( lep_n[0]==8 || lep_n[0]==9 )
    {
      on_z_char = " < 75 GeV" ;
      if( on_z==2 )  { x = 2  ; on_z_char = " > 105 GeV" ; }
    } 
// ------------------------------------------

  // Loop over the st_limits
  for ( size_t i=0 ; i < st_limit.size() -1 ; ++i )
  {
    // Avoiding memory error
    if (lep_n[0] < 1 || lep_n[0] > 9)
      break ;
    
    // drop the event in the corresponding bin
    if ( st_limit[ i ] <= st_val && st_val < st_limit[ i+1 ] )
    {
      bin_set[ lep_n[0]-1 ][ x ][ i ] += one_event ;
      break ;
    }

    // if S_T is greater than all the limits, put it in the last bin
    if( i == st_limit.size() -2 )
    bin_set[ lep_n[0]-1 ][ x ][ i+1 ] += one_event ;
  }

  if ( cut_report )
  {
  // Add info to the cut report file
    std::FILE * pCUT_REPORT_FILE ;
    pCUT_REPORT_FILE = fopen(cut_f_name.c_str(), "a") ;

    fprintf(pCUT_REPORT_FILE,
    "\n\n ---------------------- SUMMARY (Event ") ;

    fprintf(pCUT_REPORT_FILE,
    "%5d ) -----------------------\n|%70s\n", evp->i() ,"|") ;

    fprintf(pCUT_REPORT_FILE,
    "|   Bin: N_l = %d, N_{tau_h} = %d, N_{OSSF} = %d , m_{ll}: %-9s %-4s|\n|%70s\n",
    lep_n[1], lep_n[2], lep_n[3], on_z_char.c_str()," ","|") ;

    fprintf(pCUT_REPORT_FILE,
    "|    %-5s S_T = %-9.1f GeV,  Event Weight = %-4.2f%-18s|\n|%70s\n","==>",
     st_val,evp->Weight(),".","|") ;

    fprintf(pCUT_REPORT_FILE," %s\n", pr(69,'-').c_str() ) ;
    fclose(pCUT_REPORT_FILE) ;
  }

}

//--------------------------------------------------------------
double STBinner::S_T()
{/*
 Calculates the value of S_T for an event.
 */
    double px_sum        = 0 ;
    double py_sum        = 0 ;
    double scalar_sum_pT = 0 ;
    double missing_pT    = 0 ;

    // Adding the charged leptons' share to S_T
    for( size_t i=0 ; i < charged_lept.size() ; ++i )
    {
        px_sum         += charged_lept[i].GetVisMom().px() ;
        py_sum         += charged_lept[i].GetVisMom().py() ;
        scalar_sum_pT  += charged_lept[i].GetVisMom().pT() ;
    }

    // Adding the jets' share to S_T
    for ( size_t i = 0 ; i < jet_set.size() ; ++i)
    {
        px_sum         += jet_set[i].px()   ;
        py_sum         += jet_set[i].py()   ;
        scalar_sum_pT  += jet_set[i].perp() ;
    }

    missing_pT          = pow(pow(px_sum,2) + pow(py_sum,2),0.5) ;

    return (missing_pT  + scalar_sum_pT) ;
}

//--------------------------------------------------------------
void STBinner::OnOffZ()
{
  /*
  ---------------------------------- On-Z or Off-Z ----------------------------
   Checks if an event is on-Z or off-Z. An event is on-Z if there is any pair of
   OSSF charged (light) leptons with: 75 GeV < m_{l+l-} < 105 GeV.

   -returns 0 for:            m_{l+l-} < 75 GeV
   -returns 1 for:  75 GeV  < m_{l+l-} < 105 GeV
   -returns 2 for:  105 GeV < m_{l+l-}

  */

  // by default assumes the first bin
    on_z = 0 ;

  for(size_t i=0 ; i < charged_lept.size() ; ++i )
  {
    // Only light leptons (e and mu)
    if( charged_lept[i].idAbs() != ID_TAU ) 

    for(size_t j=0 ; j < charged_lept.size() ; ++j )
    {
      // OSSF condition
      if( charged_lept[i].id() == -charged_lept[j].id() )
      {
        ParticleLST tmp_lst = {charged_lept[i], charged_lept[j]} ;
        // if it's in the second bin
        if( 75 < invM(tmp_lst) 
           && invM(tmp_lst) < 105)
          on_z = 1 ;

        // if it's in the third bin
        else if(105 < invM( tmp_lst) )
          on_z = 2 ;
      }
    }
  }

}

//--------------------------------------------------------------
std::vector<int> STBinner::LepNumCheck()
/*           Lepton Number Cut Conditions
Checking the number of charged leptons in an event:

*/
{
  int         hadr_taus = (int) evp->HadronicTaus().size() ;

  std::vector<int> output    = {0,0,0,0} ;

  //--------------------------------------------------------------------------

  output[1] = hadr_taus
            + count(charged_lept, {ID_ELECTRON, -ID_ELECTRON, ID_MUON, -ID_MUON}) ;

  output[2] = hadr_taus ;

  output[3] = OSSF(charged_lept) ;

  // Making a 3 digit number based on the above numbers
  int var = output[1]*100 + output[2]*10 + output[3]*1 ;

  //--------------------------------------------------------------------------

  switch (var) 
    { 
        // N_l = 3,   N_{tau_h} = 0,    N_{OSSF} = 0
        case 300: output[0] = 6 ;
                  break ; 

        // N_l = 4,   N_{tau_h} = 0,    N_{OSSF} = 0
        case 400: output[0] = 1 ; 
                  break ; 

        // N_l = 4,   N_{tau_h} = 0,    N_{OSSF} = 2
        case 402: output[0] = 3 ; 
                  break ;

        // N_l = 4,   N_{tau_h} = 0,    N_{OSSF} = 1
        case 401: output[0] = 2 ;
                  break ; 

        // N_l = 4,   N_{tau_h} = 1,    N_{OSSF} = 0
        case 410: output[0] = 4 ;  
                  break ; 

        // N_l = 4,   N_{tau_h} = 1,    N_{OSSF} = 1
        case 411: output[0] = 5 ; 
                  break ; 

        // N_l = 3,   N_{tau_h} = 1,    N_{OSSF} = 0
        case 310: output[0] = 7 ;
                  break ; 

        // N_l = 3,   N_{tau_h} = 1,    N_{OSSF} = 1
        case 311: output[0] = 8 ; 
                  break ; 

        // N_l = 3,   N_{tau_h} = 0,    N_{OSSF} = 1
        case 301: output[0] = 9 ; 
                  break ; 

        default:  break ;   
    } 

  return output ;
}

//--------------------------------------------------------------
// Internal report for python, and an external report for the user
// Should be implemented in a "bin" calss maybe.
void STBinner::Report(std::string File_LHE) const
{
    std::FILE * INT_REPORT ;
    char INT_Report_Char[100] ;
    sprintf(INT_Report_Char, "_report_%s", File_LHE.c_str()) ;
    INT_REPORT= fopen(INT_Report_Char, "w") ;

    // Writing the report text file:
    std::FILE * FINAL_REPORT ;
    char Final_Report_Char[100] ;
    sprintf(Final_Report_Char, "Report_%s.txt", File_LHE.c_str()) ;
    FINAL_REPORT= fopen(Final_Report_Char, "w") ;

    std::string content( "\n  Final Cut Report : \n\n" ) ; // Title
    char tmp[200] ;

    for (size_t j = 1 ; j < 10 ; ++j)
    {
      // NOTE: Dependent on the definition of bins!
      int N_l        =  4 - j/6  ;
      int N_tau_h    = 0         ;  if(j==4 || j==5 || j==7 || j==8) N_tau_h = 1 ;
      int N_OSSF     = 1         ;  if(j==1 || j==4 || j==6 || j==7) N_OSSF = 0  ; if(j==3) N_OSSF = 2 ;
      int dash_count = 0         ;

      // label the columns
      sprintf(tmp,"  %s\n |  N_l = %d  -  tau_h = %d -  OSSF = %d  |",pr(37,'_').c_str(),
               N_l,N_tau_h, N_OSSF) ;
      content += tmp ;

      if(j!=8 && j!=9)
      {

        sprintf(tmp,"\n |%-s|\n | %9s |%9s   |%9s   |\n", pr(37,'=').c_str(),"S_T bin #","off-Z", "on-Z") ;
        content += tmp ;
        sprintf(tmp," | %s |\n",pr(35+dash_count,'-').c_str()) ;
        content += tmp ;
        for (size_t i = 0 ; i < 5 ; ++i)
        {
          fprintf(INT_REPORT," %2u , %9.2f , %9.2f , %9.2f  \n",(int)j, bin_set[j-1][0][i], bin_set[j-1][1][i],  bin_set[j-1][2][i]) ;

          sprintf(tmp," | %6u    |%9.2f   |%9.2f   |\n",(int)i+1, bin_set[j-1][0][i], bin_set[j-1][1][i]) ;
          content += tmp ;
        }
        sprintf(tmp,"  %s \n\n",pr(37,'-').c_str()) ;
        content += tmp ;

      }
      else
      {
        dash_count = 13 ;

        sprintf(tmp,"\n |%-s\n | %9s |  %9s |%9s   | %9s |\n", pr(50,'=').c_str(),"S_T bin #","m_ll < 75", "on-Z", "m_ll > 105") ;
        content += tmp ;
        sprintf(tmp," | %s |\n",pr(35+dash_count,'-').c_str()) ;
        content += tmp ;

        for (size_t i = 0 ; i < 5 ; ++i)
        {

         fprintf(INT_REPORT," %2u , %9.2f , %9.2f , %9.2f  \n",(int)j, bin_set[j-1][0][i], bin_set[j-1][1][i], bin_set[j-1][2][i]) ;

          sprintf(tmp," | %6u    |%9.2f   |%9.2f   |%9.2f   |\n",(int)i+1, bin_set[j-1][0][i], bin_set[j-1][1][i], bin_set[j-1][2][i]) ;
          content += tmp ;
        }
        sprintf(tmp,"  %s \n\n",pr(50,'-').c_str()) ;
        content += tmp ;

      }

      // fprintf(FINAL_REPORT," | %s |\n",pr(35+dash_count,'-').c_str()) ;


      // print out the details for each bin
      // if(j!=8 && j!=9)
      // {
      //   for (size_t i = 0 ; i < 5 ; ++i) {
      //     fprintf(FINAL_REPORT," | %6u    |%9.2f   |%9.2f   |\n",(int)i+1, bin_set[j-1][0][i], bin_set[j-1][1][i]) ;
      //     fprintf(INT_REPORT," %2u , %9.2f , %9.2f , %9.2f  \n",(int)j, bin_set[j-1][0][i], bin_set[j-1][1][i],  bin_set[j-1][2][i]) ;
      //   }
      //   fprintf(FINAL_REPORT,"  %s \n\n",pr(37,'-').c_str()) ;
      // }
      // else
      // {
      //   for (size_t i = 0 ; i < 5 ; ++i) {
      //     fprintf(FINAL_REPORT," | %6u    |%9.2f   |%9.2f   |%9.2f   |\n",(int)i+1, bin_set[j-1][0][i], bin_set[j-1][1][i], bin_set[j-1][2][i]) ;
      //     fprintf(INT_REPORT," %2u , %9.2f , %9.2f , %9.2f  \n",(int)j, bin_set[j-1][0][i], bin_set[j-1][1][i], bin_set[j-1][2][i]) ;
      //   }
      //   fprintf(FINAL_REPORT,"  %s \n\n",pr(50,'-').c_str()) ;
      // }
    }
    fprintf(FINAL_REPORT, "%s", content.c_str()) ;

    fclose(INT_REPORT) ;
    fclose(FINAL_REPORT) ;
}

//--------------------------------------------------------------------------

//==============================================================