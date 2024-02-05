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
    //if(argc != 9) return 1; /// If the number of arguments is not right, return error code
//    for (int i = 0; i < argc; ++i) {
//        std::cout << "argv[" << i << "]: " << argv[i] << std::endl;
//    }

//    //@todo parse the string into the 5 numbers temporarily
//    // Create a string stream to parse the numbers
//    std::istringstream iss(argv[1]);
//    // Variables to store each number
//    int num1, num2, num3, num4, num5;
//    // Use the string stream to extract each number
//    iss >> num1 >> num2 >> num3 >> num4 >> num5;

//    int test_case_number = num1;
//    int tonic = num2;
//    int mode = num3;
//    int variable_selection_heuristic = num4;
//    int value_selection_heuristic = num5;

    int test_case_number = stoi(argv[1]);
    int tonic = stoi(argv[2]);
    int mode = stoi(argv[3]);
    int variable_selection_heuristic = stoi(argv[4]);
    int value_selection_heuristic = stoi(argv[5]);
    int cutoff_depth = stoi(argv[6]);
    int cutoff_strategy = stoi(argv[7]);
    int nogood_depth = stoi(argv[8]);

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

    string testCase3Name = "I5-V6-VI5-V5-IV5-I6-II5-V5-I5-V6-VI5-V5-IV5-V7+-I5";
    vector<int> chords3 = {FIRST_DEGREE, FIFTH_DEGREE, SIXTH_DEGREE, FIFTH_DEGREE, FOURTH_DEGREE, FIRST_DEGREE,
                           SECOND_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, FIFTH_DEGREE, SIXTH_DEGREE, FIFTH_DEGREE,
                           FOURTH_DEGREE, FIFTH_DEGREE, FIRST_DEGREE};
    vector<int> chords_qualities_major3 = {MAJOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MAJOR_CHORD,
                                           MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD,
                                           MAJOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD};
    vector<int> chords_qualities_minor3 = {MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MINOR_CHORD,
                                           DIMINISHED_CHORD, MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD,
                                           MAJOR_CHORD, MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD};
    vector<int> states3 = {FUNDAMENTAL_STATE, FIRST_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE,
                           FIRST_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FIRST_INVERSION,
                           FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE};
    vector<vector<int>> testCase3 = {chords3, chords_qualities_major3, chords_qualities_minor3, states3};

    string testCase4Name = "I5-V5-VI5-I5-III5-VI5-II5-I5-V5"; //todo test strategies with this one in Ab Major
    vector<int> chords4 = {FIRST_DEGREE, FIFTH_DEGREE, SIXTH_DEGREE, FIRST_DEGREE, THIRD_DEGREE, SIXTH_DEGREE,
                          SECOND_DEGREE, FIRST_DEGREE, FIFTH_DEGREE};
    vector<int> chords_qualities_major4 = {MAJOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MINOR_CHORD,
                                    MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD};
    vector<int> chords_qualities_minor4 = {MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD,
                                    DIMINISHED_CHORD, MINOR_CHORD, MINOR_CHORD};
    vector<int> states4 = {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE,
                          FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE};
    vector<vector<int>> testCase4 = {chords4, chords_qualities_major4, chords_qualities_minor4, states4};

    string testCase5Name = "I5-V5-VI5-I5-VI5-VII6-III5-VI5-II6-V7+-I5";
    vector<int> chords5 = {FIRST_DEGREE, FIFTH_DEGREE, SIXTH_DEGREE, FIRST_DEGREE, FOURTH_DEGREE, SEVENTH_DEGREE,
                          THIRD_DEGREE, SIXTH_DEGREE, SECOND_DEGREE, FIFTH_DEGREE, FIRST_DEGREE};
    vector<int> chords_qualities_major5 = {MAJOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, DIMINISHED_CHORD,
                                          MINOR_CHORD, MINOR_CHORD, MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD};
    vector<int> chords_qualities_minor5 = {MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MINOR_CHORD, DIMINISHED_CHORD,
                                    MAJOR_CHORD, MAJOR_CHORD, DIMINISHED_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD};
    vector<int> states5 = {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE,
                          FIRST_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FIRST_INVERSION, FUNDAMENTAL_STATE,
                          FUNDAMENTAL_STATE};
    vector<vector<int>> testCase5 = {chords5, chords_qualities_major5, chords_qualities_minor5, states5};

    string testCase6Name = "I5-II6-IV5-V+6-I6-III5-VI5-V65/-I5-IV5-I64-V7+-I5";
    vector<int> chords6 = {FIRST_DEGREE, SECOND_DEGREE, FOURTH_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, THIRD_DEGREE,
                          SIXTH_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, FOURTH_DEGREE, FIRST_DEGREE, FIFTH_DEGREE,
                          FIRST_DEGREE};
    vector<int> chords_qualities_major6 = {MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD,
                                    MINOR_CHORD, MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD, MAJOR_CHORD,
                                    MAJOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD};
    vector<int> chords_qualities_minor6 = {MINOR_CHORD, DIMINISHED_CHORD, MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD,
                                          MAJOR_CHORD, MAJOR_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD, MINOR_CHORD, MINOR_CHORD,
                                          DOMINANT_SEVENTH_CHORD, MINOR_CHORD};
    vector<int> states6 = {FUNDAMENTAL_STATE, FIRST_INVERSION, FUNDAMENTAL_STATE, SECOND_INVERSION, FIRST_INVERSION,
                          FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FIRST_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE,
                          SECOND_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE};
    vector<vector<int>> testCase6 = {chords6, chords_qualities_major6, chords_qualities_minor6, states6};


    vector<vector<vector<int>>> testCases = {testCase1, testCase2, testCase3, testCase4, testCase5, testCase6}; //testCase1, testCase2, testCase3, testCase4, testCase5, testCase6

    vector<string> testCasesNames = {testCase1Name, testCase2Name, testCase3Name, testCase4Name, testCase5Name, testCase6Name}; //testCase1Name, testCase2Name, testCase3Name, testCase4Name, testCase5Name, testCase6Name

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                              Branching strategies                                                   *
 *                                                                                                                     *
 ***********************************************************************************************************************/

    vector<int> var_sel = {RIGHT_TO_LEFT}; //DEGREE_MAX, DOM_SIZE_MIN, LEFT_TO_RIGHT,
    vector<int> val_sel = {VAL_RND};

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                                 Search parameters                                                   *
 *                                                                                                                     *
 **********************************************************************************************************************/
    /// cutoff generators
    int nVariables = testCases[test_case_number][0].size() * 4;
    vector<int> cutoff_depths = {nVariables / 2, nVariables, 2 * nVariables, 4 * nVariables, nVariables ^ 2,
                                 2 * nVariables ^ 2};
    vector<string> cutoff_depths_names = {"n/2", "n", "2n", "4n", "n^2", "2n^2"};

    Cutoff* geo = Search::Cutoff::geometric(cutoff_depths[cutoff_depth], 2);
    Cutoff* lin = Search::Cutoff::linear(cutoff_depths[cutoff_depth]);
    Cutoff* lin_geo = Search::Cutoff::merge(lin, geo);
    vector<Cutoff*> cutoffs = {geo, lin, lin_geo};
    vector<string> cutoffs_names = {"geometric", "linear", "linear-geometric"};

    /// no good depth
    vector<int> nogoods_depths = {nVariables, 2*nVariables, 4*nVariables, nVariables^2};
    vector<string> nogoods_depths_names = {"n", "2n", "4n", "n^2"};

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                                  Solution generation                                                *
 *                                                                                                                     *
 ***********************************************************************************************************************/

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

    int size = testCases[test_case_number][0].size();

    string csv_line;
    csv_line += testCasesNames[test_case_number] + " , " + tonality->get_name() +  " , " +
                variable_selection_heuristics_names[variable_selection_heuristic] + " ," +
                value_selection_heuristics_names[value_selection_heuristic] + " , " +
                cutoff_depths_names[cutoff_depth] + " , " + cutoffs_names[cutoff_strategy] + " , " +
                nogoods_depths_names[nogood_depth];

    auto pb = new FourVoiceTexture(size, tonality,testCases[test_case_number][0],qualities,
                                   testCases[test_case_number][3],
                                   variable_selection_heuristic,
                                   value_selection_heuristic);
    /// Search options
    /*
    opts.cutoff = Search::Cutoff::merge(
            Search::Cutoff::linear(4*size*4),
            Search::Cutoff::geometric(4*size*4, 2));
     */
    Search::Options opts;
    opts.threads = 1;
    opts.stop = Search::Stop::time(180000); // stop after 120 seconds
    opts.cutoff = cutoffs[cutoff_strategy];
    opts.nogoods_limit = nogoods_depths[nogood_depth];

    //BAB<FourVoiceTexture> solver(pb, opts);
    /// Restart based solver
    RBS<FourVoiceTexture, BAB> solver(pb, opts);
    delete pb;

    Search::Statistics bestSolStats = solver.statistics();
    string solsAndTime;
    FourVoiceTexture *bestSol = nullptr;
    bool bestSolFound = false;
    auto start = std::chrono::high_resolution_clock::now();     /// start time

    while(FourVoiceTexture *sol = solver.next()){
        auto currTime = std::chrono::high_resolution_clock::now();     /// current time
        bestSol = (FourVoiceTexture*) sol->copy();
        bestSolStats = solver.statistics();
        std::chrono::duration<double> duration = currTime - start;
        solsAndTime += "," + to_string(duration.count()) + " , " + intVarArgs_to_string(sol->get_cost_vector()) + ",";
    }
    string csv_entry;

    auto currTime = std::chrono::high_resolution_clock::now();     /// current time
    std::chrono::duration<double> duration = currTime - start;

    if(bestSol == nullptr){/// no solutions found
        csv_entry = csv_line + "," + to_string(bestSolFound) + " , " + to_string(duration.count());
    }
    else if(solver.stopped()){/// optimal solution not found
        bestSolFound = false;
        csv_entry = csv_line + "," + to_string(bestSolFound) + "," + to_string(duration.count()) + ",,," +
                statistics_to_csv_string(bestSolStats) + intVarArgs_to_string(bestSol->get_cost_vector()) +
                ",,," + statistics_to_csv_string(solver.statistics()) + "," +
                solsAndTime + ",";
    }
    else{ /// optimal solution found
        bestSolFound = true;
        csv_entry = csv_line + "," + to_string(bestSolFound) + "," + to_string(duration.count()) + ",,," +
                           statistics_to_csv_string(bestSolStats) + intVarArgs_to_string(bestSol->get_cost_vector()) +
                           ",,," + statistics_to_csv_string(solver.statistics()) + "," +
                           solsAndTime + ",";
    }
    cout << csv_entry << endl;
    return 0;
}