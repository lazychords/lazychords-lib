#include "../src/Signature.hpp"
#include "../src/Utilities.hpp"
#include "catch.hpp"
#include "genericTests.hxx"

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
                    REQUIRE_NOTHROW(Signature(j+1,powUnsigned(2,i)));
                    Signature S(j+1,powUnsigned(2,i));
                }
            }
            REQUIRE_NOTHROW(Signature(4,4));
            forAll<Signature>([](const Signature& c){
                    /** @todo enable capture when << operator is available*/
                    //CAPTURE(c);
                    REQUIRE_NOTHROW(c.check());
                });
        }
        SECTION("Invalid Constructions"){
            FOR(i,Signature::maxNumber){
                REQUIRE_PRECONDITION_FAIL(Signature(0,i));
                REQUIRE_PRECONDITION_FAIL(Signature(i,0));
                REQUIRE_PRECONDITION_FAIL(Signature(i,3));
                REQUIRE_PRECONDITION_FAIL(Signature(i,5));
                REQUIRE_PRECONDITION_FAIL(Signature(i,6));
                REQUIRE_PRECONDITION_FAIL(Signature(i,7));
                REQUIRE_PRECONDITION_FAIL(Signature(i,9));
                REQUIRE_PRECONDITION_FAIL(Signature(i,10));
                REQUIRE_PRECONDITION_FAIL(Signature(i,11));
                REQUIRE_PRECONDITION_FAIL(Signature(i,12));
                REQUIRE_PRECONDITION_FAIL(Signature(i,13));
                REQUIRE_PRECONDITION_FAIL(Signature(i,14));
                REQUIRE_PRECONDITION_FAIL(Signature(i,15));
                REQUIRE_PRECONDITION_FAIL(Signature(i,17));
                REQUIRE_PRECONDITION_FAIL(Signature(i,18));
                REQUIRE_PRECONDITION_FAIL(Signature(i,19));
                REQUIRE_PRECONDITION_FAIL(Signature(i,20));
                REQUIRE_PRECONDITION_FAIL(Signature(i,21));
                REQUIRE_PRECONDITION_FAIL(Signature(i,22));
                REQUIRE_PRECONDITION_FAIL(Signature(i,23));
                REQUIRE_PRECONDITION_FAIL(Signature(i,24));
                REQUIRE_PRECONDITION_FAIL(Signature(i,25));
                REQUIRE_PRECONDITION_FAIL(Signature(i,26));
                REQUIRE_PRECONDITION_FAIL(Signature(i,27));
                REQUIRE_PRECONDITION_FAIL(Signature(i,28));
                REQUIRE_PRECONDITION_FAIL(Signature(i,29));
                REQUIRE_PRECONDITION_FAIL(Signature(i,30));
                REQUIRE_PRECONDITION_FAIL(Signature(i,31));
                REQUIRE_PRECONDITION_FAIL(Signature(i,33));
                REQUIRE_PRECONDITION_FAIL(Signature(i,34));
                REQUIRE_PRECONDITION_FAIL(Signature(i,35));
                REQUIRE_PRECONDITION_FAIL(Signature(i,36));
                REQUIRE_PRECONDITION_FAIL(Signature(i,37));
                REQUIRE_PRECONDITION_FAIL(Signature(i,38));
                REQUIRE_PRECONDITION_FAIL(Signature(i,39));
                REQUIRE_PRECONDITION_FAIL(Signature(i,40));
                REQUIRE_PRECONDITION_FAIL(Signature(i,41));
                REQUIRE_PRECONDITION_FAIL(Signature(i,42));
                REQUIRE_PRECONDITION_FAIL(Signature(i,43));
                REQUIRE_PRECONDITION_FAIL(Signature(i,44));
                REQUIRE_PRECONDITION_FAIL(Signature(i,45));
                REQUIRE_PRECONDITION_FAIL(Signature(i,46));
                REQUIRE_PRECONDITION_FAIL(Signature(i,47));
                REQUIRE_PRECONDITION_FAIL(Signature(i,48));
                REQUIRE_PRECONDITION_FAIL(Signature(i,49));
                REQUIRE_PRECONDITION_FAIL(Signature(i,50));
                REQUIRE_PRECONDITION_FAIL(Signature(i,51));
                REQUIRE_PRECONDITION_FAIL(Signature(i,52));
                REQUIRE_PRECONDITION_FAIL(Signature(i,53));
                REQUIRE_PRECONDITION_FAIL(Signature(i,54));
                REQUIRE_PRECONDITION_FAIL(Signature(i,55));
                REQUIRE_PRECONDITION_FAIL(Signature(i,56));
                REQUIRE_PRECONDITION_FAIL(Signature(i,57));
                REQUIRE_PRECONDITION_FAIL(Signature(i,58));
                REQUIRE_PRECONDITION_FAIL(Signature(i,59));
                REQUIRE_PRECONDITION_FAIL(Signature(i,60));
                REQUIRE_PRECONDITION_FAIL(Signature(i,61));
                REQUIRE_PRECONDITION_FAIL(Signature(i,62));
                REQUIRE_PRECONDITION_FAIL(Signature(i,63));
            }
        }
    }
    SECTION("Comparison operators"){
        FOR(i,6){
            FOR(j,63){
                Signature S1(j+1,powUnsigned(2, i));
                FOR(k,6){
                    FOR(l,63){
                        Signature S2(l+1,powUnsigned(2, k));
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
