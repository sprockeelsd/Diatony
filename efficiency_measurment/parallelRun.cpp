#include "../c++/headers/diatony/FourVoiceTexture.hpp"
#include "../c++/headers/aux/Utilities.hpp"
#include "../c++/headers/aux/Tonality.hpp"
#include "../c++/headers/aux/MajorTonality.hpp"
#include "../c++/headers/aux/MinorTonality.hpp"
#include "../c++/headers/midifile/MidiFile.h"
#include "../c++/headers/aux/MidiFileGeneration.hpp"

#include "TestCases.hpp"

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

    // Generate tonalities
    vector<Tonality*> tonalities;
    for(int i = 0; i < tonics.size(); i++){
        if(modes[i] == MAJOR_MODE)
            tonalities.push_back(new MajorTonality(tonics[i]));
        else
            tonalities.push_back(new MinorTonality(tonics[i]));
    }

    vector<string> tonalityNames;
    tonalityNames.reserve(tonalities.size());
    for(auto & tonality : tonalities)
        tonalityNames.push_back(tonality->get_name());

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
    csv_line += testCasesNames[test_case_number] + " , " + tonality->get_name();

    auto pb = new FourVoiceTexture(size, tonality, testCases[test_case_number][0], qualities,
                                   testCases[test_case_number][3]);
    /// Search options
    int nVariables = testCases[test_case_number][0].size() * 4;

    Cutoff* geo = Search::Cutoff::geometric(nVariables^2, 2);
    Cutoff* lin = Search::Cutoff::linear(nVariables/2);
    Cutoff* lin_geo = Search::Cutoff::merge(lin, geo);

    Search::Options opts;
    opts.threads = 1;
    opts.stop = Search::Stop::time(450000); // stop after 120 seconds
    opts.cutoff = lin_geo;
    opts.nogoods_limit = 4*nVariables;

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
        solsAndTime += to_string(duration.count()) + " , " + intVarArgs_to_string(sol->get_cost_vector()) + ",,";
    }
    string csv_entry;

    auto currTime = std::chrono::high_resolution_clock::now();     /// current time
    std::chrono::duration<double> duration = currTime - start;

    if(bestSol == nullptr){/// no solutions found
        csv_entry = csv_line + "," + to_string(bestSolFound) + " , " + to_string(duration.count());
    }
    csv_entry = csv_line + "," + to_string(!solver.stopped()) + "," + to_string(duration.count()) + ",,," +
        statistics_to_csv_string(bestSolStats) + "," + intVarArgs_to_string(bestSol->get_cost_vector()) +
        ",,," + statistics_to_csv_string(solver.statistics()) + "," +
        solsAndTime + ",";
    cout << csv_entry << endl;
    return 0;
}