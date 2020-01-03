/*
  Logger class

*/

#include "../include/Logger.h"

//==============================================================
//--------------------------------------------------------------
// Constructor
Logger::Logger() { }

//--------------------------------------------------------------
// Setting the unit name
void Logger::SetUnit(const char* unit_input) 
{
  m_UnitName = unit_input ;
}

//--------------------------------------------------------------
void Logger::SetLevel(Logger::Level level)    
{
  m_LogLevel = level ;
}

//--------------------------------------------------------------
void Logger::Info(const char* str, const char* func)  const
{
  if ( m_LogLevel >= LevelInfo )
  {
    char tmp[200] ;
    sprintf(tmp, "%sInfo in %s:%s: %s\033[0m\n", InfoColor, m_UnitName.c_str(), func, str) ;
    std::cout<< tmp << std::flush ; 
  }
}

//--------------------------------------------------------------
void Logger::Warn(const char*  str, const char* func) const
{
  if ( m_LogLevel >= LevelWarning )
  {
    char tmp[200] ;
    sprintf(tmp, "%sWarning in %s:%s: %s\033[0m\n", WarningColor, m_UnitName.c_str(), func, str) ;
    std::cout<< tmp << std::flush ; 
  }
}

//--------------------------------------------------------------
void Logger::Error(const char* str, const char* func) const
{
  if ( m_LogLevel >= LevelError )
  {
    char tmp[200] ;
    sprintf(tmp, "%sError in %s:%s: %s\033[0m\n", ErrorColor, m_UnitName.c_str(), func, str) ;
    std::cout<< tmp << std::flush ; 
  }
}

//--------------------------------------------------------------
//==============================================================