#ifndef Cut_H
#define  Cut_H

#include "Basics.h"
#include "ExEvent.h"
#include "ExParticle.h"

//--------------------------------------------------------------
namespace PHENO
{
  class Pheno;
//--------------------------------------------------------------
namespace CUTS
{
//=============================================================
/*
  Cut class takes an event, and a removal list and outputs
  another removal list, reports the Cut if asked by the user.
*/
class Cut : public Prog
{
  // Gives access to input, etc.
  friend class PHENO::Pheno ;   
  friend struct CutOptions ;
  //--------------------------------------------------------------
  protected:
    
    // // Protected Constructor for use in derived
    // // copy constructors
    // Cut(std::string name) ;
  //--------------------------------------------------------------
  public:

    typedef std::vector<ExParticle> ParticleLST ;

    // Default constructor
    Cut() ;

    // Constructor takes a pointer to an event
    Cut(ExEvent*) ;

    // // Copy Constructor
    // Cut(const Cut&) ;

    // Virtual destructor
    virtual ~Cut() ;

    //-------- Applying Cut ------------
    // Usual definition for when we want to modify the 
    // input particle list
    void Apply(ParticleLST&) ; 

    // /* Overloaded definiton, if we want to save the 
    //   particles passing the cut in another list instead.
    // */
    // void apply(std::vector<Particle>&, std::vector<Particle>& )    ;  
    // //----------------------------------

    // Getting the cut label
    std::string GetName() const ;
  
  //--------------------------------------------------------------

  protected:

    //.......................
    // Setters
    //.......................
    // Setting the cut label
    void SetName(const char*) ;

    // Setting pointer to an event
    void SetEventPtr(ExEvent*) ;

    // inputing the cut options
    virtual void Input(std::string) ;  

    //.......................
    // Getters
    //.......................
    // Getting pointer to an event
    ExEvent* GetEventPtr() const ;

    virtual std::shared_ptr<Cut> Clone() = 0 ;

    //.......................
    // Saves cut details in a .txt file
    void Report() const ;    
    // cut_cond checks cut and adds those not passing it to rm_list 
    virtual void CutCond(ParticleLST&) = 0 ;   
    //.......................


    //.......................
    //  Protected Vars
    //.......................
    // Cut label
    std::string name = "unnamed_cut" ;

    // Pointer to the event
    ExEvent* evPtr = NULL ;  

    // The removal list
    std::vector<int> rm_list ;  

    // Report cut or not
    bool report_cut = false ;  

    // The title used to identify various cuts
    std::string report_title = "cut" ;  

    // Cut's intermediate message in the reportfile 
    std::string special_message = "" ;  

    // Cut report file name
    char  cut_report_char[100] ;  
    
    // Copy of input particles for reporting
    ParticleLST  in_parts_cpy ;  

    // Copy of output particles for reporting
    ParticleLST out_parts_cpy ;   
};

//==============================================================
//                  Cut Options struct
//=============================================================

// Cut options includes a cut and its options
struct CutOptions : public Prog
{
  std::shared_ptr<Cut> cutPtr ;
  // Cut* cut ;
  std::vector<std::string> options ;

  //--------------------------------------------------------------
  // Constructor
  CutOptions(Cut* in_cut, std::string command="")
  {
    cutPtr.reset(in_cut) ;

    // stripping spaces from command
    command = Zaki::String::Strip(command, ' ') ;

    Zaki::String::Str2Lst( options, command )  ;

    InputOptions() ;
  }

  //--------------------------------------------------------------
  // Copy Constructor
  CutOptions(const CutOptions& old_obj)
  {
    cutPtr = old_obj.cutPtr->Clone() ;

    options = old_obj.options ;
  }

  ~CutOptions() {}
  //--------------------------------------------------------------
  // Getting the string form
  std::string GetStrForm()
  {
    std::string out_str = "Cut Name: '" + cutPtr->GetName() + "'";

    // if (options.size() == 0 )
    if (options[0] == "" )
      return out_str ;

    out_str += "\n - Options: " ;

    for(size_t i=0 ; i< options.size(); i++)
    {
      out_str += options[i] ;

      // if its the last element 
      // no need for an extra comma
      if ( i == options.size()-1)
        break;
      
      out_str += ", " ; 
    }
    return out_str ;
  }

  //--------------------------------------------------------------
  // Set the input for each cut
  void InputOptions()
  {
    for( size_t i=0 ; i<options.size() ; ++i)
    {
      cutPtr->Input(options[i]) ;
    }
  }
  //--------------------------------------------------------------

};
//=============================================================
} // CUTS namespace
//=============================================================
} // PHENO namespace
//=============================================================
#endif
