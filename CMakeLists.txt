CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7)
ENABLE_TESTING()

message("You can affect the compilation options with the WARNNONE, WARNALL, and OPTIMIZE macros. By default, none are active and the default options are -Wall -Wextra -pedantic and no optimizations")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

IF(MY_BOOST)
    message("Using custom BOOST path : ${MY_BOOST}")
    SET(BOOST_ROOT ${BOOST_ROOT} ${MY_BOOST})
ELSE(MY_BOOST)
    message("Using automatic boost path, please set MY_BOOST variable to the boost root to specify the boost folder if Cmake can not find boost")
ENDIF(MY_BOOST)


FIND_PACKAGE(Doxygen)
FIND_PACKAGE(Boost COMPONENTS system filesystem REQUIRED)


INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
IF(OPTIMIZE)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wunsafe-loop-optimizations ")
ELSE(OPTIMIZE)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
ENDIF(OPTIMIZE)
IF(WARNNONE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
ELSE(WARNNONE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")
    IF(WARNALL)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wcast-qual -Wcast-align -Wfloat-equal -Wpointer-arith")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunreachable-code -Wchar-subscripts -Wcomment -Wformat")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror-implicit-function-declaration -Wmain -Wmissing-braces")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wparentheses -Wsequence-point -Wreturn-type -Wswitch")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wuninitialized -Wreorder -Wundef -Wshadow -Wwrite-strings")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsign-compare  -Wconversion")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmissing-noreturn  -Wsign-conversion -Wunused ")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wstrict-aliasing -Wstrict-overflow -Wconversion")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdisabled-optimization -Wsuggest-attribute=noreturn ")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wsuggest-attribute=pure ")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wlogical-op")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wunused-parameter -Wshadow  -Weffc++")
    ENDIF(WARNALL)
ENDIF(WARNNONE)


#SET(SOFT_FLAGS "${CMAKE_CXX_FLAGS}")

ADD_DEFINITIONS(-DNOT_USE_ML)


############################
# LIBS
############################

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/libs/ELL/)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/libs/)

############################
# LazyChords Lib
############################

SET(LazyChordsLib_SRCS
  src/Chord.cpp
  src/Figure.cpp
  src/Key.cpp
  src/Signature.cpp
  src/Melody.cpp
  src/Note.cpp
  src/Pitch.cpp
  src/Log.cpp
  src/Utilities.cpp
)


INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/)
ADD_LIBRARY(LazyChordsLib ${LazyChordsLib_SRCS})


############################
# Tests
############################

#SET(CMAKE_CXX_FLAGS "${SOFT_FLAGS}")
SET(Tests_SRCS
    tests/MusicRulesTest.cxx
    tests/musicTest.cxx
    tests/TestMain.cxx
    tests/genericTests.cxx
)

INCLUDE_DIRECTORIES(tests/)
ADD_EXECUTABLE(runTests ${Tests_SRCS})
TARGET_LINK_LIBRARIES(runTests LazyChordsLib ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
TARGET_LINK_LIBRARIES(runTests m)

ADD_TEST(runTests runTests -d yes)

SET(Main_SRCS
    main.cpp
)

ADD_EXECUTABLE(main ${Main_SRCS})
TARGET_LINK_LIBRARIES(main LazyChordsLib)
TARGET_LINK_LIBRARIES(main ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(main m)


############################
# DOxygen
############################
IF(DOXYGEN_FOUND)
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    ADD_CUSTOM_TARGET(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
    #Wanted to change location of main file of documentation or at least automatically open documentation
    add_custom_command(TARGET doc
                     POST_BUILD
                     #COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/doc/html/index.html ${PROJECT_SOURCE_DIR}/doc.html
                     COMMAND #firefox ${PROJECT_SOURCE_DIR}/doc/html/index.html
    )
ENDIF(DOXYGEN_FOUND)


############################
# Coverage
############################
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
if(CMAKE_COMPILER_IS_GNUCXX)
    include(CodeCoverage)
    setup_target_for_coverage_cobertura(Lazychord_lib_coverage ${PROJECT_SOURCE_DIR}/runTests coverage)
endif()
