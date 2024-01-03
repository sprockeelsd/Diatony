#include "../headers/FourVoiceTexture.hpp"
#include "../headers/Utilities.hpp"
#include "../headers/Tonality.hpp"
#include "../headers/MajorTonality.hpp"
#include "../headers/MinorTonality.hpp"
#include "../headers/Solver.hpp"
#include "../headers/MidiFile.h"
#include "../headers/MidiFileGeneration.hpp"

using namespace Gecode;
using namespace std;
using namespace smf;

/**
- Tests different branching strategies on different problems in different tonalities
 */
int main(int argc, char* argv[]) {
    /// Tonalities
    Tonality* CMajor = new MajorTonality(C);
    Tonality* CMinor = new MinorTonality(C);
    Tonality* AbMajor = new MajorTonality(A_FLAT);
    Tonality* BbMinor = new MinorTonality(B_FLAT);
    Tonality* EMajor = new MajorTonality(E);
    Tonality* CsharpMinor = new MinorTonality(C_SHARP);

    vector<Tonality*> tonalities = {CMajor, CMinor, AbMajor, BbMinor, EMajor, CsharpMinor};

    vector<string> tonalityNames;
    for(int i = 0; i < tonalities.size();i++)
        tonalityNames.push_back(tonalities[i]->get_name());

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                                     Test cases                                                      *
 *                                                                                                                     *
 ***********************************************************************************************************************/

    string testCase1Name = "I5 - V5 - I5 - V65 - I6 - II6 - V64 - V7+ - I5";
    vector<int> chords1 = {FIRST_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, SECOND_DEGREE,
                           FIRST_DEGREE, FIFTH_DEGREE, FIRST_DEGREE};
    vector<int> chords_qualities_major1 = {MAJOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD,
                                    MINOR_CHORD, MAJOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD};
    vector<int> chords_qualities_minor1 = {MINOR_CHORD, MAJOR_CHORD, MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD,
                                    DIMINISHED_CHORD, MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD};
    vector<int> states1 = {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, SECOND_INVERSION, FIRST_INVERSION,
                          FIRST_INVERSION, SECOND_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE};
    vector<vector<int>> testCase1 = {chords1, chords_qualities_major1, chords_qualities_minor1, states1};
    auto testCases = {testCase1};

    vector<string> testCasesNames = {testCase1Name};

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                                  Solution generation                                                *
 *                                                                                                                     *
 ***********************************************************************************************************************/

    //@todo vary the branching strategies to compare them

    int solNumber = 0;
    int i = 0;
    for (auto tonality : tonalities){
        int j = 0;
        for(auto testCase : testCases){
            FourVoiceTexture* problem;
            if(MinorTonality* minorPtr = dynamic_cast<MinorTonality*>(tonality)){ /// the tonality is minor
                problem = new FourVoiceTexture(testCase[0].size(), tonality, testCase[0],
                                                     testCase[2], testCase[3]);
            }
            else{/// the tonality is major
                problem = new FourVoiceTexture(testCase[0].size(), tonality, testCase[0],
                                                     testCase[1], testCase[3]);
            }
            string testConfig = "Test case: " + testCasesNames[j] + " in " + tonalityNames[i] + "\n";
            write_to_log_file(testConfig.c_str(), STATISTICS_FILE);

            auto bestSol = find_best_solution(problem);

            writeSolToMIDIFile(testCase[0].size(), solNumber, bestSol);

            solNumber++;
            j++;
        }
        i++;
    }

    return 0;
}