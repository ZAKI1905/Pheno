/*
  ExParticle class

  Last Updated by Zaki on July 6, 2019

*/

#include <random>
#include <chrono>
#include <algorithm>

#include "../include/ExEvent.h"

//==============================================================

//--------------------------------------------------------------
// Constructor
// Takes a particle.
PHENO::ExParticle::ExParticle( const Particle& pt) : Particle(pt)
{
  // initializing the measured momentum
  SetMom() ;
}

//--------------------------------------------------------------
// Copy Constructor
// PHENO::ExParticle::ExParticle( const ExParticle& other)
// {
// idSave = other.idSave; statusSave = other.statusSave;
// mother1Save = other.mother1Save; mother2Save = other.mother2Save;
// daughter1Save = other.daughter1Save; daughter2Save = other.daughter2Save;
// colSave = other.colSave; acolSave = other.acolSave; pSave = other.pSave;
// mSave = other.mSave; scaleSave = other.scaleSave; polSave = other.polSave;
// hasVertexSave = other.hasVertexSave; vProdSave = other.vProdSave;
// tauSave = other.tauSave; *pdePtr = *other.pdePtr; *evtPtr = *other.evtPtr; 

// decays_to_had = other.decays_to_had; set_had_dec_flag = other.set_had_dec_flag;
// visible_mom =other.visible_mom; set_vis_mom_flag = other.set_vis_mom_flag;
// detected_mom = other.detected_mom; *event_ptr = *other.event_ptr;
// id_eff_val = other.id_eff_val; set_id_eff_flag = other.set_id_eff_flag ;

// }

//--------------------------------------------------------------
// '==' comparison operator
bool PHENO::ExParticle::operator==(const ExParticle& right) const
{
    if ( this == &right ) return true ;
    
    else return ( this->e() == right.e() && this->px() == right.px() && 
    this->py() == right.py() && this->pz() == right.pz() 
    && this->id() == right.id() ) ;
}

//--------------------------------------------------------------
// Member function to set the Event pointer.
void PHENO::ExParticle::SetEventPtr(ExEvent* evPtrIn) { event_ptr = evPtrIn ;}

//--------------------------------------------------------------
// Smears the momentum mesured in the detectors for electron
// and muons
void PHENO::ExParticle::SetMom()
{
  // If visible, returns the true momentum, otherwise '0'.
  // ptMom = (this->isVisible()) ? this->p() : 0;

  detected_mom = this->p() ;
  float pt_res ;

  if(this->idAbs() == ID_ELECTRON)
    pt_res = ElectronMomRes() ;

  else if(this->idAbs() == ID_MUON)
    pt_res = MuonMomRes() ;
  
  else return ;

  // time-based seed:
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count() ;
  // unsigned seed = 123456789 ;

  std::default_random_engine        generator(seed);
  std::normal_distribution<double>  distribution( 0, pt_res ) ;

  detected_mom = this->p()*( 1 + distribution(generator) ) ;

  // cout<<"\n"<<"- Random number is: "<<distribution(generator)<<endl ;
}

//--------------------------------------------------------------
/* The PDF for the electron observed momentum given
  the true momentum's value.
  Reference: Fig. 11 of "1502.02701" 
*/
float PHENO::ExParticle::ElectronMomRes()
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

    else //  100 < this->pT()
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

    else // if( 100 < this->pT() )
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
float PHENO::ExParticle::MuonMomRes()
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

Pythia8::Vec4 PHENO::ExParticle::GetMom()
{
  return detected_mom ;
}

//--------------------------------------------------------------
// If it decays hadronically
bool PHENO::ExParticle::IsHadDec()
{
  if( !set_had_dec_flag )
    SetHadDec() ;

  return decays_to_had ;
}

//--------------------------------------------------------------

void PHENO::ExParticle::SetHadDec()
{
  decays_to_had = event_ptr->HadronicDecay(*this) ;
  set_had_dec_flag = true ;
}

//--------------------------------------------------------------

void PHENO::ExParticle::SetVisMom()
{
  visible_mom  = event_ptr->VisMom(*this) ;
  set_vis_mom_flag = true ;
} 

//--------------------------------------------------------------

Pythia8::Vec4 PHENO::ExParticle::GetVisMom()
{
  if( !set_vis_mom_flag )
    SetVisMom() ;
    
  return visible_mom ;
}

//--------------------------------------------------------------

double PHENO::ExParticle::GetIdEff()
{
  if( !set_id_eff_flag )
    SetIdEff() ;
    
  return id_eff_val ;
}

//--------------------------------------------------------------
void PHENO::ExParticle::SetIdEff()
/*
 Evaluates the efficiency of detectors 
 in detecting this particle.
*/
{
  set_id_eff_flag = true ;

  if ( this->pT() < 10 )
    {id_eff_val = 0 ; return ;}

  if (this->idAbs() == ID_ELECTRON)
  {
    id_eff_val = 0.91 - 6.83 /(this->pT()) + 78.1/(pow(this->pT(),2)) - 731/(pow(this->pT(),3));

  } else if(this->idAbs() == ID_MUON)
    {
      id_eff_val = 0.94 - 2.85/(this->pT()) + 41.9/(pow(this->pT(),2)) - 227/(pow(this->pT(),3));

    } else if(this->idAbs() == ID_TAU &&  this->IsHadDec() )
      {
        id_eff_val = 0.7 ;
      }

}

//==============================================================