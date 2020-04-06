#ifndef Prog_H
#define Prog_H

#include <zaki/Util/Logger.h>
#include <zaki/Util/Instrumentor.h>
// #include <zaki/Util/Simple_Timer.h>

#include "stdio.h"

//--------------------------------------------------------------
namespace PHENO
{
//==============================================================
class Prog
{
  //--------------------------------------------------------------
  protected:
    void ShowBanner();

    static size_t counter;
    // Zaki::Util::Logger logger;
    // std::string wrk_dir_str = "" ;
    // std::string name        = "" ;

    // Flags
    // bool set_name_flag = false    ;
    // bool set_wrk_dir_flag = false ;

  //--------------------------------------------------------------
  public:
    // Constructor
    Prog() ;

    // Destructor
    virtual ~Prog() ;

    // Setters
    // virtual void SetWrkDir(const char*) ;
    // virtual void SetName(const char*) ;

    // Getters
    // virtual std::string GetWrkDir() const ;
    // virtual std::string GetName() const   ;
    // virtual void Print() const            ;

};

//==============================================================
} // PHENO namespace
//=============================================================
#endif /*Prog_H*/