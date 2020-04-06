#ifndef Zaki_Math_Basic_H
#define Zaki_Math_Basic_H

#include <iostream>

//--------------------------------------------------------------
namespace Zaki
{
//--------------------------------------------------------------
namespace Math
{

//==============================================================
struct Quantity
{
    double val, err ;
};

//==============================================================
template <typename T>
struct Range
{
    T min, max ;

    Range() : min(0), max(0) {}
    Range(T in_min, T in_max) : min(in_min), max(in_max) {}

    T Len() { return max - min ; }
    T LenAbs() {return abs(max - min) ; }
};

//==============================================================
struct Axis
{
  Range<double> range ; 
  int   res ;
  std::string scale ;
  double Min() const { return range.min; }
  double Max() const { return range.max; }
};

//==============================================================
struct Grid2D
{
  Axis xAxis ;
  Axis yAxis ;
};

//==============================================================

//--------------------------------------------------------------
} // End of namespace Math
//--------------------------------------------------------------
} // End of namespace Zaki
//--------------------------------------------------------------

std::ostream& operator << ( std::ostream &, Zaki::Math::Quantity);
std::ostream& operator << (std::ostream &, Zaki::Math::Range<double>) ;
std::ostream& operator << (std::ostream &, Zaki::Math::Range<double>) ;
std::ostream& operator << (std::ostream &, Zaki::Math::Range<int>) ;

//==============================================================
#endif /*Zaki_Math_Basic_H*/
