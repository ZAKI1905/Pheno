/*
  Basic functions

  Last Updated by Zaki on July 6, 2019

*/

// #define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
// #define PBWIDTH 60

extern const int  ID_ELECTRON  = 11 ;
extern const int  ID_MUON      = 13 ;
extern const int  ID_TAU       = 15 ;

#include <algorithm>
// #include <vector>
#include <random>
#include <chrono>
#include <ctime>
// #include "Pythia8/Pythia.h"
// #include "fastjet/ClusterSequence.hh"
// #include "fastjet/Selector.hh"

#include "../include/Basics.h"

// using    namespace Pythia8 ;

extern const std::vector<int> lept_id_list { ID_ELECTRON, -ID_ELECTRON,
                                ID_MUON, -ID_MUON, ID_TAU, -ID_TAU } ;

//==============================================================
// Random function
double rand01()
{
  return (double)rand()/RAND_MAX ;
}

//==============================================================
bool endswith (std::string const &fullString, std::string const &ending) 
{
  if (fullString.length() >= ending.length())
    return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending)); 
  else
    return false;
}

//==============================================================
// Function to remove occurences of a certain char
// ( e.g. spaces ) from a given string 
std::string strip(std::string str, char c) 
{ 
  std::string out_str ;
  
  for (size_t i = 0; i<str.size(); i++) 
    if (str[i] != c) 
      out_str.push_back(str[i]); 

  return out_str;
} 

//==============================================================
template <class T> 
bool contains( std::vector<T>& Vec, T Element ) 
{
    if (std::find(Vec.begin(), Vec.end(), Element) != Vec.end())
        return true ;

    return false ;
}

//----------------------------------------------------------
// Specialization for particle instances
template <> 
bool contains<ExParticle>( std::vector<ExParticle>& Vec, ExParticle p ) 
{
    
    // Checking if a particle exists in a vector
    bool it_exists = false ;
    for(size_t  i  = 0     ; i < Vec.size() ; ++i)
    {

      if( p==Vec[i] )
      {
          it_exists = true ;
          break ;
      }
    }
    return it_exists ;
}

//==============================================================
// Counts the number of particles with ID's specified in the list
int count( std::vector<ExParticle>& Vec, std::vector<int> id_list ) 
{
  
  int counter = 0 ;

  for ( size_t i=0 ; i < id_list.size() ; ++i )
  {
   counter += std::count_if(Vec.begin(),Vec.end(), 
             [&](ExParticle p) { return (p.id() == id_list[i] ); } ) ;
    
  }

  return counter ;
}

//==============================================================
// List as an argument
template <class T>
void add_elem(std::vector<T>& lst, T ele)
// Add an int elements to a vector of ints, 
// only if it doesn't already exist.
{
    if(!contains(lst, ele))  lst.push_back(ele) ;
}


//==============================================================
template <class T> void rm_elem(std::vector<T>& list, std::vector<int> rm_index_list)
// Removing certain members of a vector, 
// given another vector containing an index set.
{
        int    j = 0 ;
    for(size_t i = 0 ; i < list.size() ; ++i)
    {
        if( contains(rm_index_list, (int) i+j) )
        {
            list.erase(list.begin()+i) ;
            i--                        ;
            j++                        ;
        }
    }
}


//==============================================================
std::string pr(int size, char ch)
// const char* pr(int size, char ch)
{ 
  std::string rep(size,ch) ;
  return rep          ;
}


//==============================================================
/*
  Parses lineIn based on the given delim from the left side,
   for slice_nums times
*/
std::vector<std::string> pars(std::string lineIn, std::string delim, int slice_nums)
{
  // Default token_nums is 1
  std::vector<std::string> tokens ;
  int token_counter = 0 ;
  std::string intermediate ;
  size_t pos = 0 ;

  // Take copy that will be modified.
  std::string line = lineIn ;

  while (  ( token_counter < slice_nums || slice_nums == -1) && (pos) != std::string::npos) 
  {
      pos = line.find(delim) ;  
      intermediate = line.substr(0, pos) ;

      tokens.push_back(intermediate) ;

      line = line.substr( pos + delim.length(), line.size() ) ;

      token_counter++ ;
  }

  if( slice_nums != -1 ) tokens.push_back(line) ;

  return tokens ;
}

