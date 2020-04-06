#ifndef Zaki_Physics_Coordinate_H
#define Zaki_Physics_Coordinate_H

// Root
#include <Math/Vector3D.h>

#include "DateTime.h"
#include "Constants.h"

//--------------------------------------------------------------
namespace Zaki
{
//--------------------------------------------------------------
namespace Physics
{

//==============================================================
struct Coord3D
{
    double x,y,z ;

    Coord3D() {}
    Coord3D(double in_x, double in_y, double in_z) : x(in_x), y(in_y), z(in_z) {}

    bool operator==(const Coord3D& rhs) const
    {
        return ( x == rhs.x && y == rhs.y && z == rhs.z ) ;
    }

    bool operator>(const Coord3D& rhs) const
    {
        return ( XYDist2({0,0,0}) > rhs.XYDist2({0,0,0})) ;
    }

    bool operator<(const Coord3D& rhs) const
    {
        return ( XYDist2({0,0,0}) < rhs.XYDist2({0,0,0})) ;
    }

    double XYDist2(const Coord3D& rhs) const
    {
        return (x - rhs.x)*(x - rhs.x)
                + (y - rhs.y)*(y - rhs.y) ;
    }
  // coords operator+(const coords& rhs) const
  // {
  //   return {x + rhs.x, y + rhs.y, z + rhs.z } ;
  // }

  // coords operator-(const coords& rhs) const
  // {
  //   return {x - rhs.x, y - rhs.y, z - rhs.z } ;
  // }
  
  // coords operator*(const double& fac) const
  // {
  //   return {fac*x, fac*y, fac*z } ;
  // }
};

//==============================================================
// Yaw, Pitch, Roll Coordinates
struct YPR
{
  double Yaw, Pitch, Roll ;

  YPR Rad() const
  {
    return {Yaw*DEG_2_RAD, Pitch*DEG_2_RAD, Roll*DEG_2_RAD} ;
  }
};

//==============================================================
// Geocentric Equatorial Inertial("GEI") Coordinate
struct EclipticCoord
{
    double lambda, beta, r;
};

//==============================================================
// Geocentric Equatorial Inertial("GEI") Coordinate
struct GEICoord
{
    // Rectangular
    ROOT::Math::XYZVector xyz ;

    // .....................................
    // Operators
    bool operator==(const GEICoord& other)
    { return xyz == other.xyz; }

    GEICoord operator+(const GEICoord& other)
    { return {xyz + other.xyz}; }

    GEICoord operator-(const GEICoord& other)
    { return {xyz - other.xyz}; }
    GEICoord operator*(const int& other)
    { return {other*xyz}; }
    GEICoord operator*(const double& other)
    { return {other*xyz}; }
    GEICoord operator-()
    { return {-xyz}; }
    // .....................................

    // .....................................
    // Spherical
    double r();  // { return sqrt(xyz.mag2()) ;}
    double alpha(); //  { return RAD_2_DEG*atan2(xyz.y(), xyz.x()) ;}
    double delta();  // { return RAD_2_DEG*asin(xyz.z() / r()) ;} 
    // .....................................

    // Convert to Ecliptic coordinates
    EclipticCoord ToEcliptic(Zaki::Physics::Date) ;

    // overloaded for UnixTDays input
    EclipticCoord ToEcliptic(double ) ;
};

//==============================================================
// The obliquity of the ecliptic (in degrees)
double EclipticObliquity(Zaki::Physics::Date) ;
// Overloading for UnixTDay input
double EclipticObliquity(double) ;


//==============================================================
//                  ostream << overloading
//==============================================================
std::ostream& operator << ( std::ostream &output, YPR );
std::ostream& operator << (std::ostream &, Coord3D) ;
std::ostream& operator << (std::ostream &output, GEICoord) ;
std::ostream& operator << (std::ostream &output, EclipticCoord) ;
//==============================================================


//--------------------------------------------------------------
} // End of namespace Physics
//--------------------------------------------------------------
} // End of namespace Zaki
//--------------------------------------------------------------

//==============================================================
#endif /*Zaki_Physics_Coordinate_H*/
