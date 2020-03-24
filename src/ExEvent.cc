/*
  EV class

  Last Updated by Zaki on July 6, 2019

*/

#include <iostream>
#include <algorithm>

#include "../include/ExEvent.h"
#include "../include/GenJet.h"

//==============================================================
//--------------------------------------------------------------
// Constructor
// Takes event number and an event.
ExEvent::ExEvent(int set_ev_num, Pythia8::Event& event) :
 event_number(set_ev_num)
{
  PROFILE_FUNCTION() ;
  for(int i=0 ; i < event.size() ; ++i)
  {
    ExParticle pt(event[i]) ;
    // sets the event pointer for pt
    SetEventPtr(pt) ;

    // Saving the event into a particle vector
    full_event.push_back(pt) ;

    /*
      // Filling the pseudojet list with particles.
      Note (May-12-2019): This doesn't have to be this early,
        but might save time by removing a loop.
    */
    GenPseudoJets(event[i]) ;
  }
}

//--------------------------------------------------------------
ExEvent::~ExEvent() { }
//--------------------------------------------------------------
// Copy constructor
// Resets the cut pointers!
ExEvent::ExEvent(const ExEvent& other) : 
drop_low_eff(other.drop_low_eff), event_number(other.event_number),
weight_val(other.weight_val), set_event_weight_flag(other.set_event_weight_flag),
tau_report_flag(other.tau_report_flag), had_taus(other.had_taus),
full_event(other.full_event), isolated_leptons(other.isolated_leptons),
pseudo_jet_set(other.pseudo_jet_set), lepton_jets(other.lepton_jets)
{
  // GenJet pointer for STBinner
  *GenJetPtr = *other.GenJetPtr ;
  GenJetPtr->SetEventRef(*this) ;

  // Not working !
  // cut_ptr.reserve(other.cut_ptr.size() ) ;
  // for (size_t i = 0; i < other.cut_ptr.size() ; i++)
  // {
  //   std::shared_ptr<Cut> tmp = std::make_shared<Cut>(*other.cut_ptr[i]);
  //   cut_ptr[i].reset() ;
  // }

}
//--------------------------------------------------------------
// Member function to set the Event pointer.
void ExEvent::SetEventPtr(ExParticle& pt)
{
  pt.SetEventPtr(this) ;
}

//--------------------------------------------------------------
// Member function to set the Cut pointer.
void ExEvent::AddCutPtr(std::shared_ptr<Cut>cutPtrIn) 
{ 
  cut_ptr.push_back(cutPtrIn) ;
}

//--------------------------------------------------------------
// Member function to set the GenJet pointer.
void ExEvent::AddGenJetPtr(GenJet* genjetPtrIn) 
{ GenJetPtr = genjetPtrIn ; }

//--------------------------------------------------------------
// Saves pseudojet lists for fastjet
void ExEvent::GenPseudoJets(ExParticle p)
{

  if( p.isFinal() )
  {
    if( p.idAbs() == ID_ELECTRON || p.idAbs() == ID_MUON )
      lepton_jets.push_back(p) ;

    else
    {
      fastjet::PseudoJet particle(p.px(),p.py(),p.pz(),p.e()) ;
      particle.set_user_index(p.chargeType()) ;
      // particle.set_user_index(-50) ;
      pseudo_jet_set.push_back(particle) ;
    }
  }

}

//--------------------------------------------------------------
// Inputing options
void ExEvent::Input(std::string property, bool value)
{
  if(property == "Report_Taus")
  {
    tau_report_flag = value ;
  }
}

