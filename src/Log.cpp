
#include "Log.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include "Utilities.hpp"
#include <fstream>

class LogImpl
{
    friend class Log;
private :
    LogImpl() = default;
    void write(const std::string& message, const std::string& file, unsigned line) const
    {
        static std::ofstream f("Log.txt");
        std::cerr<<file + ":" + toString(line) + ":0: "+message<<"\n";
        f<<file + ":" + toString(line) + ":0: "+message<<"\n";
        std::cerr.flush();
        f.flush();
    }
};

void Log::reportError(const std::string& errorMessage, const std::string& file, unsigned line)
{
    l->write("error: " + errorMessage, file, line);
    if(fatalErrors)
        throw std::runtime_error("Fatal error occured : " + errorMessage);
}

void Log::debugInfo(const std::string& info, const std::string& file, unsigned line)
{
    l->write("info: " + info, file, line);
}

void Log::reportWarning(const std::string& warning, const std::string& file, unsigned line)
{
    l->write("warning: " + warning, file, line);
}

void Log::setErrorsFatal(bool y)
{
    fatalErrors = y;
}

bool Log::fatalErrors = false;

std::unique_ptr<LogImpl> Log::l(new LogImpl);
