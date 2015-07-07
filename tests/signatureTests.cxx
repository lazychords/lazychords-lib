#include "../src/Signature.hpp"
#include "catch.hpp"

#include <climits>
#include <exception>
#include <sstream>
#include <array>
#include <map>
#include <algorithm>

#define FOR(i,n) for(unsigned i=0;i<n;i++)
using namespace std;


TEST_CASE("Test of Signature structure","Signature")
{
    SECTION("Default Constructor"){
        Signature S;
        REQUIRE(S.getNumber() == 4);
        REQUIRE(S.getStep() == 4);
    }
    
    SECTION("Custom Constructor"){
        SECTION("Valid Constructions"){
            FOR(i,8){
                FOR(j,15){
                    REQUIRE_NOTHROW(Signature(j+1,1<<i));
                }
            }
            REQUIRE_NOTHROW(Signature(4,4));
        }
        SECTION("Invalid Constructions"){
            FOR(i,33){
                REQUIRE_THROWS(Signature(0,i));
                REQUIRE_THROWS(Signature(i,0));
                REQUIRE_THROWS(Signature(i,3));
                REQUIRE_THROWS(Signature(i,5));
                REQUIRE_THROWS(Signature(i,6));
                REQUIRE_THROWS(Signature(i,7));
                REQUIRE_THROWS(Signature(i,9));
                REQUIRE_THROWS(Signature(i,10));
                REQUIRE_THROWS(Signature(i,11));
                REQUIRE_THROWS(Signature(i,12));
            }
        }
    }
}
