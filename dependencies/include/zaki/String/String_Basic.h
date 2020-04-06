#ifndef Zaki_String_Basic_H
#define Zaki_String_Basic_H

#include "string"
#include "sstream"
#include "vector"

//--------------------------------------------------------------
namespace Zaki
{
//--------------------------------------------------------------
namespace String
{

//==============================================================
template <class T>
std::string ToString(T a) 
{
  std::stringstream buff;
  buff << a;
  return buff.str();
}

//==============================================================
// Checks if a string ends in another string
bool EndsWith(std::string const &fullString, std::string const &ending) ;

//==============================================================
// returns a copy of str by stripping character c from it
std::string Strip(const std::string& str, char c) ;

//==============================================================
// Parses the input based on the given delim
std::vector<std::string> Pars(const std::string&, const char*, int=1) ;
// Parses the input based on the given delim list
std::vector<std::string> Pars(const std::string&, const std::vector<std::string>&) ;

//==============================================================
// converts a string list of numbers separated by delim (default = ",")
// to a vector of numbers
void Str2Lst(std::vector<int>&, const std::string&, const char* =",") ;
// Overloading for float numbers
void Str2Lst(std::vector<float>&, const std::string&, const char* =",") ;
// overloading for string lists
void Str2Lst(std::vector<std::string>&, const std::string&, const char* =",") ;

//==============================================================
// Integer range input parser for inputs like:
//  (...=1-10,50-75,300,401)
std::vector<int> RangeParser(const std::string&) ;

//==============================================================
// Copying a single character multiple (n) times
std::string Multiply(const char, size_t) ;

// Copying a character multiple (n) times
std::string Multiply(const char*, size_t) ;

//==============================================================
enum class FGColor
{
  Black, Red, Green, Yellow, Blue, Purple, Cyan, White,
  LBlack, LRed, LGreen, LYellow, LBlue, LPurple, LCyan, LWhite
};

//==============================================================
enum class BGColor
{
  BlackBg, RedBg, GreenBg, YellowBg, BlueBg, PurpleBg, CyanBg, WhiteBg
};

//==============================================================
struct Color
{
  FGColor Foreground;
  BGColor Background;

  // Constructor 1:
  Color(FGColor fg, BGColor bg) 
    : Foreground(fg), Background(bg) {}

  // Constructor 2:
  Color(FGColor fg) 
    : Foreground(fg), Background(BGColor::BlackBg) {}

  // Constructor 3:
  Color() 
    : Foreground(FGColor::White), Background(BGColor::BlackBg) {}
  
  // String form of the color for '<<'
  std::string Str() ;

  private:
    std::string str_form = "" ;
};

//==============================================================
// std::string ColorDict(Color) ;

//==============================================================
int FindOccurrences(const std::string&, const std::string&) ;

//==============================================================
//                  ostream << overloading
std::ostream& operator << (std::ostream &output, Zaki::String::Color) ;

//==============================================================

//--------------------------------------------------------------
} // End of namespace String
//--------------------------------------------------------------
} // End of namespace Zaki
//--------------------------------------------------------------

//==============================================================
#endif /*Zaki_String_Basic_H*/