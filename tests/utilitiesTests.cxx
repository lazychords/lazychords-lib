#include "../src/Signature.hpp"
#include "../src/Utilities.hpp"
#include "catch.hpp"
#include "genericTests.hxx"
#include <sstream>
#include <fstream>
#define FOR(i,n) for(unsigned i=0;i<n;i++)
using namespace std;


TEST_CASE("Test of Utilities","Utilities")
{
    SECTION("getFile"){
        SECTION("invalid file"){
            REQUIRE_THROWS(getFile("auieuaennrst.nonsense"));
        }
        SECTION("Reading file"){
            stringstream ss;
            ss<<"auieuae"<<endl<<23<<">[]^^^±»«"<<endl;
            ofstream file("test_write.out");
            file<<ss.str();
            file.close();
            REQUIRE(getFile("test_write.out")==ss.str());
            system("rm test_write.out");
        }
    }

    SECTION("binary logarithm"){
        FOR(i,30){
            REQUIRE(logUnsigned(2, safe_cast<unsigned>(1<<i))==i);
            if(i & (i-1)){
                REQUIRE_THROWS(logUnsigned(2, i));
            }
        }
    }
}