//--------------------------------------------------------------
// Finding the hadronically decaying taus
void ExEvent::FindHadTaus(std::string filename)
{
  PROFILE_FUNCTION() ;
  std::FILE * pTAU_REPORT_FILE ;
  // char pTAU_REPORT_CHAR[50];
  // sprintf(pTAU_REPORT_CHAR, "Tau_Report.txt");
  if (tau_report_flag) pTAU_REPORT_FILE = fopen(filename.c_str(), "a") ;

  for(size_t i=0 ; i < full_event.size() ; ++i)
  {

    if ( full_event[i].idAbs()!=ID_TAU ) continue ;

  if(full_event[i].daughter2()>=full_event[i].daughter1() && tau_report_flag)
  {
    std::vector<ExParticle>::const_iterator first = full_event.begin() +
     full_event[i].daughter1() ;
    std::vector<ExParticle>::const_iterator last = full_event.begin() +
     full_event[i].daughter2() + 1 ;
    std::vector<ExParticle> Daughter_Range(first, last) ;

    // Actual, Visible
    std::vector<Pythia8::Vec4> Tau_Mom{full_event[i].p(), full_event[i].GetVisMom() } ;

    fprintf(pTAU_REPORT_FILE, "\n %s Event # %-5d %s", pr(25,'-').c_str()
    ,event_number, pr(25,'-').c_str() ) ;

    fprintf(pTAU_REPORT_FILE, "\n  %s \n | %-6s(%3d) | %8s | %8s | %8s |\n",
    pr(46,'=').c_str(), full_event[i].name().c_str(),full_event[i].status(),
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

    PrintTable<ExParticle>(pTAU_REPORT_FILE, Daughter_Range) ;
  }

    full_event[i].SetHadDec() ;

    if( HadronicDecay(full_event[i]) ) had_taus.push_back(full_event[i]) ;

  }
  if(tau_report_flag) fclose(pTAU_REPORT_FILE) ;

}

//--------------------------------------------------------------
// Prints a table of particles in list with their info.
template <class T>
void ExEvent::PrintTable(std::FILE * file, std::vector<T> list)
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

    for(size_t j  = 0 ; j < list.size() ; j++)
    {
      ExParticle p  = list[j] ;

      fprintf(file," | %zu %8s %+8d %13.*E %13.*E %13.*E %+13.*E %+6.1f %13.*E |\n",
       j+1, p.name().c_str(), p.status(),
       prec, p.e(), prec, p.GetVisMom().e(),
       prec, p.GetVisMom().pT(), prec, p.GetVisMom().pz(),
       p.GetVisMom().eta(), prec, p.GetVisMom().mCalc() ) ;
    }
      fprintf(file,"  %s\n",pr(99,'=').c_str() ) ;
    }
}

//--------------------------------------------------------------
// Returns event number

int ExEvent::i() const
{
  return event_number ;
}

//--------------------------------------------------------------
//Returns a reference to the full event

std::vector<ExParticle>& ExEvent::FullEvent()
{
  return full_event ;
}

//--------------------------------------------------------------
// Returns the size of the event
size_t ExEvent::size() const
{
  return full_event.size() ;
}


//--------------------------------------------------------------
bool ExEvent::HadronicDecay(ExParticle& p)
// If a particle decays hadronically
{
    bool hadronic_decay = false ;

    if(0 < p.daughter1() && p.daughter1()< p.daughter2())
    {
    // Looping over indices:
      for(int i=p.daughter1() ; i <= p.daughter2() ; ++i )
      {
        if(full_event[i].isHadron())
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

std::vector<ExParticle>& ExEvent::HadronicTaus()
{
  return had_taus ;
}

//--------------------------------------------------------------
Pythia8::Vec4 ExEvent::VisMom(ExParticle& p)
// Returns the visible momentum of a particle
{

  if ( p.isFinal() ) return p.GetMom() ;

  Pythia8::Vec4           four_mom(0) ;
  std::vector<ExParticle>  Temp_Parts ;
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
          if(full_event[i].isFinal())
          {
            if(full_event[i].isVisible()) four_mom += full_event[i].GetMom() ;
          }
          else Temp_Parts.push_back(full_event[i]) ;
      }
    }
    else four_mom += Temp_Parts[j].GetMom() ;
  }

  return four_mom ;
}

//--------------------------------------------------------------
// Overloading "()":
// ev({11,-11}) returns a list of particles with id's 11 and -11.

std::vector<ExParticle> ExEvent::operator() (std::vector<int> sel_id)
{

  std::vector<ExParticle> particle_set ;

  for(size_t     i  = 0 ; i < full_event.size() ; ++i)
  {
      for(size_t j  = 0 ; j < sel_id.size() ; ++j)
      {
          /*  Selects the particle with specific ID if it's either in
               final state or it's a hadronically decaying tau lepton:
          */
          if(full_event[i].id()==sel_id[j] && (full_event[i].isFinal() \
          || (full_event[i].idAbs()==ID_TAU && full_event[i].IsHadDec() ) ) )
          {
              particle_set.push_back(full_event[i]) ;
          }

      }
  }
  return particle_set ;
}

