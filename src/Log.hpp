#ifndef LOG_HPP_INCLUDED
#define LOG_HPP_INCLUDED

///@todo comment this file + add GPL information
///@todo comment ASSERT
#ifdef DEBUG
    #define ASSERT(X) if(!X) Log::reportError(std::string(#X), std::string(__FILE__), __LINE__);
#else
    #define ASSERT(X) ;
#endif
#include <memory>
#include <fstream>

class LogImpl;

class Log
{
    ///@todo comment this class
    static std::unique_ptr<LogImpl> l;
    static bool fatalErrors;
public :
    Log() = delete;

    /**
     *@brief Reports an error. If setErrorsFatal has been set to yes, it throws an std::runtime_error
     *@author Julien
    **/
    static void reportError(const std::string& errorMessage, const std::string& file, unsigned line);

    /**
     *@brief Enables/Disable std::runtime_error throwing when an error is thrown
     *@author Julien
    **/
    static void setErrorsFatal(bool y = true);

    /**
     *@brief Reports an info useful for debugging
     *@author Julien
    **/
    static void debugInfo(const std::string& info, const std::string& file, unsigned line);

    /**
     *@brief Reports a warning
     *@author Julien
    **/
    static void reportWarning(const std::string& warning, const std::string& file, unsigned line);
};


#endif // LOG_HPP_INCLUDED
