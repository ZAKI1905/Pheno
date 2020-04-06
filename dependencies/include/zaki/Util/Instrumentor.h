#ifndef Zaki_Util_Instrumentor_H
#define Zaki_Util_Instrumentor_H

// Basic instrumentation profiler by Cherno

// Usage: include this header file somewhere in your code (eg. precompiled header), and then use like:
//
// Instrumentor::Get().BeginSession("Session Name");        // Begin session 
// {
//     InstrumentationTimer timer("Profiled Scope Name");   // Place code like this in scopes you'd like to include in profiling
//     // Code
// }
// Instrumentor::Get().EndSession();                        // End Session
//
// You will probably want to macro-fy this, to switch on/off easily and use things like __FUNCSIG__ for the profile name.

#include <string>
#include <chrono>
#include <fstream>
#include <mutex>
#include <thread>

//--------------------------------------------------------------
namespace Zaki
{
//--------------------------------------------------------------
namespace Util
{
//==============================================================
struct ProfileResult
{
    std::string Name;
    long long Start, End;
    uint32_t ThreadID;
};

//==============================================================
//                     Singleton design
class Instrumentor
{

    //--------------------------------------------------------------
    private:

    std::string     m_sessionName   = "None";
    std::ofstream   m_OutputStream;
    int             m_ProfileCount = 0;
    std::mutex      m_lock;
    bool            m_activeSession = false;

    Instrumentor() {}

    //--------------------------------------------------------------
    void IBeginSession(const std::string&, const std::string& ="results.json") ;

    //--------------------------------------------------------------
    void IEndSession() ;

    //--------------------------------------------------------------
    public:

    //--------------------------------------------------------------
    static void BeginSession(const std::string& , const std::string& = "results.json") ;

    //--------------------------------------------------------------
    ~Instrumentor()
    {
        EndSession();
    }

    //--------------------------------------------------------------
    static void EndSession() ;

    //--------------------------------------------------------------
    void WriteProfile(const ProfileResult&) ;

    //--------------------------------------------------------------
    void WriteHeader() ;

    //--------------------------------------------------------------
    void WriteFooter() ;
    //--------------------------------------------------------------
    static Instrumentor& Get() ;
    //--------------------------------------------------------------
};

//==============================================================
class InstrumentationTimer
{
    //--------------------------------------------------------------
    public:

    InstrumentationTimer(const std::string & name)
            : m_Result({ name, 0, 0, 0 }), m_Stopped(false)
    {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    //--------------------------------------------------------------
    ~InstrumentationTimer()
    {
        if (!m_Stopped)
            Stop();
    }

    //--------------------------------------------------------------
    void Stop() ;
    //--------------------------------------------------------------
    private:
        ProfileResult m_Result;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
        bool m_Stopped;
};
//==============================================================

//--------------------------------------------------------------
} // End of namespace Util
//--------------------------------------------------------------
} // End of namespace Zaki
//==============================================================
//                    Profiling Macros
//==============================================================

#if defined(_MSC_VER)
#define Z_PRETTY_FUNCTION __FUNCSIG__
#else
#define Z_PRETTY_FUNCTION __PRETTY_FUNCTION__
#endif
//--------------------------------------------------------------
#define PROFILING 1
#if PROFILING
  #define PROFILE_SCOPE(name) Zaki::Util::InstrumentationTimer inst_timer##__LINE__(name)
  #define PROFILE_FUNCTION() PROFILE_SCOPE(Z_PRETTY_FUNCTION)
#else
  #define PROFILE_SCOPE(name)
  #define PROFILE_FUNCTION()
#endif

//==============================================================
#endif /*Zaki_Util_Instrumentor_H*/
