
#include "Log.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include "Utilities.hpp"
#include <fstream>


namespace impl {
void write(const std::string& message, const std::string& file, unsigned line, bool onstd = true)
{
    static std::ofstream f("Log.txt");
    if(onstd)
    {
        std::cerr<<file + ":" + toString(line) + ":0: "+message<<"\n";
        std::cerr.flush();
    }

    f<<file + ":" + toString(line) + ":0: "+message<<"\n";

    f.flush();
}
}
void Log::reportError(const std::string& errorMessage, const std::string& file, unsigned line)
{
    impl::write("error: " + errorMessage, file, line, !fatalErrors);
    if(fatalErrors)
        throw AssertExcpt("Fatal error occured : " + errorMessage);
}

void Log::debugInfo(const std::string& info, const std::string& file, unsigned line)
{
    impl::write("info: " + info, file, line);
}

void Log::reportWarning(const std::string& warning, const std::string& file, unsigned line)
{
    impl::write("warning: " + warning, file, line);
}

void Log::setErrorsFatal(bool y)
{
    fatalErrors = y;
}

bool Log::fatalErrors = false;

