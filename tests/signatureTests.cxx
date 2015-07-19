#include "../src/Signature.hpp"
#include "../src/Utilities.hpp"
#include "catch.hpp"
#include "genericTests.hxx"
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
            FOR(i,6){
                FOR(j,127){
                    REQUIRE_NOTHROW(Signature(j+1,safe_cast<unsigned>(1<<i)));
                    Signature S(j+1,safe_cast<unsigned>(1<<i));
                }
            }
            REQUIRE_NOTHROW(Signature(4,4));
            forAll<Signature>([](const Signature& c){
                    /** @todo enable capture when << operator is available*/
                    //CAPTURE(c);
                    REQUIRE(c.check());
                });
        }
        SECTION("Invalid Constructions"){
            FOR(i,Signature::maxNumber){
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
                REQUIRE_THROWS(Signature(i,13));
                REQUIRE_THROWS(Signature(i,14));
                REQUIRE_THROWS(Signature(i,15));
                REQUIRE_THROWS(Signature(i,17));
                REQUIRE_THROWS(Signature(i,18));
                REQUIRE_THROWS(Signature(i,19));
                REQUIRE_THROWS(Signature(i,20));
                REQUIRE_THROWS(Signature(i,21));
                REQUIRE_THROWS(Signature(i,22));
                REQUIRE_THROWS(Signature(i,23));
                REQUIRE_THROWS(Signature(i,24));
                REQUIRE_THROWS(Signature(i,25));
                REQUIRE_THROWS(Signature(i,26));
                REQUIRE_THROWS(Signature(i,27));
                REQUIRE_THROWS(Signature(i,28));
                REQUIRE_THROWS(Signature(i,29));
                REQUIRE_THROWS(Signature(i,30));
                REQUIRE_THROWS(Signature(i,31));
                REQUIRE_THROWS(Signature(i,33));
                REQUIRE_THROWS(Signature(i,34));
                REQUIRE_THROWS(Signature(i,35));
                REQUIRE_THROWS(Signature(i,36));
                REQUIRE_THROWS(Signature(i,37));
                REQUIRE_THROWS(Signature(i,38));
                REQUIRE_THROWS(Signature(i,39));
                REQUIRE_THROWS(Signature(i,40));
                REQUIRE_THROWS(Signature(i,41));
                REQUIRE_THROWS(Signature(i,42));
                REQUIRE_THROWS(Signature(i,43));
                REQUIRE_THROWS(Signature(i,44));
                REQUIRE_THROWS(Signature(i,45));
                REQUIRE_THROWS(Signature(i,46));
                REQUIRE_THROWS(Signature(i,47));
                REQUIRE_THROWS(Signature(i,48));
                REQUIRE_THROWS(Signature(i,49));
                REQUIRE_THROWS(Signature(i,50));
                REQUIRE_THROWS(Signature(i,51));
                REQUIRE_THROWS(Signature(i,52));
                REQUIRE_THROWS(Signature(i,53));
                REQUIRE_THROWS(Signature(i,54));
                REQUIRE_THROWS(Signature(i,55));
                REQUIRE_THROWS(Signature(i,56));
                REQUIRE_THROWS(Signature(i,57));
                REQUIRE_THROWS(Signature(i,58));
                REQUIRE_THROWS(Signature(i,59));
                REQUIRE_THROWS(Signature(i,60));
                REQUIRE_THROWS(Signature(i,61));
                REQUIRE_THROWS(Signature(i,62));
                REQUIRE_THROWS(Signature(i,63));
            }
        }
    }
    SECTION("Comparison operators"){
        FOR(i,6){
            FOR(j,63){
                Signature S1(j+1,safe_cast<unsigned>(1<<i));
                FOR(k,6){
                    FOR(l,63){
                        Signature S2(l+1,safe_cast<unsigned>(1<<k));
                        if(i==k && j==l){
                             REQUIRE(S1==S2);
                             REQUIRE_FALSE(S1!=S2);
                         }else{
                             REQUIRE(S1!=S2);
                             REQUIRE_FALSE(S1==S2);
                         }
                    }
                }
            }
        }
        forAll<Signature>([](const Signature& c){
                /** @todo enable capture when << operator is available*/
                //CAPTURE(c);
                REQUIRE(c==c);
            });
    }
}
