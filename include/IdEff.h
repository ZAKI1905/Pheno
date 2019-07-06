#ifndef IdEff_H
#define IdEeff_H

#include <vector>
#include "Pythia8/Pythia.h"
#include "Basics.h"
#include "Cut.h"

using    std::vector ;
using    namespace Pythia8 ;

//==============================================================
class IdEff : public Cut
{

// Gives access to input, etc.
friend class Pheno ;   

//--------------------------------------------------------------

public:

  // Constructor
  IdEff(EV &) ;

//--------------------------------------------------------------

private:

  // Virtual method from cut class
  void cut_cond(vector<ExParticle>&) ;    

  void input(std::string) ;

};

//==============================================================
#endif /*IdEff_H*/
