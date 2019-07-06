#ifndef Basics_H
#define Basics_H


extern const int  ID_ELECTRON  ;
extern const int  ID_MUON      ; 
extern const int  ID_TAU       ;  

#include <vector>
#include "Pythia8/Pythia.h"
#include "ExParticle.h"

using namespace Pythia8 ;

extern const std::vector<int> lept_id_list ;

//==============================================================
// Random function
double rand01() ;

//==============================================================
// Checks if an element exists in the list.
template <class T>
bool contains(vector<T>& , T) ;

//==============================================================
// Adds element to a list if it doesn't already exists.
template <class T>
void add_elem(vector<T>& , T) ;

//==============================================================
// Removes a list of int elements from a T type list
template <class T> 
void rm_elem(vector<T>&, vector<int>) ;

//==============================================================
// Counts the number of particles with id's in id_list
int count( vector<ExParticle>&, vector<int> ) ;

//==============================================================
// Prints "ch" for "size" times.
string pr(int size, char ch) ;

//==============================================================
// Parses the input based on the given delim
vector<std::string> pars(std::string, std::string, int=1) ;
// Parses the input based on the given delim list
vector<std::string> pars(std::string, vector<std::string>) ;
//==============================================================
// converts a string list of numbers separated by delim (default = ",")
// to a vector of numbers
void stolst(vector<int>&, std::string, std::string=",") ;
// Overloading for float numbers
void stolst(vector<float>&, std::string, std::string=",") ;
// overloading for string lists
void stolst(vector<std::string>&, std::string, std::string=",") ;
//==============================================================
// Returns the invariant mass of two particles
double invM(vector<ExParticle>& prt_lst) ;

//==============================================================
// Returns the distance between two particles
double R(ExParticle& p1, ExParticle& p2) ;

//==============================================================
// Returns the number of opposite sign same flavor in a list
int OSSF(vector<ExParticle>&) ;

//==============================================================
// Returns the number of opposite sign opposite flavor leptons
// in a list
int OSOF(vector<ExParticle>&) ;

//==============================================================
struct rec_var 
{
std::string name = "" ;
double val = 0 ;
};

//==============================================================
// Saves any vector quantity to a file
void saveVec(vector<int>, std::string) ;
void saveVec(vector<float>, std::string) ;
void saveVec(vector<double>, std::string) ;
void saveVec(vector<rec_var>) ;
//==============================================================

#endif /*Basics_H*/
