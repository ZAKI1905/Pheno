#ifndef Basics_H
#define Basics_H


extern const int  ID_ELECTRON  ;
extern const int  ID_MUON      ; 
extern const int  ID_TAU       ;  

// #include <zaki/Util/Logger.h>
// #include <zaki/Util/Instrumentor.h>
// #include <zaki/Vector/Vector_Basic.h>

// Local headers
#include "ExParticle.h"

//--------------------------------------------------------------
namespace PHENO
{
//--------------------------------------------------------------

extern const std::vector<int> lept_id_list ;

//==============================================================
// Random function
double rand01() ;

//==============================================================
// Checks if a string ends in another string
// bool endswith (std::string const &fullString, std::string const &ending) ;

//==============================================================
// strips string from character c
// std::string strip(std::string str, char c) ;

//==============================================================
// // Checks if an element exists in the list.
// template <class T>
// bool contains(std::vector<T>& , T) ;

// //==============================================================
// // Adds element to a list if it doesn't already exists.
// template <class T>
// void add_elem(std::vector<T>& , T) ;

// //==============================================================
// // Removes a list of int elements from a T type list
// template <class T> 
// void rm_elem(std::vector<T>&, std::vector<int>) ;

//==============================================================
// Counts the number of particles with id's in id_list
int count(std::vector<ExParticle>&, std::vector<int> ) ;

//==============================================================
// Prints "ch" for "size" times.
// std::string pr(int size, char ch) ;

//==============================================================
// // Parses the input based on the given delim
// std::vector<std::string> pars(std::string, std::string, int=1) ;
// // Parses the input based on the given delim list
// std::vector<std::string> pars(std::string, std::vector<std::string>) ;
// //==============================================================
// // converts a string list of numbers separated by delim (default = ",")
// // to a vector of numbers
// void stolst(std::vector<int>&, std::string, std::string=",") ;
// // Overloading for float numbers
// void stolst(std::vector<float>&, std::string, std::string=",") ;
// // overloading for string lists
// void stolst(std::vector<std::string>&, std::string, std::string=",") ;

// //==============================================================
// // Event range input parser for inputs like:
// //  (...=1-10,50-75,300,401)
// std::vector<int> ev_range_pars(std::string) ;

//==============================================================
// Returns the invariant mass of two particles
double invM(std::vector<ExParticle>& prt_lst) ;

//==============================================================
// Returns the distance between two particles
double R(ExParticle& p1, ExParticle& p2) ;

//==============================================================
// Returns the number of opposite sign same flavor in a list
int OSSF(std::vector<ExParticle>&) ;

//==============================================================
// Returns the number of opposite sign opposite flavor leptons
// in a list
int OSOF(std::vector<ExParticle>&) ;

//==============================================================
template <typename T>
struct rec_var 
{
std::string name = "" ;
T val ;
};

//==============================================================
// Saves any vector quantity to a file
// template <class T> 
// void saveVec(std::vector<T>, std::string) ;

template <class T> 
void SaveVec(std::vector<rec_var<T> >) ;
//==============================================================
} // Pheno namespace
//=============================================================
#endif /*Basics_H*/
