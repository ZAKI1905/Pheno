/*
  ExParticle class

  Last Updated by Zaki on July 6, 2019

*/

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include "Pythia8/Pythia.h"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/Selector.hh"
#include "../include/ExParticle.h"
#include "../include/Basics.h"
#include "../include/EV.h"

using std::vector ;
using namespace Pythia8 ;

//==============================================================

//--------------------------------------------------------------
// Constructor
// Takes a particle.
ExParticle::ExParticle( const Particle& pt) : Particle(pt)
{
  // initializing the measured momentum
  setMom() ;
}

//--------------------------------------------------------------
// '==' comparison operator
bool ExParticle::operator==(const ExParticle& right)
{
    if ( this == &right ) return true ;
    
    else return ( this->e() == right.e() && this->px() == right.px() && 
    this->py() == right.py() && this->pz() == right.pz() 
    && this->id() == right.id() ) ;
}

//--------------------------------------------------------------
// Member function to set the Event pointer.
void ExParticle::setEVPtr(EV* evPtrIn) { EVPtr = evPtrIn ;}

//--------------------------------------------------------------
// Smears the momentum mesured in the detectors for electron
// and muons
void ExParticle::setMom()
{

  ptMom = this->p() ;
  float pt_res ;

  if(this->idAbs() == ID_ELECTRON)
    pt_res = elecMomRes() ;

  else if(this->idAbs() == ID_MUON)
    pt_res = muonMomRes() ;
  
  else return ;

  // time-based seed:
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count() ;
  // unsigned seed = 123456789 ;

  std::default_random_engine        generator(seed);
  std::normal_distribution<double>  distribution( 0, pt_res ) ;

  ptMom = this->p()*( 1 + distribution(generator) ) ;

}

//--------------------------------------------------------------
/* The PDF for the electron observed momentum given
  the true momentum's value.
  Reference: Fig. 11 of "1502.02701" 
*/
float ExParticle::elecMomRes()
{
  float pt_res ;

  
  // If in the barrel
  if( abs( this->eta() ) <= 1.479 )
  {
  // -----------------------
    if ( this->pT() < 6.98) 
      pt_res = 0.03 ;

    else if( 6.98 <= this->pT() && this->pT() < 10.02)
      pt_res = 2.095*0.01 ;

    else if( 10.02 <= this->pT() && this->pT() < 14.96)
      pt_res = 1.959*0.01 ;

    else if( 14.96 <= this->pT() && this->pT() < 19.97)
      pt_res = 1.921*0.01 ;

    else if( 19.97 <= this->pT() && this->pT() < 24.98)
      pt_res = 1.795*0.01 ;

    else if( 24.98 <= this->pT() && this->pT() < 29.99)
      pt_res = 1.722*0.01 ;

    else if( 29.99 <= this->pT() && this->pT() < 35.00)
      pt_res = 1.629*0.01 ;

    else if( 35.00 <= this->pT() && this->pT() < 40.00)
      pt_res = 1.538*0.01 ;

    else if( 40.00 <= this->pT() && this->pT() < 50.00)
      pt_res = 1.491*0.01 ;

    else if( 50.00 <= this->pT() && this->pT() <= 100.00)
      pt_res = 1.347*0.01 ;

    else if( 100 < this->pT() )
      pt_res = 1.347*0.01 ;
  }
  // -----------------------
  // If in the endcap region
  else if( 1.479 < abs(this->eta())  &&  abs(this->eta()) <= 3.0 )
  {
    if ( this->pT() < 6.98) 
      pt_res = 0.2 ;

    if( 6.98 <= this->pT() && this->pT() < 10.02)
      pt_res = 10.615*0.01 ;

    else if( 10.02 <= this->pT() && this->pT() < 14.99)
      pt_res = 8.583*0.01 ;

    else if( 14.99 <= this->pT() && this->pT() < 20.00)
      pt_res = 6.932*0.01 ;

    else if( 20.00 <= this->pT() && this->pT() < 24.98)
      pt_res = 5.903*0.01 ;

    else if( 24.98 <= this->pT() && this->pT() < 29.99)
      pt_res = 5.304*0.01 ;

    else if( 29.99 <= this->pT() && this->pT() < 35.00)
      pt_res = 4.809*0.01 ;

    else if( 35.00 <= this->pT() && this->pT() < 40.00)
      pt_res = 4.547*0.01 ;

    else if( 40.00 <= this->pT() && this->pT() < 50.00)
      pt_res = 4.189*0.01 ;

    else if( 50.00 <= this->pT() && this->pT() <= 100.00)
      pt_res = 3.530*0.01 ;

    else if( 100 < this->pT() )
      pt_res = 3.530*0.01 ;
  }
  // -----------------------

  // For eta > 3.0 , I don't know what pt_res should be but
  // the electrons wouldn't pass the cut anyways. 
  else pt_res = 0.05 ;
  
  return pt_res ;
}

//--------------------------------------------------------------
/* The PDF for the muon observed momentum given
  the true momentum's value.
  Reference: Section 7.1 of "1804.04528" 
   "The resolution for muons with momenta up to approximately
   100 GeV is 1% in the barrel and 3% in the endcap."
*/
float ExParticle::muonMomRes()
{

  float pt_res ;

  // If in the barrel
  if( abs( this->eta() ) < 0.9)
    pt_res = 0.01 ;
  
  // If in the overlap region
  else if( 0.9 <= abs(this->eta())  &&  abs(this->eta()) < 1.2 )
    pt_res = 0.02 ;

  // If in the endcap region
  else if( 1.2 <= abs(this->eta())  &&  abs(this->eta()) <= 2.4 )
    pt_res = 0.03 ;
  
  // I don't know what pt_res should be but
  // for eta > 2.4 the muon wouldn't pass the cut anyways. 
  else pt_res = 0.03 ;
  
  return pt_res ;
}

//--------------------------------------------------------------

Vec4 ExParticle::mom()
{
  return ptMom ;
}

//--------------------------------------------------------------
// If it decays hadronically
bool ExParticle::isHadDec()
{

  if( !HadDecisSet )
    setHadDec() ;

  return DecaysToHad ;
}

//--------------------------------------------------------------

void ExParticle::setHadDec()
{
  DecaysToHad = EVPtr->had_dec(*this) ;
  HadDecisSet = true ;
}

//--------------------------------------------------------------

void ExParticle::setVisMom()
{
  visibleMom  = EVPtr->vis_p(*this) ;
  visMomisSet = true ;
} 

//--------------------------------------------------------------

Vec4 ExParticle::visMom()
{
  if( !visMomisSet )
    setVisMom() ;
    
  return visibleMom ;
}

//--------------------------------------------------------------

double ExParticle::idEff()
{
  if( !idEffisSet )
    setIdEff() ;
    
  return id_eff_val ;
}

//--------------------------------------------------------------

void ExParticle::setIdEff()
/*
 Evaluates the efficiency of detectors 
 in detecting this particle.
*/
{
  idEffisSet = true ;

  if ( this->pT() < 10 )
    {id_eff_val = 0 ; return ;}

  if (this->idAbs() == ID_ELECTRON)
  {
    id_eff_val = 0.91 - 6.83 /(this->pT()) + 78.1/(pow(this->pT(),2)) - 731/(pow(this->pT(),3));

  } else if(this->idAbs() == ID_MUON)
    {
      id_eff_val = 0.94 - 2.85/(this->pT()) + 41.9/(pow(this->pT(),2)) - 227/(pow(this->pT(),3));

    } else if(this->idAbs() == ID_TAU &&  this->isHadDec() )
      {
        id_eff_val = 0.7 ;
      }

}

//==============================================================