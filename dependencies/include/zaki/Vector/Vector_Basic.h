#ifndef Zaki_Vector_Basic_H
#define Zaki_Vector_Basic_H

// Functions for vector manipulations
// Functions for vector manipulations
#include <vector>

//==============================================================
namespace Zaki
{
//--------------------------------------------------------------
namespace Vector
{

//==============================================================
// Checks if an element exists in the list.
template <class T> 
bool Exists(T Element, const std::vector<T>& Vec) 
{
    if (std::find(Vec.begin(), Vec.end(), Element) != Vec.end())
        return true ;

    return false ;
}

//==============================================================

//==============================================================
// Adds element to a list if it doesn't already exists.
template <class T>
void Add(std::vector<T>& lst, T ele)
// Add an element to a vector, 
// only if it doesn't already exist.
{
    if(!Exists(ele ,lst))  lst.push_back(ele) ;
}

//==============================================================
// Removes a list of elements from a T type list
template <class T> 
void Remove(std::vector<T>& list, std::vector<int> rm_index_list)
// Removing certain members of a vector, 
// given another vector containing an index set.
{
        int    j = 0 ;
    for(size_t i = 0 ; i < list.size() ; ++i)
    {
        if( Exists((int) i+j, rm_index_list))
        {
            list.erase(list.begin()+i) ;
            i--                        ;
            j++                        ;
        }
    }
}

//==============================================================

//--------------------------------------------------------------
} // End of namespace Vector
//--------------------------------------------------------------
} // End of namespace Zaki
//==============================================================

#endif /*Zaki_Vector_Basic_H*/
