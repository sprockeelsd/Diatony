#include "../headers/FourVoiceTexture.hpp"
#include "../headers/Utilities.hpp"
#include "../headers/Tonality.hpp"
#include "../headers/MajorTonality.hpp"
#include "../headers/MinorTonality.hpp"
#include "../headers/Solver.hpp"
#include "../headers/MidiFile.h"
#include "../headers/MidiFileGeneration.hpp"

#include <gecode/gist.hh>

using namespace Gecode;
using namespace std;
using namespace smf;

/**
 * Finds solutions to a musical problem
 * Takes 2 arguments:
 * - the first one specifies whether we need to find all solutions or just the best one
 * - The second specifies whether we need to create a MIDI file or not
 */
int main(int argc, char* argv[]) {
    // if there is not exactly 2 argument, there is an error
    if(argc != 3)
        return 1;

    Tonality* tonality = new MinorTonality(B_FLAT);
    write_to_log_file(time().c_str(), LOG_FILE);

    std::string search_type = argv[1];
    std::string build_midi = argv[2];
//    vector<int> chords = {FIRST_DEGREE, FIRST_DEGREE, FIFTH_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, SECOND_DEGREE,
//                          FIRST_DEGREE, FIFTH_DEGREE, FIRST_DEGREE};
//    vector<int> chords_qualities = {MINOR_CHORD, MINOR_CHORD, MAJOR_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD,
//                                    DIMINISHED_CHORD, MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD};
//    vector<int> states = {FUNDAMENTAL_STATE, SECOND_INVERSION, FUNDAMENTAL_STATE, THIRD_INVERSION, FIRST_INVERSION,
//                          FIRST_INVERSION, SECOND_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE};

//    vector<int> chords = {FIRST_DEGREE, FIFTH_DEGREE, SIXTH_DEGREE, FIRST_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, THIRD_DEGREE,
//                          SIXTH_DEGREE, SECOND_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, FOURTH_DEGREE, FIFTH_DEGREE, FIFTH_DEGREE,
//                          FIRST_DEGREE};
//    vector<int> chords_qualities = {MAJOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD,
//                             MINOR_CHORD, MINOR_CHORD, MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD, MAJOR_CHORD,
//                             MAJOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD};
//    vector<int> states = {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, SECOND_INVERSION, FUNDAMENTAL_STATE,
//                          FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FIRST_INVERSION, FUNDAMENTAL_STATE,
//                          FIRST_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE};
    /// vectors representing the chords and the states
//    vector<int> chords = {FIRST_DEGREE, SIXTH_DEGREE, FOURTH_DEGREE, FIRST_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, SECOND_DEGREE, FIFTH_DEGREE,
//                          FIRST_DEGREE};
//    vector<int> chords_qualities = {MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MAJOR_CHORD,
//                                    MAJOR_CHORD, MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD};
//    vector<int> states = {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, SECOND_INVERSION, FUNDAMENTAL_STATE,
//                          FUNDAMENTAL_STATE, FIRST_INVERSION, THIRD_INVERSION, FIRST_INVERSION};

    vector<int> chords = {FIRST_DEGREE, FOURTH_DEGREE, SEVENTH_DEGREE, FIRST_DEGREE, THIRD_DEGREE, SIXTH_DEGREE,
                           SECOND_DEGREE, FIFTH_DEGREE, FIRST_DEGREE};
    vector<int> chords_qualities = {MINOR_CHORD, MINOR_CHORD, DIMINISHED_CHORD, MINOR_CHORD, MAJOR_CHORD,
                                           MAJOR_CHORD, DIMINISHED_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD};
    vector<int> states = {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FIRST_INVERSION, FIRST_INVERSION, FUNDAMENTAL_STATE,
                           FUNDAMENTAL_STATE, FIRST_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE};

//    vector<int> chords = {FIRST_DEGREE, FIRST_DEGREE};
//    vector<int> chords_qualities = {MAJOR_CHORD, MAJOR_CHORD};
//    vector<int> states = {FUNDAMENTAL_STATE, FIRST_INVERSION};
    int size = chords.size();

    /// create a new problem
    auto *pb = new FourVoiceTexture(size, tonality, chords, chords_qualities, states,
                                    LEFT_TO_RIGHT, VAL_RND);
    /// find solution(s)
    vector<const FourVoiceTexture *> sols;
    const FourVoiceTexture *sol;
    auto start = std::chrono::high_resolution_clock::now();     /// start time
    if(search_type == "all")
        sols = find_all_solutions(pb, BAB_SOLVER);
    else{
        sol = find_best_solution(pb, 60000); // add the solution to the vector (it only has one element)
        if(sol == nullptr){
            std::cout << "no solutions found in the given time limit." << std::endl;
            return 0;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();       /// end time

    delete pb;

    /// total time spent searching
    std::chrono::duration<double> duration = end - start;
    string m = "Execution time: " + std::to_string(duration.count()) + "seconds.\n";
    std::cout << m << std::endl;
    write_to_log_file(m.c_str(), LOG_FILE);

    /// check wether we have to create a MIDI file or not
    if(build_midi == "true"){
        if(sols.size() > 0 && sols[0] != nullptr)
            writeSolsToMIDIFile(size, sols);
    }

    return 0;
}