//--------------------------------------------------------------
double ExEvent::Weight()
{
  if( !set_event_weight_flag && !drop_low_eff )
    SetEventWeight() ;
    
  return weight_val ;
}

//--------------------------------------------------------------
void ExEvent::SetEventWeight()
{

  for (size_t i=0 ; i<isolated_leptons.size() ; ++i)
  {
    if( isolated_leptons[i].GetIdEff() != 0 )
      weight_val *= isolated_leptons[i].GetIdEff() ;
  }

  set_event_weight_flag = true ;

}

//--------------------------------------------------------------
// Returns a reference to the pseudo_jets stored in the event
std::vector<fastjet::PseudoJet>& ExEvent::GetPseudoJets()
{
  return pseudo_jet_set ;
}


//--------------------------------------------------------------
// Returns a reference to the lep_pseudo_jets stored in the event
std::vector<ExParticle>& ExEvent::GetLeptonJets()
{
  return lepton_jets ;
}

//--------------------------------------------------------------
// Returns a reference to the leptons that passed all the cuts
std::vector<ExParticle>& ExEvent::PassedLeptons()
{
  return isolated_leptons ;
}

//--------------------------------------------------------------
// Updates the list of leptons that passed the cuts
void ExEvent::UpdatePassedLepts(std::vector<ExParticle> input)
{
  isolated_leptons = input ;
}

//--------------------------------------------------------------
// Updating hadronic tau list for binning
void ExEvent::UpdateHadTaus( std::vector<ExParticle> pass_cuts )
{
  std::vector<int> tmp_rm_lst ;

  for ( size_t i=0 ; i < had_taus.size() ; ++i)
  {
    // If the tau hasn't passed the cut
    if( !contains( pass_cuts, had_taus[i]) )
      // remove it
      add_elem( tmp_rm_lst , (int) i) ;
  }

  rm_elem(had_taus, tmp_rm_lst) ;

}

//--------------------------------------------------------------
//
void ExEvent::UpdatePseudoJet()
{
  std::vector<int> tmp_rm_lst ;

  for(size_t i=0 ; i < lepton_jets.size() ; ++i )
  {
    if( !contains( isolated_leptons , lepton_jets[i] ) )
    {
      ExParticle p = lepton_jets[i] ;
      fastjet::PseudoJet particle(p.GetMom().px(),p.GetMom().py(),
      p.GetMom().pz(),p.GetMom().e()) ;
      particle.set_user_index(p.chargeType()) ;
      // particle.set_user_index(-50) ;
      pseudo_jet_set.push_back(particle) ;
    }
  }

}

//--------------------------------------------------------------
void ExEvent::Print(std::string filename)
{
  // For saving the full event record in a text file:
  std::FILE * Event_report_file ;
  Event_report_file = fopen(filename.c_str(), "a") ;

  fprintf(Event_report_file, " Event #: %-5d %20s Particles: %-4d\n\n",
   this->i()," ", (int)this->size()-1 ) ;

  for( size_t i=0 ;  i< this->size() ; ++i )
  {
    ExParticle p = full_event[i] ;
    
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
    " ", p.name().c_str()," ", p.status()," ", p.GetMom().e(), " ") ;

    fprintf(Event_report_file, " %8.1f %5s %8.1f %10s", p.GetMom().pT()," ",
     p.GetMom().pz()," ") ;

    fprintf(Event_report_file, " %4d %5s %4d %5s\n", p.daughter1()," ",
     p.daughter2()," ") ;

  }
  fclose(Event_report_file) ;
}

//==============================================================
//                    explicit instantiations
//==============================================================
// print_table
template void ExEvent::PrintTable<ExParticle>(std::FILE * file, std::vector<ExParticle>) ;
template void ExEvent::PrintTable<Pythia8::Particle>(std::FILE * file, std::vector<Pythia8::Particle>) ;

//==============================================================
