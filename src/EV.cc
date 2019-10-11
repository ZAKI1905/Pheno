/*
  EV class

  Last Updated by Zaki on July 6, 2019

*/

#include <iostream>
#include <vector>
#include <algorithm>
#include "Pythia8/Pythia.h"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/Selector.hh"
#include "../inc/EV.h"
#include "../inc/Basics.h"
#include "../inc/GenJet.h"

using std::vector ;
using namespace Pythia8 ;

//==============================================================

//--------------------------------------------------------------
// Constructor
// Takes event number and an event.
EV::EV(int set_ev_num, Event& event):event_number(set_ev_num)
{

  for(int i=0 ; i < event.size() ; ++i)
  {
      ExParticle pt(event[i]) ;
      // sets the event pointer for pt
      setEVPtr(pt) ;

      // Saving the event into a particle vector
      Full_Event.push_back(pt) ;

      /*
        // Filling the pseudojet list with particles.
        Note (May-12-2019): This doesn't have to be this early,
         but might save time by removing a loop.
      */
      gen_psjts(event[i]) ;

  }

}

//--------------------------------------------------------------
EV::~EV() { }

//--------------------------------------------------------------
// Member function to set the Event pointer.
void EV::setEVPtr(ExParticle& pt)
{
  pt.setEVPtr(this) ;
}

//--------------------------------------------------------------
// Member function to set the Cut pointer.
void EV::addCutPtr(std::shared_ptr<Cut>cutPtrIn) 
{ 
  CutPtr.push_back(cutPtrIn) ;
}

//--------------------------------------------------------------
// Member function to set the GenJet pointer.
void EV::addGenJetPtr(GenJet* genjetPtrIn) 
{ GenJetPtr = genjetPtrIn ; }

//--------------------------------------------------------------
// Saves pseudojet lists for fastjet
void EV::gen_psjts(ExParticle p)
{

  if( p.isFinal() )
  {
    if( p.idAbs() == ID_ELECTRON || p.idAbs() == ID_MUON )
      lep_jts.push_back(p) ;

    else
    {
      fastjet::PseudoJet particle(p.px(),p.py(),p.pz(),p.e()) ;
      particle.set_user_index(p.chargeType()) ;
      // particle.set_user_index(-50) ;
      ps_jts.push_back(particle) ;
    }
  }

}

//--------------------------------------------------------------
// Inputing options

void EV::input(string property, bool value)
{
  if(property == "Report_Taus")
  {
    tau_report = value ;
  }
}

//--------------------------------------------------------------
// Finding the hadronically decaying taus
void EV::find_had_tau(std::string filename)
{

std::FILE * pTAU_REPORT_FILE ;
// char pTAU_REPORT_CHAR[50];
// sprintf(pTAU_REPORT_CHAR, "Tau_Report.txt");
if (tau_report) pTAU_REPORT_FILE = fopen(filename.c_str(), "a") ;

for(size_t i=0 ; i < Full_Event.size() ; ++i)
{

  if ( Full_Event[i].idAbs()!=ID_TAU ) continue ;

  if(Full_Event[i].daughter2()>=Full_Event[i].daughter1() && tau_report)
  {
    vector<ExParticle>::const_iterator first = Full_Event.begin() +
     Full_Event[i].daughter1() ;
    vector<ExParticle>::const_iterator last = Full_Event.begin() +
     Full_Event[i].daughter2() + 1 ;
    vector<ExParticle> Daughter_Range(first, last) ;

    // Actual, Visible
    vector<Vec4> Tau_Mom{Full_Event[i].p(), Full_Event[i].visMom() } ;

    fprintf(pTAU_REPORT_FILE, "\n %s Event # %-5d %s", pr(25,'-').c_str()
    ,event_number, pr(25,'-').c_str() ) ;

    fprintf(pTAU_REPORT_FILE, "\n  %s \n | %-6s(%3d) | %8s | %8s | %8s |\n",
    pr(46,'=').c_str(), Full_Event[i].name().c_str(),Full_Event[i].status(),
     "Energy", "p_T", "p_Z") ;

    fprintf(pTAU_REPORT_FILE, " | %s |\n | %-11s | %8.2f | %8.2f | %8.2f |\n",
    pr(44,'-').c_str(), "Actual",Tau_Mom[0].e(), Tau_Mom[0].pT(), Tau_Mom[0].pz()) ;

    fprintf(pTAU_REPORT_FILE, " | %-11s | %8.2f | %8.2f | %8.2f |\n",
    "Visible",Tau_Mom[1].e(), Tau_Mom[1].pT(), Tau_Mom[1].pz()) ;

    fprintf(pTAU_REPORT_FILE,
    " | %-11s | %8.2f | %8.2f | %8.2f |\n  %s\n  %s\n | Daughters |",
    "Difference" , Tau_Mom[0].e()-Tau_Mom[1].e(),
    Tau_Mom[0].pT()-Tau_Mom[1].pT(), Tau_Mom[0].pz()-Tau_Mom[1].pz(),
    pr(46,'=').c_str() ,pr(11,'_').c_str() ) ;

    print_table<ExParticle>(pTAU_REPORT_FILE, Daughter_Range) ;
  }

  Full_Event[i].setHadDec() ;

  if( had_dec(Full_Event[i]) ) h_taus.push_back(Full_Event[i]) ;

}
if(tau_report) fclose(pTAU_REPORT_FILE) ;

}

