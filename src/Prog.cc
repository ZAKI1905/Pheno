/*
  Prog class

*/

// Creating directory
// #include <sys/stat.h>

#include <zaki/String/Banner.h>

#include "../include/Prog.h"

//==============================================================
size_t PHENO::Prog::counter = 0 ;
//==============================================================

//--------------------------------------------------------------
PHENO::Prog::Prog(/* args */)
{

  // logger.SetLevel(Zaki::Logger::LevelInfo) ;

  if(counter==0)
    ShowBanner() ;

  counter++ ;
}

//--------------------------------------------------------------
PHENO::Prog::~Prog()
{
}

//--------------------------------------------------------------
void PHENO::Prog::ShowBanner()
{
  using namespace Zaki::String ;

  Banner banner ;

  ProgramName p_name("Pheno", 1) ;
  banner.AddContent(&p_name) ;

  Author auth("Mohammadreza", "Zakeri", 4) ;
  banner.AddContent(&auth) ;

  Version ver("1.1.0", "Apr 6, 2020", 2);
  banner.AddContent(&ver) ;

  Website web("GitHub", "github.com/ZAKI1905/Pheno", 5) ;
  banner.AddContent(&web) ;

  Misc misc("A package for basic collider analysis", 3); 
  banner.AddContent(&misc) ;

  banner.GetTextBox()->SetTextColor({FGColor::LCyan, BGColor::BlackBg}) ;
  banner.GetTextBox()->SetFrameColor({FGColor::LYellow, BGColor::BlackBg}) ;
  banner.GetTextBox()->SetPadColor({FGColor::LCyan, BGColor::BlackBg}) ;

  banner.GetTextBox()->SetAlignment(TextBox::center) ;
  banner.GetTextBox()->SetPadding(5) ;

  banner.Show() ;

}
//--------------------------------------------------------------
// void PHENO::Prog::SetWrkDir(const char* input) 
// {
//   wrk_dir_str = input;
//   set_wrk_dir_flag = true ;

//   // ............ Creating a directory ............
//   if (mkdir(wrk_dir_str.c_str(), ACCESSPERMS) == -1) 
//   {
//     char tmp[75] ;
//     sprintf(tmp, "Directory '%s' wasn't created, because: %s.", wrk_dir_str.c_str(), strerror(errno)) ;
//     Z_LOG_WARNING(tmp) ; 
//   }
//   else
//     Z_LOG_INFO(("Directory '" + wrk_dir_str + "' created.").c_str()); 
//   // .................................................

//   char tmp[75];
//   sprintf(tmp, "Work directory set to '%s'.", input) ;
//   Z_LOG_INFO(tmp) ;
// }

//--------------------------------------------------------------
// void PHENO::Prog::SetName(const char* input) 
// {
//   name          = input;
//   set_name_flag = true ;
//   char tmp[75];
//   sprintf(tmp, "Name set to '%s'.", input) ;
//   Z_LOG_INFO(tmp) ;
// }

//--------------------------------------------------------------
// std::string PHENO::Prog::GetName() const 
// {
//   if (!set_name_flag)
//     Z_LOG_ERROR("Name not set!") ;
//   return name;
// }

//--------------------------------------------------------------
// std::string PHENO::Prog::GetWrkDir() const 
// {
//   if (!set_wrk_dir_flag)
//     Z_LOG_ERROR("Work directory not set!") ;

//   return wrk_dir_str;
// }

//--------------------------------------------------------------
// void PHENO::Prog::Print() const
// {
//   std::cout<<"\n --------------------------------------------------------------\n" ;
//   std::cout<<"|         Name: "<<GetName() << "\n";
//   std::cout<<" --------------------------------------------------------------\n" ;
// }

//==============================================================
