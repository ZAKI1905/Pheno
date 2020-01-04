#ifndef Cut_H
#define  Cut_H

#include "Basics.h"
#include "ExEvent.h"
#include "ExParticle.h"

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
  protected:
    Logger logger ;
  //--------------------------------------------------------------
  public:

    // Constructor takes a reference to an event
    Cut(ExEvent &) ;

    // Virtual destructor
    virtual ~Cut() ;

    //-------- Applying Cut ------------
    // Usual definition for when we want to modify the 
    // input particle list
    void Apply(std::vector<ExParticle>&) ; 

    // /* Overloaded definiton, if we want to save the 
    //   particles passing the cut in another list instead.
    // */
    // void apply(std::vector<Particle>&, std::vector<Particle>& )    ;  
    // //----------------------------------

  //--------------------------------------------------------------

  protected:

    // Cut label
    std::string name = "unnamed_cut" ;

    // Ref. to the event
    ExEvent& ev_ref ;  

    // cut_cond checks cut and adds those not passing it to rm_list 
    virtual void CutCond(std::vector<ExParticle>&) = 0 ;   
    
    // inputing the cut options
    virtual void  Input(std::string) ;  

    // Saves cut details in a .txt file
    void Report() const ;    

    // The removal list
    std::vector<int> rm_list ;  

    // Report cut or not
    bool  report_cut = false ;  

    // The title used to identify various cuts
    std::string report_title = "cut" ;  

    // Cut's intermediate message in the reportfile 
    std::string special_message = "" ;  

    // Cut report file name
    char  cut_report_char[100] ;  
    
    // Copy of input particles for reporting
    std::vector<ExParticle>  in_parts_cpy ;  

    // Copy of output particles for reporting
    std::vector<ExParticle> out_parts_cpy ;   

};

//=============================================================

#endif
