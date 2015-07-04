#include "../src/Signature.hpp"
#include "catch.hpp"

#include <climits>
#include <exception>
#include <sstream>
#include <array>
#include <map>
#include <algorithm>

using namespace std;


TEST_CASE("Test of Signature structure","Signature")
{
    SECTION("Default Constructor"){
        Signature S;
        REQUIRE(S.getNumber() == 4);
        REQUIRE(S.getStep() == 4);
    }

}
