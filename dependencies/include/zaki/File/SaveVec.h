#ifndef Zaki_File_SaveVec_H
#define Zaki_File_SaveVec_H

#include <vector>

//--------------------------------------------------------------
namespace Zaki
{
//--------------------------------------------------------------
namespace File
{
//==============================================================
// Saves any vector quantity to a file
template <class T> 
void SaveVec(const std::vector<T>&, const std::string&, const char* mode = "w") ;

template <class T> 
void SaveVec(const std::vector<T>&, const std::string&, const std::string&, const char* mode= "w") ;
//==============================================================

//--------------------------------------------------------------
} // End of namespace File
//--------------------------------------------------------------
} // End of namespace Zaki
//--------------------------------------------------------------

//==============================================================
#endif /*Zaki_File_SaveVec_H*/
