#include "Random.hpp"
#include <chrono>
#include "Log.hpp"
#include "Utilities.hpp"

bool Random::seedSet = false;
long unsigned Random::seed = 42;
std::mt19937 Random::generator = std::mt19937(42);

void Random::setSeed(long unsigned s)
{
    seedSet = true;
    seed = s;
    generator = std::mt19937(s);
    Log::debugInfo("Random seed changed to : " + toString(s), __FILE__, __LINE__);
}

void Random::pickTimeSeed()
{
    setSeed(safe_cast<long unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
}

