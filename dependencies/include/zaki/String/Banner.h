#ifndef Zaki_String_Banner_H
#define Zaki_String_Banner_H

#include "string"
#include "vector"

#include "TextBox.h"

//--------------------------------------------------------------
namespace Zaki
{
//--------------------------------------------------------------
namespace String
{

//==============================================================
  //--------------------------------------------------------------
  struct Content
  {
    size_t index = 0 ;
    
    Content(size_t idx) : index(idx) {}

    virtual std::string Str() = 0;

    virtual ~Content() {}
  };

  //--------------------------------------------------------------
  struct Version : public Content
  {
    std::string num;
    std::string date;

    // constructor
    Version(const char* in_num, const char* in_date, size_t idx=0) 
      :Content(idx), num(in_num), date(in_date) {}

    std::string Str() override 
    {
      return "Version " + num + "         " + date ;
    }

  };

  //--------------------------------------------------------------
  struct Website : public Content
  {
    std::string label;
    std::string link ;

    // constructor
    Website(const char* in_lab, const char* in_link, size_t idx=0) 
      : Content(idx), label(in_lab), link(in_link){}

    std::string Str() override 
    {
      return label + ": " + link ;
    }
  };

  //--------------------------------------------------------------
    struct Author : public Content
  {
    std::string first_name;
    std::string last_name ;

    // constructor
    Author(const char* in_first, const char* in_last, size_t idx=0) 
      : Content(idx), first_name(in_first), last_name(in_last) {}

    std::string Str() override 
    {
      return "Created by " + ToString(first_name[0]) + ". " + last_name ;
    }
  };

  //--------------------------------------------------------------
  struct ProgramName : public Content
  {
    std::string name;

    // constructor
    ProgramName(const char* in_name, size_t idx=0) 
      : Content(idx), name(in_name){}


    std::string Str() override 
    {
      return "Welcome to " + name ;
    }
  };

  //--------------------------------------------------------------
    struct Misc : public Content
  {
    std::string item;

    // constructor
    Misc(const char* in_item, size_t idx=0) 
      :Content(idx), item(in_item) { }


    std::string Str() override 
    {
      return item ;
    }
  };

  //------------------------------------------------------------
  class Banner
  {

  public:
    Banner();
    Banner(ProgramName*, Version*, Author*, Website*);

    // void SetName(const char*) ;
    // void SetAuthor(const char*) ;
    // void SetVersion(const Version) ;
    // void SetWebsite(const Website) ;
    void AddContent(Content*) ;
    void SortContents() ;

    TextBox* GetTextBox() ;
    void Show() ;

  private:
    // std::string name ;
    TextBox text_box ;  
    // Version version ;
    // std::string author ;
    // std::vector<std::string> extra_txt ;
    // Website website ;
    std::vector<Content*> contents;
  };
  //------------------------------------------------------------

//--------------------------------------------------------------
} // End of namespace String
//--------------------------------------------------------------
} // End of namespace Zaki
//--------------------------------------------------------------

//==============================================================
#endif /*Zaki_String_Banner_H*/