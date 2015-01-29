#include "MusicRules.hpp"
#include "Music.hpp"
#include "catch.hpp"
#include <climits>
#include <exception>
#include <sstream>
#include <algorithm>

// WARNING : due to the position of the files, make test should be used only in lazychord directory

using namespace std;

TEST_CASE("Test of getBestChordProgression","[getBCP]")
{
    string ruleFile = "./default.rules";
    vector<string> ruleNames = { "Default1", "Default2", "Default3", "Default4", "Default5"};
    vector<Chord> simpleChords;

    simpleChords.push_back(Chord(4,CompleteNoteName(NoteName::C)));
    simpleChords.push_back(Chord(4,CompleteNoteName(NoteName::F)));
    simpleChords.push_back(Chord(4,CompleteNoteName(NoteName::G)));
    simpleChords.push_back(Chord(4,CompleteNoteName(NoteName::A), Chord::SeventhType::None, Chord::ChordType::Minor));

    MachineLearningOutput possibleChords;

    for (unsigned c0 = 0; c0 < simpleChords.size(); c0++)
    {
	for (unsigned c1 = 0; c1 < simpleChords.size(); c1++)
	{
	    for (unsigned c2 = 0; c2 < simpleChords.size(); c2++)
	    {
		for (unsigned c3 = 0; c3 < simpleChords.size(); c3++)
		{
		    for (unsigned c4 = 0; c4 < simpleChords.size(); c4++)
		    {
			for (unsigned c5 = 0; c5 < simpleChords.size(); c5++)
			{
			    for (unsigned c6 = 0; c6 < simpleChords.size(); c6++)
			    {
				for (unsigned c7 = 0; c7 < simpleChords.size(); c7++)
				{
				    ChordProgression cp(8);
				    cp[0] = simpleChords[c0];
				    cp[1] = simpleChords[c1];
				    cp[2] = simpleChords[c2];
				    cp[3] = simpleChords[c3];
				    cp[4] = simpleChords[c4];
				    cp[5] = simpleChords[c5];
				    cp[6] = simpleChords[c6];
				    cp[7] = simpleChords[c7];
				    possibleChords.push_back(make_pair(cp,1));
				}
			    }
			}
		    }
		}
	    }
	}
    }
    /*SECTION("Examples execute")
    {
	vector<string> melodyFiles;
	melodyFiles.push_back("./examples/Melodies/DavidGuetta_WhenLoveTakesOver_Refrain_Melody.abc");
	melodyFiles.push_back("./examples/Melodies/MGMT_Kids_Melody.abc");

	melodyFiles.push_back("./examples/Melodies/MickaelJackson_BeatIt_Verse_Melody.abc");
	melodyFiles.push_back("./examples/Melodies/TheBeatles_LetItBe_Refrain_Melody.abc");
	melodyFiles.push_back("./examples/Melodies/TheBeatles_LetItBe_Verse_Melody.abc");
	melodyFiles.push_back("./examples/Melodies/Timbaland_Apologize_Refrain_Melody.abc");
	melodyFiles.push_back("./examples/Melodies/Train_HeySoulSister_Melody.abc");

	for (auto& mf : melodyFiles)
	{
	    if(possibleChords.size()>100)
            possibleChords.resize(100);
	    REQUIRE_NOTHROW(getBestChordProgression(ruleFile, ruleNames, possibleChords, mf, 1));
	}
    }*/
}
