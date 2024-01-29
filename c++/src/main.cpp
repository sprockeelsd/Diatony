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

    Tonality* tonality = new MajorTonality(C);
    write_to_log_file(time().c_str(), LOG_FILE);

    std::string search_type = argv[1];
    std::string build_midi = argv[2];
    /// vectors representing the chords and the states
    vector<int> chords = {FIRST_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, SECOND_DEGREE, FIFTH_DEGREE, FIRST_DEGREE,
                FOURTH_DEGREE, FIRST_DEGREE, FIFTH_DEGREE, FIRST_DEGREE};
    vector<int> chords_qualities = {MAJOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD,
                MAJOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD};
//    {MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD, DIMINISHED_CHORD, MAJOR_CHORD,
//                MINOR_CHORD, MINOR_CHORD, MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD},
    vector<int> states = {FUNDAMENTAL_STATE, THIRD_INVERSION, FIRST_INVERSION, FIRST_INVERSION, FUNDAMENTAL_STATE,
                FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, SECOND_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE};
//    vector<int> chords = {FIRST_DEGREE, FOURTH_DEGREE, FIFTH_DEGREE, FIRST_DEGREE};
//    vector<int> chords_qualities = {MAJOR_CHORD, MAJOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD};
//    vector<int> states = {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE};
    int size = chords.size();

    /// create a new problem
    auto *pb = new FourVoiceTexture(size, tonality, chords, chords_qualities, states,
                                    RIGHT_TO_LEFT, VAL_MIN);
    std::cout << "problem created" << std::endl;
    std::cout << pb->to_string() << std::endl;

    Search::Options opts;
    opts.stop = Search::Stop::time(60000); // stop after 120 seconds

    BAB<FourVoiceTexture> solver(pb, opts);
    delete pb;

    FourVoiceTexture* currentBestSol = nullptr;
    while(FourVoiceTexture* next_sol = solver.next()){
        std::cout << "temporary solution found" << std::endl;
         currentBestSol = next_sol;
    }
    if(solver.stopped() || currentBestSol == nullptr){
        std::cout << "No solution found within the time limit" << std::endl;
    }
    else{
        std::cout << currentBestSol->to_string() << std::endl;
    }

    return 0;
}