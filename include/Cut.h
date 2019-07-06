#ifndef Cut_H
#define  Cut_H

#include <vector>
#include "Basics.h"
#include "EV.h"
#include "ExParticle.h"

using std::vector ;

//=============================================================
/*
  Cut class takes an event, and a removal list and outputs
  another removal list, reports the Cut if asked by the user.
*/
class Cut
{
  // Gives access to input, etc.
  friend class Pheno ;   

  //--------------------------------------------------------------
  
  public:

    // Constructor takes a reference to an event
    Cut(EV &) ;

    // Virtual destructor
    virtual ~Cut() ;

    //-------- Applying Cut ------------
    // Usual definition for when we want to modify the 
    // input particle list
    void apply(vector<ExParticle>&) ; 
    //----------------------------------

  //--------------------------------------------------------------

  protected:

    // Cut label
    string name = "unnamed_cut" ;

    // Ref. to the event
    EV& ev_ref ;  

    // cut_cond checks cut and adds those not passing it to rm_list 
    virtual void cut_cond(vector<ExParticle>&) = 0 ;   

    // Saves cut details in a .txt file
    void report() ;    

    // The removal list
    vector<int> rm_list ;  

    // Report cut or not
    bool  report_cut = false ;  

    // The title used to identify various cuts
    std::string report_title = "cut" ;  

    // Cut's intermediate message in the reportfile 
    std::string special_message = "" ;  

    // Cut report file name
    char  cut_report_char[100] ;  
    
    // Copy of input particles for reporting
    vector<ExParticle>  in_parts_cpy ;  

    // Copy of output particles for reporting
    vector<ExParticle> out_parts_cpy ;  

    // inputing the cut options
    virtual void  input(std::string) ;   

};


//=============================================================

#endif
