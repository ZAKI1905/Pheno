#ifndef Binner_H
#define Binner_H

#include "ExEvent.h"

//==============================================================
class Binner
{
//--------------------------------------------------------------
public:

  // Constructor
    Binner() ;

  // Virtual destructor
    virtual ~Binner() ;

  // For inputting properties
    virtual void Input(std::string) = 0 ;
  
  // For inputting event
    virtual void Input(ExEvent&) = 0 ;

  // bins the event and adds to cut report if "true"
    virtual void BinIt(bool cut_report, std::string) = 0 ;

  // bins the event and adds to cut report if "true"
    virtual void Report(std::string) const = 0 ;

//--------------------------------------------------------------
protected:

  // Pointer to the event 
    ExEvent* evp = NULL ;
  
};

//==============================================================

#endif /*Binner_H*/
