#ifndef Logger_H
#define Logger_H

#include "iostream"
#include "string"

//==============================================================
class Logger {

  //--------------------------------------------------------------
  public:

    enum Level
    {
      LevelError = 0, LevelWarning, LevelInfo    
    } ;

    const char* InfoColor    = "\033[1;36m" ;
    const char* WarningColor  = "\033[1;33m" ;
    const char* ErrorColor     = "\033[1;31m" ;

  //--------------------------------------------------------------
  private:
    Level m_LogLevel = LevelInfo ;
    std::string m_UnitName = ""  ;

  //--------------------------------------------------------------
  public:
    // Constructor
    Logger()       ;

    void SetUnit(const char* ) ;
    void SetLevel(Level)      ;

    void Info(const char* , const char*  = "")  const  ;
    void Warn(const char* , const char*  = "")  const  ;
    void Error(const char* , const char* = "") const  ;
};

//==============================================================
#endif /*Logger_H*/