//==============================================================
std::vector<std::string> pars(std::string lineIn, std::vector<std::string> delim_lst)
{
  std::vector<std::string> tokens ;
  size_t pos = 0 ;

  // Take copy that will be modified.
  std::string line = lineIn ;
   

  for( size_t i=0 ; i<delim_lst.size() ; ++i)
    {
      pos = line.find(delim_lst[i]) ; 

      if ( (pos) == std::string::npos )
        continue ;

      tokens.push_back( line.substr(0, pos) ) ;
      tokens.push_back( delim_lst[i] ) ;
      tokens.push_back(line.substr( pos + delim_lst[i].length(), line.size() )) ;
    }
   
  return tokens ;
}
//==============================================================
// Takes a string input like "s1,s2,s3,s4" and returns a vector
// {s1,s2,s3,s4}, the delim option is set to "," by default
void stolst(std::vector<std::string>& out_lst, std::string in_lst_str, std::string delim)
{
  // Parsing the input as far as possible (hence the option '-1')
  std::vector<std::string> tmp = pars( in_lst_str, delim, -1 )  ;

  for( size_t i=0 ; i < tmp.size() ; ++i)
  {
    out_lst.push_back( tmp[i] ) ;
  }  
}

//==============================================================
// Takes a string input like "1,2,3,4" and returns a vector {1,2,3,4}
// the delim option is set to "," by default
void stolst(std::vector<int>& out_lst, std::string in_lst_str, std::string delim)
{
  // Parsing the input as far as possible (hence the option '-1')
  std::vector<std::string> tmp = pars( in_lst_str, delim, -1 ) ;

  for( size_t i=0 ; i < tmp.size() ; ++i)
  {
    out_lst.push_back( std::stoi(tmp[i]) ) ;
  }  
}

//==============================================================
// Overloading for float inputs
void stolst(std::vector<float>& out_lst, std::string in_lst_str, std::string delim)
{
// Parsing the input as far as possible (hence the option '-1')
  std::vector<std::string> tmp = pars( in_lst_str, delim, -1 ) ;

  for( size_t i=0 ; i < tmp.size() ; ++i)
  {
    out_lst.push_back( std::stof(tmp[i]) ) ;
  }  
}

//==============================================================
// Event range input parser for inputs like:
//  (...=1-10,50-75,300,401)
std::vector<int> ev_range_pars(std::string input)
{
    std::vector<std::string> pars_pr_ev_set = pars(input, ",", -1) ;
    std::vector<int> ev_set ;

    for (size_t i=0 ; i<pars_pr_ev_set.size() ; ++i)
    {
      std::vector<std::string> tmp_set = pars(pars_pr_ev_set[i], "-");

      int min_ev = std::stoi(tmp_set[0]);
      int max_ev = std::stoi(tmp_set[1]);

      // For inputs that are a range of event, i.e. '...,20-50,...'
      if (min_ev != max_ev)
      {
        for(int j=min_ev ; j<=max_ev; ++j)
          ev_set.push_back(j) ;
      }
      // For inputs that are single event, i.e. '...,50,...' 
      else
        ev_set.push_back(min_ev);
    }

  return ev_set;
}
//==============================================================
// Returns the invariant mass of a list of particles
double invM(std::vector<ExParticle>& prt_lst) 
{
  Pythia8::Vec4 tmp_4Mom = 0.0 ;

  for( size_t i=0 ; i < prt_lst.size() ; ++i ) 
    tmp_4Mom += prt_lst[i].visMom() ;

  return tmp_4Mom.mCalc() ;
}

