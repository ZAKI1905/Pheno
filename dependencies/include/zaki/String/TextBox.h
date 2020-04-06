#ifndef Zaki_String_TextBox_H
#define Zaki_String_TextBox_H

#include "sstream"
#include "vector"

#include "String_Basic.h"

//--------------------------------------------------------------
namespace Zaki
{
//--------------------------------------------------------------
namespace String
{
//==============================================================
class TextBox
  {
  
  public:
    // Constructor 1
    TextBox();
    // Constructor 2
    TextBox(const std::vector<std::string>&);

    enum TextAlignment
    {
      left = 0, center = 1, right = 2
    };

    void SetText(const std::vector<std::string>&) ;
    void AddText(const char*) ;
    void AddText(const std::vector<std::string>&) ;

    void SetAlignment(const TextAlignment) ;
    void SetFrameColor(const Color&) ;
    void SetTextColor(const Color&) ;
    void SetPadColor(const Color&) ;
    void SetPadLeft(size_t) ;
    void SetPadRight(size_t) ;
    void SetPadding(size_t, int = -1) ;

    void Print() ;
    void MakeFrame() ;

  private:
    const char* top_left_corn = "\u2554";
    const char* top_right_corn = "\u2557";
    const char* bottom_left_corn = "\u255A";
    const char* bottom_right_corn = "\u255D";
    const char* horizontal_line = "\u2550";
    const char* vertical_line = "\u2551";

    Color reset_color; // "\E[0m"

    Color frame_color;
    Color text_color;
    Color pad_color;


    size_t height = 0, width = 0, l_pad = 1, r_pad = 1 ;

    TextAlignment text_align = left ;

    std::stringstream top_frame, mid_section, bottom_frame;
    std::vector<std::string> text ;

    // std::string Multiply(const char*, size_t) ;
    size_t CorrectEncodedChars(const std::string&) ;

  };
  //------------------------------------------------------------

//--------------------------------------------------------------
} // End of namespace String
//--------------------------------------------------------------
} // End of namespace Zaki
//--------------------------------------------------------------

//==============================================================
#endif /*Zaki_String_TextBox_H*/