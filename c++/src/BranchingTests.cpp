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
    tonalityNames.reserve(tonalities.size());
    for(auto & tonality : tonalities)
        tonalityNames.push_back(tonality->get_name());

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                                     Test cases                                                      *
 *                                                                                                                     *
 ***********************************************************************************************************************/

    string testCase1Name = "I5-V5-I5-V65-I6-II6-V64-V7+-I5";
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

    /// branching strategies
    vector<int> branchingStrategies = {DEGREE_MAX_VAL_MIN, DOM_SIZE_MIN_VAL_MIN, FIRST_UNASSIGNED_VAL_MIN, RIGHT_TO_LEFT_VAL_MIN};

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                                  Solution generation                                                *
 *                                                                                                                     *
 ***********************************************************************************************************************/

    string timeString = time();
    timeString.erase(std::remove(timeString.begin(), timeString.end(), '\n'), timeString.end());
    std::replace(timeString.begin(), timeString.end(), ' ', '-');
    string statFile = STATISTICS_CSV + timeString.c_str() + ".csv";

    write_to_log_file(time().c_str(), LOG_FILE);
    write_to_log_file(time().c_str(), statFile);

    string headers = "Chord progression , Tonality, Branching strategy, Time to prove optimality, , "

                     "Best solution Statistics, Nodes traversed, Failed nodes explored, Restarts performed, "
                        "Propagators executed, No goods generated, Maximal depth of explored tree, number of 4 note "
                        "diminished chords, number of chords with 3 notes, number of fundamental state chords without "
                        "doubled bass, number of incomplete chords, number of common notes in the tenor, number of "
                        "common notes in the alto, number of common notes in the soprano, cost of melodic intervals,, "

                     " Total search statistics, Nodes traversed, Failed nodes explored, Restarts performed, "
                        "Propagators executed, No goods generated, Maximal depth of explored tree, , "

                    "Intermediate solutions ,time, number of 4 note diminished chords, number of chords with 3 notes, "
                        "number of fundamental state chords without doubled bass, number of incomplete chords, "
                        "number of common notes in the tenor, number of common notes in the alto, "
                        "number of common notes in the soprano, cost of melodic intervals, ,";
    write_to_log_file(headers.c_str(), statFile);

    int solNumber = 0;
    int i = 0;
    /// for each tonality
    for (auto tonality : tonalities){
        int j = 0;
        /// for each test case
        for(auto testCase : testCases){

            /// for each branching strategy
            for(auto strategy: branchingStrategies){
                /// define the test case
                string csv_line;
                csv_line += testCasesNames[j] + " , " + tonalityNames[i] + " , " + branchingStrategiesNames[strategy];

                /// define the problem instance
                FourVoiceTexture* problem;
                if(auto* minorPtr = dynamic_cast<MinorTonality*>(tonality)){ /// the tonality is minor
                    problem = new FourVoiceTexture(testCase[0].size(), tonality, testCase[0],
                                                   testCase[2], testCase[3], strategy);
                }
                else{ /// the tonality is major
                    problem = new FourVoiceTexture(testCase[0].size(), tonality, testCase[0],
                                                   testCase[1], testCase[3], strategy);
                }
                /// find the best solution (with a timeout of 90 seconds)
                auto bestSol = find_best_solution(problem, 90000, statFile,
                                                  csv_line);
                /// create a MIDI file to hear the solution
                writeSolToMIDIFile(testCase[0].size(), solNumber, bestSol);
                solNumber++;
            }
            j++;
        }
        i++;
    }
    return 0;
}