//--------------------------------------------------------------
// Prints a table of particles in list with their info.
template <class T>
void EV::print_table(std::FILE * file, vector<T> list)
{
  // Double numbers' precision
  int prec = 5;

  if( list.size() > 0 )
  {
    // label the columns
    fprintf(file, "\n |%-s\n | %2s %8s %8s %13s %13s %13s %13s %6s %13s |\n",\
      pr(99,'=').c_str(), "#", "Name", "Status", "E_Truth", "E",
       "P_T", "P_Z", "P_Rap", "M_Vis") ;
    fprintf(file, " | %s |\n",pr(97,'-').c_str() ) ;

    for(uint j  = 0 ; j < (uint) list.size() ; j++)
    {
      ExParticle p  = list[j] ;

      fprintf(file," | %2u %8s %+8d %13.*E %13.*E %13.*E %+13.*E %+6.1f %13.*E |\n",
       j+1, p.name().c_str(), p.status(),
       prec, p.e(), prec, p.visMom().e(),
       prec, p.visMom().pT(), prec, p.visMom().pz(),
       p.visMom().eta(), prec, p.visMom().mCalc() ) ;
    }
      fprintf(file,"  %s\n",pr(99,'=').c_str() ) ;
    }
}

//--------------------------------------------------------------
// Returns event number

int EV::i() const
{
  return event_number ;
}

//--------------------------------------------------------------
//Returns a reference to the full event

vector<ExParticle>& EV::Full_Ev()
{
  return Full_Event ;
}

//--------------------------------------------------------------
// Returns the size of the event

size_t EV::size() const
{
  return Full_Event.size() ;
}


//--------------------------------------------------------------
bool EV::had_dec(ExParticle& p)
// If a particle decays hadronically
{
    bool hadronic_decay = false ;

    if(0 < p.daughter1() && p.daughter1()< p.daughter2())
    {
    // Looping over indices:
      for(int i=p.daughter1() ; i <= p.daughter2() ; ++i )
      {
        if(Full_Event[i].isHadron())
        {
          hadronic_decay = true  ;
          break ;
        }
      }

    }

    return  hadronic_decay ;
}

//--------------------------------------------------------------
// Returns the hadronically decaying taus.

vector<ExParticle>& EV::htau()
{
  return h_taus ;
}

//--------------------------------------------------------------
Vec4 EV::vis_p(ExParticle& p)
// Returns the visible momentum of a particle
{

  if ( p.isFinal() ) return p.mom() ;

  Vec4              four_mom(0) ;
  vector<ExParticle>  Temp_Parts ;
  Temp_Parts.push_back(p) ;


  for(size_t j=0 ; j<Temp_Parts.size() ; j++)
  {
    //Either the particle has a range of decay products or 
    // a "carbon copy" as its sole daughter.
    if(0 < Temp_Parts[j].daughter1() && 
       Temp_Parts[j].daughter1()<= Temp_Parts[j].daughter2())
    {

        for(int i= Temp_Parts[j].daughter1() ; i<= Temp_Parts[j].daughter2() ; ++i )
        {
            // If the daughters are not final, then their daughters
            // will be checked and so on.
            if(Full_Event[i].isFinal())
            {
              if(Full_Event[i].isVisible()) four_mom += Full_Event[i].mom() ;
            }
            else Temp_Parts.push_back(Full_Event[i]) ;
        }
    }
    else four_mom += Temp_Parts[j].mom() ;
  }

  return four_mom ;
}

//--------------------------------------------------------------
// Overloading "()":
// ev({11,-11}) returns a list of particles with id's 11 and -11.