//==============================================================
// Returns the number of opposite sign same flavor leptons 
// in a list
int OSSF(std::vector<ExParticle>& ev)
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
int OSOF(std::vector<ExParticle>& ev)
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
double R(ExParticle& p1, ExParticle& p2)
{
  return pow((pow((p1.visMom().eta() - p2.visMom().eta()), 2) 
          + pow((p1.visMom().phi() - p2.visMom().phi()),2)),0.5) ;
}

//==============================================================
// Records the input quantity (int, float or double) and saves in
// multiple (# of threads) text files.

//-------------------------------------------------------
template <class T> 
void saveVec(std::vector<T> list, std::string out_file_str)
{
  if (list.size() ==0 ) return ;
  out_file_str = out_file_str + ".dat" ;

  std::FILE * out_file ;
  out_file = fopen(out_file_str.c_str(), "a") ;

  for(size_t i=0 ; i < list.size() ; ++i)
    { fprintf(out_file,"%f\n", list[i]) ; }

  fclose(out_file) ;
}

//-------------------------------------------------------
// Specialization for int instances
template <> 
void saveVec<int>(std::vector<int> list, std::string out_file_str)
{
  if (list.size() ==0 ) return ;

  out_file_str = out_file_str + ".dat" ;

  std::FILE * out_file ;
  out_file = fopen(out_file_str.c_str(), "a") ;

  for(size_t i=0; i < list.size(); ++i)
    { fprintf(out_file,"%d\n", list[i]) ; }

  fclose(out_file) ;
}

//-------------------------------------------------------
// void saveVec(vector<double> list, std::string out_file_str)
// {
//   if (list.size() ==0 ) return ;
//   out_file_str = out_file_str + ".dat" ;

//   std::FILE * out_file ;
//   out_file = fopen(out_file_str.c_str(), "a") ;

//   for(size_t i=0 ; i < list.size() ; ++i)
//     { fprintf(out_file,"%f\n", list[i]) ; }

//   fclose(out_file) ;
// }

//-------------------------------------------------------
// Specialization for string instances
template <> 
void saveVec<std::string>(std::vector<std::string> list, std::string out_file_str)
{
  if (list.size() ==0 ) return ;
  out_file_str = out_file_str + ".dat" ;

  std::FILE * out_file ;
  out_file = fopen(out_file_str.c_str(), "a") ;

  for(size_t i=0 ; i < list.size() ; ++i)
    { fprintf(out_file,"%s\n", list[i].c_str()) ; }

  fclose(out_file) ;
}

//-------------------------------------------------------
// Overloading saveVec for rec_var<T> inputs
template <class T>
void saveVec(std::vector<rec_var<T> > list)
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
void saveVec<std::vector<double> >(std::vector<rec_var<std::vector<double> > > list)
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
void saveVec<int>(std::vector<rec_var<int> > list)
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

// contains
template bool contains<int>(std::vector<int>&, int) ;
// template bool contains<ExParticle>(std::vector<ExParticle>&, ExParticle) ;
template bool contains<std::string>(std::vector<std::string>&, std::string) ;
//--------------------------------

// add_elem
template void add_elem<int>(std::vector<int>&, int) ;
template void add_elem<ExParticle>(std::vector<ExParticle>&, ExParticle) ;
//--------------------------------

// rm_elem
template void rm_elem<int>(std::vector<int>&, std::vector<int>) ;
template void rm_elem<ExParticle>(std::vector<ExParticle>&, std::vector<int>) ;
template void rm_elem<fastjet::PseudoJet>(std::vector<fastjet::PseudoJet>&,
 std::vector<int>) ;
//--------------------------------

// SaveVec
// template void saveVec<int>(std::vector<int>, std::string) ;
template void saveVec<float>(std::vector<float>, std::string) ;
template void saveVec<double>(std::vector<double>, std::string) ;
// template void saveVec<std::string>(std::vector<std::string>, std::string) ;

template void saveVec<double>(std::vector<rec_var<double> >) ;
// template void saveVec<std::vector<double> >(std::vector<rec_var<std::vector<double> > >) ;
// template void saveVec<int>(std::vector<rec_var<int> >) ;
// template void saveVec<float>(std::vector<rec_var<float> >) ;

//==============================================================
