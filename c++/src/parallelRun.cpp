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
 This takes as argument:
    - The name of the test case
    - The number of the tonality
    - The number of the variable branching strategy
    - The number of the value branching strategy
    It will write in a CSV file the statistics
 */
int main(int argc, char* argv[]) {
    if(argc < 6) return 1; /// If the number of arguments is not right, return error code

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                                     Tonalities                                                      *
 *                                                                                                                     *
 ***********************************************************************************************************************/
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

    string testCase2Name = "I5-IV5-VII6-I6-III5-VI5-II6-V7+-I5";
    vector<int> chords2 = {FIRST_DEGREE, FOURTH_DEGREE, SEVENTH_DEGREE, FIRST_DEGREE, THIRD_DEGREE, SIXTH_DEGREE,
                           SECOND_DEGREE, FIFTH_DEGREE, FIRST_DEGREE};
    vector<int> chords_qualities_major2 = {MAJOR_CHORD, MAJOR_CHORD, DIMINISHED_CHORD, MAJOR_CHORD, MINOR_CHORD,
                                           MINOR_CHORD, MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD};
    vector<int> chords_qualities_minor2 = {MINOR_CHORD, MINOR_CHORD, DIMINISHED_CHORD, MINOR_CHORD, MAJOR_CHORD,
                                           MAJOR_CHORD, DIMINISHED_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD};
    vector<int> states2 = {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FIRST_INVERSION, FIRST_INVERSION, FUNDAMENTAL_STATE,
                           FUNDAMENTAL_STATE, FIRST_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE};
    vector<vector<int>> testCase2 = {chords2, chords_qualities_major2, chords_qualities_minor2, states2};

    string testCase3Name = "I5-V+4-I6-II6-V5-I5-IV5-I64-V7+-I5";
    vector<int> chords3 = {FIRST_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, SECOND_DEGREE, FIFTH_DEGREE, FIRST_DEGREE,
                           FOURTH_DEGREE, FIRST_DEGREE, FIFTH_DEGREE, FIRST_DEGREE};
    vector<int> chords_qualities_major3 = {MAJOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD,
                                           MAJOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD};
    vector<int> chords_qualities_minor3 = {MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD, DIMINISHED_CHORD, MAJOR_CHORD,
                                             MINOR_CHORD, MINOR_CHORD, MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD};
    vector<int> states3 = {FUNDAMENTAL_STATE, THIRD_INVERSION, FIRST_INVERSION, FIRST_INVERSION, FUNDAMENTAL_STATE,
                            FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, SECOND_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE};
    vector<vector<int>> testCase3 = {chords3, chords_qualities_major3, chords_qualities_minor3, states3};



    vector<vector<vector<int>>> testCases = {testCase1, testCase2};

    vector<string> testCasesNames = {testCase1Name, testCase2Name};

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                              Branching strategies                                                   *
 *                                                                                                                     *
 ***********************************************************************************************************************/

    vector<int> var_sel = {DEGREE_MAX, DOM_SIZE_MIN, LEFT_TO_RIGHT, RIGHT_TO_LEFT}; //@todo add left to right but soprano to bass and not bass to soprano
    vector<int> val_sel = {VAL_MIN, VAL_MAX}; // @todo add custom ones

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                                  Solution generation                                                *
 *                                                                                                                     *
 ***********************************************************************************************************************/
    try{
//        for (int i = 0; i < argc; ++i) {
//            std::cout << "argv[" << i << "]: " << argv[i] << std::endl;
//        }
        int test_case_number = stoi(argv[1]);
        int tonic = stoi(argv[2]);
        int mode = stoi(argv[3]);
        int variable_selection_heuristic = stoi(argv[4]);
        int value_selection_heuristic = stoi(argv[5]);

        Tonality *tonality;
        vector<int> qualities;
        if(mode == MAJOR_MODE){
            tonality = new MajorTonality(tonic);
            qualities = testCases[test_case_number][1];
        }
        else{
            tonality = new MinorTonality(tonic);
            qualities = testCases[test_case_number][2];
        }

        string csv_line;
        csv_line += testCasesNames[test_case_number] + " , " + tonality->get_name() +  " , " +
                    variable_selection_heuristics_names[variable_selection_heuristic] + " ," +
                    value_selection_heuristics_names[value_selection_heuristic];

        auto pb = new FourVoiceTexture(testCases[test_case_number][0].size(), tonality,
                                                    testCases[test_case_number][0],qualities,
                                                    testCases[test_case_number][3],
                                                    variable_selection_heuristic,
                                                    value_selection_heuristic);
        auto best_sol = find_best_solution(pb, 60000, "test.csv", csv_line);
        delete pb;
        if(best_sol != nullptr){
            string fileName = testCasesNames[test_case_number] + "|" + tonalityNames[tonic] +  "|" +
                             variable_selection_heuristics_names[variable_selection_heuristic] + "|" +
                             value_selection_heuristics_names[value_selection_heuristic];
            //@todo change the function to take a string instead of an integer
            //writeSolToMIDIFile(testCases[test_case_number][0].size(), fileName, best_sol);
        }
        return 0;
    }
    catch (exception& e){
        cout << "Error: " << e.what() << endl;
        return 1;
    }
}