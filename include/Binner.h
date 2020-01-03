#ifndef Binner_H
#define Binner_H

// #include <vector>
// #include <Pythia8/Pythia.h>

#include "EV.h"
// using namespace Pythia8 ;

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
    virtual void input(std::string) = 0 ;
  
  // For inputting event
    virtual void input(EV&) = 0 ;

  // bins the event and adds to cut report if "true"
    virtual void bin_it(bool cut_report, std::string) = 0 ;

  // bins the event and adds to cut report if "true"
    virtual void report(std::string) = 0 ;

//--------------------------------------------------------------
protected:

  // Pointer to the event 
    EV* evp = NULL ;
  
};


//==============================================================

#endif /*Binner_H*/