vector<ExParticle> EV::operator() (vector<int> sel_id)
{

  vector<ExParticle> particle_set ;

  for(size_t     i  = 0 ; i < Full_Event.size() ; ++i)
  {
      for(size_t j  = 0 ; j < sel_id.size() ; ++j)
      {
          /*  Selects the particle with specific ID if it's either in
               final state or it's a hadronically decaying tau lepton:
          */
          if(Full_Event[i].id()==sel_id[j] && (Full_Event[i].isFinal() \
          || (Full_Event[i].idAbs()==ID_TAU && Full_Event[i].isHadDec() ) ) )
          {
              particle_set.push_back(Full_Event[i]) ;
          }

      }
  }
  return particle_set ;
}

//--------------------------------------------------------------
double EV::weight()
{
  if( !eVWeightisSet && !drop_low_eff )
    setEVWeight() ;
    
  return weight_val ;
}

//--------------------------------------------------------------
void EV::setEVWeight()
{

  for (size_t i=0 ; i<iso_leptons.size() ; ++i)
  {
    if( iso_leptons[i].idEff() != 0 )
      weight_val *= iso_leptons[i].idEff() ;
  }

  eVWeightisSet = true ;

}

//--------------------------------------------------------------
// Returns a reference to the pseudo_jets stored in the event
vector<fastjet::PseudoJet>& EV::psjet()
{
  return ps_jts ;
}


//--------------------------------------------------------------
// Returns a reference to the lep_pseudo_jets stored in the event
vector<ExParticle>& EV::lep_for_jet()
{
  return lep_jts ;
}

//--------------------------------------------------------------
// Returns a reference to the leptons that passed all the cuts
vector<ExParticle>& EV::pass_lepts()
{
  return iso_leptons ;
}

//--------------------------------------------------------------
// Updates the list of leptons that passed the cuts
void EV::update_pass_lepts(vector<ExParticle> input)
{
  iso_leptons = input ;
}

//--------------------------------------------------------------
// Updating hadronic tau list for binning
void EV::update_hadtaus( vector<ExParticle> pass_cuts )
{
  vector<int> tmp_rm_lst ;

  for ( size_t i=0 ; i < h_taus.size() ; ++i)
  {
    // If the tau hasn't passed the cut
    if( !contains( pass_cuts, h_taus[i]) )
      // remove it
      add_elem( tmp_rm_lst , (int) i) ;
  }

  rm_elem(h_taus, tmp_rm_lst) ;

}

//--------------------------------------------------------------
//
void EV::update_ps_jets()
{
  vector<int> tmp_rm_lst ;

  for(size_t i=0 ; i < lep_jts.size() ; ++i )
  {
    if( !contains( iso_leptons , lep_jts[i] ) )
    {
      ExParticle p = lep_jts[i] ;
      fastjet::PseudoJet particle(p.mom().px(),p.mom().py(),
      p.mom().pz(),p.mom().e()) ;
      particle.set_user_index(p.chargeType()) ;
      // particle.set_user_index(-50) ;
      ps_jts.push_back(particle) ;
    }
  }

}

//--------------------------------------------------------------
void EV::print(std::string filename)
{
  // For saving the full event record in a text file:
  std::FILE * Event_report_file ;
  Event_report_file = fopen(filename.c_str(), "a") ;

  fprintf(Event_report_file, " Event #: %-5d %20s Particles: %-4d\n\n",
   this->i()," ", (int)this->size()-1 ) ;

  for( size_t i=0 ;  i< this->size() ; ++i )
  {
    ExParticle p = Full_Event[i] ;
    
    // Header for table entries (every 40 events):
    if(i%40 ==0)
    {
    fprintf(Event_report_file, "\n %s\n %-4s %4s %-10s %-6s %2s %8s %5s",
    pr(99,'=').c_str(),"#"," ","Name","Status"," ", "Energy"," ") ;

    fprintf(Event_report_file, " %8s %5s %8s %10s","p_T"," ", "p_Z"," ") ;

    fprintf(Event_report_file, " %4s %5s %4s %5s\n %s\n","D_1"," ", "D_2"," ",
    pr(99,'=').c_str()) ;
    }

    fprintf(Event_report_file, " %-4u %4s %-10s %1s %2d %3s %8.1f %5s",(int)i,
    " ", p.name().c_str()," ", p.status()," ", p.mom().e(), " ") ;

    fprintf(Event_report_file, " %8.1f %5s %8.1f %10s", p.mom().pT()," ",
     p.mom().pz()," ") ;

    fprintf(Event_report_file, " %4d %5s %4d %5s\n", p.daughter1()," ",
     p.daughter2()," ") ;

  }
  fclose(Event_report_file) ;
}

//==============================================================
//                    explicit instantiations
//==============================================================

// print_table
template void EV::print_table<ExParticle>(std::FILE * file, vector<ExParticle>) ;
template void EV::print_table<Particle>(std::FILE * file, vector<Particle>) ;

//==============================================================
