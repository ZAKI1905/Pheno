#ifndef Binner_H
#define Binner_H



#include "ExEvent.h"

//--------------------------------------------------------------
namespace PHENO
{

//==============================================================
class Binner : public Prog
{
//--------------------------------------------------------------
public:

  // Constructor
    Binner() ;

  // Virtual destructor
    virtual ~Binner() ;

    std::unique_ptr<Binner> Clone() const ;

  // Setting the binner label
    void SetName(const char*) ;

  // For inputting properties
    virtual void Input(std::string) = 0 ;
  
  // For inputting event
    virtual void Input(ExEvent&) = 0 ;

  // bins the event and adds to cut report if "true"
    virtual void BinIt(bool cut_report, std::string) = 0 ;

  // Reporting the binning results
    virtual void Report(std::string) const = 0 ;

  // Getting the binner label
    std::string GetName() const ;

//--------------------------------------------------------------
protected:

  // Binner label
    std::string name = "unnamed_binner" ;

  // Pointer to the event 
    ExEvent* evp = nullptr ;

  virtual Binner* IClone() const = 0;
};

//==============================================================
//                  Binner Options Struct
//==============================================================

// Binner options includes a binner and its options
struct BinnerOptions : public Prog
{
  std::shared_ptr<Binner> binnerPtr ;
  std::vector<std::string> options ;

  //--------------------------------------------------------------
  // Constructor
  BinnerOptions(Binner* in_binner, std::string command="")
  {
    binnerPtr = in_binner->Clone() ;
    // binnerPtr.reset(in_binner) ;

    // stripping spaces from command
    command = Zaki::String::Strip(command, ' ') ;

    Zaki::String::Str2Lst( options, command )  ;

    InputOptions() ;
  }

  //--------------------------------------------------------------
  // Copy Constructor
  BinnerOptions(const BinnerOptions& old_obj)
  {
    binnerPtr = old_obj.binnerPtr->Clone() ;

    options = old_obj.options ;
  }

  ~BinnerOptions(){}
  //--------------------------------------------------------------
  // Getting the string form
  std::string GetStrForm()
  {
    std::string out_str = "Binner Name: '" + binnerPtr->GetName() + "'";

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
      binnerPtr->Input(options[i]) ;
    }
  }
  //--------------------------------------------------------------

};

//==============================================================
} // PHENO namespace
//=============================================================
#endif /*Binner_H*/
