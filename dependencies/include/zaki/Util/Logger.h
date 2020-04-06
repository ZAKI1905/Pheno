#ifndef Zaki_Util_Logger_H
#define Zaki_Util_Logger_H

#include <iostream>
#include <string>
#include <fstream>
#include <mutex>

#include "../String/String_Basic.h"

//--------------------------------------------------------------
namespace Zaki
{
//--------------------------------------------------------------
namespace Util
{

// Forward declration
struct LogEntry ;

//==============================================================
enum class LogLevel
{
  ZError = 0, ZWarning, ZInfo    
} ;

//==============================================================
// struct LogSettings
// {
//   LogLevel Level ;
//   Zaki::String::Color InfoTxtColor;
//   Zaki::String::Color InfoTagColor;
//   Zaki::String::Color WarnTxtColor;
//   Zaki::String::Color WarnTagColor;
//   Zaki::String::Color ErrTxtColor;
//   Zaki::String::Color ErrTagColor;
//   Zaki::String::Color NoColor;
//   std::string LogFile = "";


//   // Constructor 1:
//   LogSettings() 
//     : Level(Zaki::Util::LogLevel::ZInfo),
//     InfoTxtColor(Zaki::String::FGColor::LCyan),
//     InfoTagColor(Zaki::String::FGColor::Cyan),
//     WarnTxtColor(Zaki::String::FGColor::LYellow),
//     WarnTagColor(Zaki::String::FGColor::Yellow),
//     ErrTxtColor(Zaki::String::FGColor::LRed),
//     ErrTagColor(Zaki::String::FGColor::Red) {}

//   // Constructor 2:
//   LogSettings(Zaki::Util::LogLevel in_lvl) 
//     : Level(in_lvl), InfoTxtColor(Zaki::String::FGColor::LCyan),
//     InfoTagColor(Zaki::String::FGColor::Cyan),
//     WarnTxtColor(Zaki::String::FGColor::LYellow),
//     WarnTagColor(Zaki::String::FGColor::Yellow),
//     ErrTxtColor(Zaki::String::FGColor::LRed),
//     ErrTagColor(Zaki::String::FGColor::Red) {}

//   void SetLogFile(const char* in_str) { LogFile = in_str; }

//   // Getters
//   std::pair<Zaki::String::Color, Zaki::String::Color> GetInfoColor() ;
//   std::pair<Zaki::String::Color, Zaki::String::Color> GetWarnColor() ;
//   std::pair<Zaki::String::Color, Zaki::String::Color> GetErrColor() ;

// };

//==============================================================
// Singleton log manager class
class LogManager 
{

  public:

    // Destructor
    ~LogManager() ;

    // Copy constructor
    LogManager(const LogManager&) = delete ;

    static LogManager& Get();

    static void Emit(const LogEntry&);

    // Setters
    // static void SetSettings(const LogSettings&);
    static void SetLogLevels(const LogLevel&, const LogLevel&) ;
    static void SetLogFile(const char*) ;

  private:
    // Constructor 1 with the default settings
    LogManager() { }

    void IEmit(const LogEntry&) ;
    void Print(const LogEntry&) ;
    void ShowReport() ;
    void AddFooter() ;
    void Export(const LogEntry&) ;
    std::string GetLogFileHeader() ;

    std::string file_name;
    LogLevel file_thresh = LogLevel::ZInfo ;
    LogLevel term_thresh = LogLevel::ZInfo ;
    std::mutex mtx;
    
    bool export_flag = false ;
    bool first_time_writing = true ;

    // LogSettings settings ; 
    int num_warnings = 0;
    int num_errors  = 0;
};

//==============================================================
struct LogFormat 
{
  LogLevel level ;
  std::string tag ;
  Zaki::String::Color tag_color, txt_color;

  // Default Constructor
  // LogFormat() ;

  // Constructor from LogLevel
  LogFormat(const LogLevel& in_lvl) 
    : level(in_lvl)
  {
    switch (in_lvl)
    {
    case LogLevel::ZInfo:
        tag = "Info" ;
        tag_color = {Zaki::String::FGColor::Cyan} ;
        txt_color = {Zaki::String::FGColor::LCyan} ;
      break;

    case LogLevel::ZWarning:
        tag = "Warning" ;
        tag_color = {Zaki::String::FGColor::Yellow} ;
        txt_color = {Zaki::String::FGColor::LYellow} ;
      break;

    case LogLevel::ZError:
        tag = "Error" ;
        tag_color = {Zaki::String::FGColor::Red} ;
        txt_color = {Zaki::String::FGColor::LRed} ;
      break;

    default:
      break;
    }
  }

  static std::string ResetColor() 
  {
    return "\E[0m" ;
  }

};

//==============================================================
struct LogEntry
{
  friend class LogManager ;

  private:
    std::string Message ;
    int Line = 0 ;
    std::string FuncName ;
    std::string FileName ;
    // LogLevel Level;
    LogFormat Format;

  public:
    // Constructor
    LogEntry(const char* in_mes, int in_line,
             const char* in_func, const char* in_file, LogLevel in_lev)
             : Message(in_mes), Line(in_line), FuncName(in_func),
               FileName(in_file), Format(in_lev)  
               {
                 LogManager::Emit(*this) ;
               }
} ;

//==============================================================

} // End of namespace Util
//--------------------------------------------------------------
} // End of namespace Zaki
//==============================================================
//                     Logging
#define Z_LOG(MSG, LEVEL)  Zaki::Util::LogEntry(MSG,  __LINE__, __func__, __FILE__, LEVEL)

#define Z_LOG_INFO(MSG)  Z_LOG(MSG, Zaki::Util::LogLevel::ZInfo)
#define Z_LOG_WARNING(MSG)  Z_LOG(MSG, Zaki::Util::LogLevel::ZWarning)
#define Z_LOG_ERROR(MSG)  Z_LOG(MSG, Zaki::Util::LogLevel::ZError)

//==============================================================
#endif /*Zaki_Util_Logger_H*/