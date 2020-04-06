/*
  Basic functions

  Last Updated by Zaki on July 6, 2019

*/

extern const int  ID_ELECTRON  = 11 ;
extern const int  ID_MUON      = 13 ;
extern const int  ID_TAU       = 15 ;

#include <algorithm>
#include <random>
#include <chrono>
#include <ctime>

#include "../include/Basics.h"

extern const std::vector<int> PHENO::lept_id_list { ID_ELECTRON, -ID_ELECTRON,
                                ID_MUON, -ID_MUON, ID_TAU, -ID_TAU } ;

//==============================================================
// Random function
double PHENO::rand01()
{
  return (double)rand()/RAND_MAX ;
}

//==============================================================
// Counts the number of particles with ID's specified in the list
int PHENO::count( std::vector<PHENO::ExParticle>& Vec, std::vector<int> id_list ) 
{
  
  int counter = 0 ;

  for ( size_t i=0 ; i < id_list.size() ; ++i )
  {
   counter += std::count_if(Vec.begin(),Vec.end(), 
             [&](PHENO::ExParticle p) { return (p.id() == id_list[i] ); } ) ;
    
  }

  return counter ;
}

//==============================================================
// Returns the invariant mass of a list of particles
double PHENO::invM(std::vector<PHENO::ExParticle>& prt_lst) 
{
  Pythia8::Vec4 tmp_4Mom = 0.0 ;

  for( size_t i=0 ; i < prt_lst.size() ; ++i ) 
    tmp_4Mom += prt_lst[i].GetVisMom() ;

  return tmp_4Mom.mCalc() ;
}

//==============================================================
// Returns the number of opposite sign same flavor leptons 
// in a list
int PHENO::OSSF(std::vector<PHENO::ExParticle>& ev)
{
  int OSSF_N = 0 ;

  for( size_t i = 0 ; i < ev.size() ; ++i )

    // If the i-th particle is final, and a charged lepton
    if( ev[i].isFinal() && ev[i].isLepton() && ev[i].isCharged() )

      for(size_t j = i ; j < ev.size(); ++j)

        // If the j-th particle is also final
        if( ev[j].isFinal() )

          // If the i-th and j-th leptons are OSSF
          if( ev[i].id() == - ev[j].id() )
            OSSF_N++ ;

  return OSSF_N ;
}

//==============================================================
// Returns the number of opposite sign opposite flavor leptons
// in a list
int PHENO::OSOF(std::vector<PHENO::ExParticle>& ev)
{
  int OSOF_N = 0 ;

  for( size_t i = 0 ; i < ev.size() ; ++i )

    // If the i-th particle is final, and a charged lepton
    if( ev[i].isFinal() && ev[i].isLepton() && ev[i].isCharged() )

      for(size_t j = i ; j < ev.size(); ++j)

        // If the j-th particle is also final
        if( ev[j].isFinal() )

          // If the i-th and j-th leptons are OS & OF
          if( ev[i].id()*ev[j].id() < 0 &&  ev[i].idAbs() != ev[j].idAbs() )
            OSOF_N++ ;

  return OSOF_N ;
}

//==============================================================
// Returns the distance between two particles
double PHENO::R(PHENO::ExParticle& p1, PHENO::ExParticle& p2)
{
  return pow((pow((p1.GetVisMom().eta() - p2.GetVisMom().eta()), 2) 
          + pow((p1.GetVisMom().phi() - p2.GetVisMom().phi()),2)),0.5) ;
}

//==============================================================
//            SaveVec for recorder values
//-------------------------------------------------------
// Overloading SaveVec for rec_var<T> inputs
template <class T>
void PHENO::SaveVec(std::vector<PHENO::rec_var<T> > list)
{
  if (list.size() ==0 ) return ;
  std::string out_file_str = list[0].name + ".dat" ;

  std::FILE * out_file ;
  out_file = fopen(out_file_str.c_str(), "a") ;

  for(size_t i=0 ; i < list.size() ; ++i)
    { 
      fprintf(out_file,"%f\n", list[i].val) ;
    }

  fclose(out_file) ;
}

//-------------------------------------------------------
// Specialization for double vector instances
template <> 
void PHENO::SaveVec<std::vector<double> >
(std::vector<PHENO::rec_var<std::vector<double> > > list)
{
  if (list.size() ==0 ) return ;

  std::string out_file_str = list[0].name + ".dat" ;

  std::FILE * out_file ;
  out_file = fopen(out_file_str.c_str(), "a") ;

  for(size_t i=0; i < list.size(); ++i)
  { 
    for(size_t j=0; j < list[i].val.size(); ++j)
      fprintf(out_file,"%f ", list[i].val[j]) ; 
    
    fprintf(out_file,"\n");
  }

  fclose(out_file) ;
}

//-------------------------------------------------------
// Specialization for int instances
template <>
void PHENO::SaveVec<int>(std::vector<PHENO::rec_var<int> > list)
{
  if (list.size() ==0 ) return ;
  std::string out_file_str = list[0].name + ".dat" ;

  std::FILE * out_file ;
  out_file = fopen(out_file_str.c_str(), "a") ;

  for(size_t i=0 ; i < list.size() ; ++i)
    { 
      fprintf(out_file,"%d\n", list[i].val) ;
    }

  fclose(out_file) ;
}
//==============================================================

//==============================================================
//                    explicit instantiations
//==============================================================
template void PHENO::SaveVec<double>(std::vector<PHENO::rec_var<double> >) ;

//==============================================================
